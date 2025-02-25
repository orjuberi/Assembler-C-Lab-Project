#include "second_pass.h"
#include "error_handling.h"
#include "line_parser.h"
#include "binary_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "common.h"
#include "utils.h"


void add_operand_word(BinaryTable *table, int *IC, const Operand *op, Symbol *symbol_table);
void remove_newline(char* str);

/* Removing extra characters from the lines before writing them to output files */
void remove_newline(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n' || str[i] == '\r') {
            int j = i;
            while (str[j] != '\0') {
                str[j] = str[j + 1];
                j++;
            }
            break;
        }
        i++;
    }
    i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n' || str[i] == '\r') {
            int j = i;
            while (str[j] != '\0') {
                str[j] = str[j + 1];
                j++;
            }
            break;
        }
        i++;
    }
}

void free_extern_references() {
    ExternReference *ref;
    ExternReference *next;

    ref = extern_references;
    while (ref != NULL) {
        next = ref->next;
        free(ref);
        ref = next;
    }
    extern_references = NULL;
}

/* Processing Symbol in Second Pass */
void process_symbol_in_second_pass(const char *symbol_name, int line_number, Symbol *symbol_table) {
    Symbol *symbol;

    if (symbol_name[0] == '#' || symbol_name[0] == 'r' || (symbol_name[0] == '*' && symbol_name[1] == 'r')) {
        return;
    }

    symbol = find_symbol(symbol_name, symbol_table);

    if (symbol == NULL) {
        fprintf(stderr, "Error: Symbol not found: %s\n", symbol_name);
        exit(ERR_SYMBOL_NOT_FOUND);
    }

    if (symbol->type == SYMBOL_EXTERN) {

        add_extern_reference(symbol_name, line_number);
    }
}

/* Processing Oprand */
int process_operand(const Operand *operand, Symbol *symbol_table, BinaryTable *binary_table, int *current_address, const Operand* src, const Operand* dest) {
    unsigned short binary_word;
    int srcVal = 0;
    int destVal = 0;

    switch (operand->type) {
        case OPERAND_IMMEDIATE:
            binary_word = (((process_immediate_value(operand->value) & 0x1FFF) << 3 | 0x4));
        break;
        case OPERAND_DIRECT: {
            Symbol *symbol = find_symbol(operand->value, symbol_table);
            if (symbol == NULL) {
                fprintf(stderr, "Error: Symbol not found: %s\n", operand->value);
                return ERR_SYMBOL_NOT_FOUND;
            }
            if (symbol->type == SYMBOL_EXTERN) {
                binary_word = 0x0001;
                add_extern_reference(operand->value, *current_address);
            } else {
                binary_word = (symbol->address & 0x1FFF) << 3 | 0x2;
            }
            break;
        }

        case OPERAND_INDIRECT_REGISTER:
            if (src->value[0] != '*') {
                return 0;
            }
        case OPERAND_REGISTER:{

            if (src->value[0] == 'r') {
                srcVal = src->value[1] - '0';
            }
            else if (src->value[0] == '*') {
                srcVal = src->value[2] - '0';
            }

            if (dest) {
                if (dest->value[0] == 'r') {
                    destVal = dest->value[1] - '0';
                }
                else if (dest->value[0] == '*') {
                    destVal = dest->value[2] - '0';
                }
            }

            if (dest) {
                binary_word = (srcVal << 6) | (destVal << 3) | 4;
            }
            else {
                binary_word = (srcVal << 3) | 4;
            }
        }
        break;
        default:
            return ERR_INVALID_OPERAND;
    }

    if (!add_binary_word(binary_table, *current_address, binary_word)) {
        return ERR_MEMORY_ALLOCATION;
    }

    (*current_address)++;
    return NO_ERROR;
}

int resolve_symbol(const char *symbol_name, Symbol *symbol_table) {
    Symbol *current;
    current = symbol_table;
    while (current != NULL) {
        if (strcmp(current->name, symbol_name) == 0) {
            return current->address;
        }
        current = current->next;
    }
    return -1;
}

/* Handling Data Directive */
int handle_data_directive2(const AssemblyLine* line, BinaryTable* binary_table, int* dc) {
    char* line_copy;
    char* token;
    int value = 0;

    line_copy = (char*)malloc(strlen(line->original) + 1);

    if (!line_copy) {
        return 0;
    }
    strcpy(line_copy, line->original);

    token = strtok(line_copy, ":");

    if (token && strchr(line->original, ':')) {
        token = strtok(NULL, " \t\n");
    }
    else {
        token = strtok(line_copy, " \t\n");
    }

    if (token) {
        token = strtok(NULL, ",");
        while (token) {

            value = atoi(token);
            add_binary_word(binary_table, *dc, (unsigned short)value);
            binary_table->data++;
            (*dc)++;
            token = strtok(NULL, " \t\n");
        }
    }

    return 1; /* Success */
}

/* Handling String Directive */
int handle_data_directive(const AssemblyLine* line, BinaryTable* binary_table, int* dc) {
    int value;

    if (!(strcmp(line->instruction, ".string"))) {
        int i = 1;
        while (line->srcOperand->value[i + 2] && line->srcOperand->value[i] != '\"') {
            value = line->srcOperand->value[i];
            add_binary_word(binary_table, *dc, (unsigned short)value);
            binary_table->data++;
            (*dc)++;
            i++;
        }
        add_binary_word(binary_table, *dc, (unsigned short)0);
        binary_table->data++;
        (*dc)++;
    }
    else if (!(strcmp(line->instruction, ".data"))) {
        handle_data_directive2(line, binary_table, dc);
    }
    return 1; /* Success */
}

int handle_entry_directive(const AssemblyLine *line, Symbol *symbol_table) {

    const char *symbol_name;
    Symbol *symbol;
    if (line->srcOperand == NULL || line->srcOperand->value == NULL) {
        printf("Error: Entry directive without operand\n");
        return ERR_INVALID_OPERAND;
    }
    symbol_name = line->srcOperand->value;

    symbol = find_symbol(symbol_name, symbol_table);
    if (symbol == NULL) {
        printf("Error: Entry symbol not found: %s\n", symbol_name);
        return ERR_SYMBOL_NOT_FOUND;
    }
    symbol->type = SYMBOL_ENTRY;
    return NO_ERROR;
}

int write_entry_file(const char *base_name, const Symbol *symbol_table) {
    char *ent_filename;
    FILE *ent_file;
    const Symbol *sym = symbol_table;

    int count = 0;

    while (sym) {
        if (sym->type == SYMBOL_ENTRY) {
            count++;
        }
        sym = sym->next;
    }

    if (!count) {
        return 1;
    }

    sym = symbol_table;

    ent_filename = add_file_extension(base_name, ".ent");

    ent_file = fopen(ent_filename, "w");
    if (ent_file == NULL) {
        fprintf(stderr, "Error: Could not open %s for writing\n", ent_filename);
        free(ent_filename);
        return 0;
    }

    while (sym) {
        if (sym->type == SYMBOL_ENTRY) {
            remove_newline(sym->name);
            fprintf(ent_file, "%s %04d\n", sym->name, sym->address);
        }
        sym = sym->next;
    }
    fclose(ent_file);
    free(ent_filename);
    return 1;
}

int write_extern_file(const char *base_name) {
    char *ext_filename;

    FILE *ext_file;
    const ExternReference *ext_ref = get_extern_references();

    if (!ext_ref) {
        return 1;
    }

    ext_filename = add_file_extension(base_name, ".ext");
    ext_file = fopen(ext_filename, "w");
    if (ext_file == NULL) {
        fprintf(stderr, "Error: Could not open %s for writing\n", ext_filename);
        free(ext_filename);
        return 0;
    }

    while (ext_ref) {
        remove_newline(ext_ref->name);
        fprintf(ext_file, "%s %04d\n", ext_ref->name, ext_ref->address);
        ext_ref = ext_ref->next;
    }

    fclose(ext_file);
    free(ext_filename);
    return 1;
}

int process_line_second_pass(const AssemblyLine *line, Symbol *symbol_table, BinaryTable *binary_table, int *ic) {
    if (line->instruction && line->instruction[0] == '.') {
        if (strcmp(line->instruction, ENTRY_DIRECTIVE) == 0) {
            return handle_entry_directive(line, symbol_table);
        } else if (strcmp(line->instruction, EXTERN_DIRECTIVE) == 0) {
            return NO_ERROR;
        }
        else {
            handle_data_directive(line, binary_table, ic);
        }
    } else if (line->instruction) {
        unsigned short binary_instruction = assemble_instruction(line, symbol_table);

        add_binary_word(binary_table, *ic, binary_instruction);
        (*ic)++;
        if (line->srcOperand) {
            process_operand(line->srcOperand, symbol_table, binary_table, ic, line->srcOperand, line->destOperand);
        }
        if (line->destOperand) {
            if (line->srcOperand && !(line->srcOperand->value[0] == 'r' && line->destOperand->value[0] == 'r')){
                process_operand(line->destOperand, symbol_table, binary_table, ic, line->srcOperand, line->destOperand);
            }
        }
    }
    return NO_ERROR;
}

void add_operand_word(BinaryTable *table, int *IC, const Operand *op, Symbol *symbol_table) {

    unsigned short word = 0;
    const Symbol *sym;

    switch (op->type) {
        case OPERAND_IMMEDIATE:
            word = (atoi(op->value + 1) & 0x3FF);
            break;
        case OPERAND_DIRECT:
            sym = find_symbol(op->value, symbol_table);
            if (sym == NULL) {
                fprintf(stderr, "Error: Symbol not found: %s\n", op->value);
                return;
            }
            word = sym->address & 0x3FF;
            if (sym->type == SYMBOL_EXTERN) {
                add_extern_reference(op->value, *IC);
            }
            break;
        case OPERAND_INDIRECT_REGISTER:
        case OPERAND_REGISTER:
            word = (atoi(op->value + 1) & 0x7) << 8;
            break;
    }
    add_binary_word(table, *IC, word);
    (*IC)++;
}

void write_output_files(const BinaryTable *table, const Symbol *symbol_table, const char *filename) {

    char *base_name;
    char *ob_filename;
    int ic = 100;
    int dc = count_data_symbols(symbol_table);

    base_name = remove_extension(filename);
    if (base_name == NULL) {
        fprintf(stderr, "Error: Failed to remove extension from filename\n");
        return;
    }

    ob_filename = add_file_extension(base_name, ".ob");

    if (!write_binary_table_to_file(table, ob_filename, ic, dc)) {
        fprintf(stderr, "Error: Failed to write .ob file\n");
    }

    if (!write_entry_file(base_name, symbol_table)) {
        fprintf(stderr, "Error: Failed to write entry file\n");
    }
    if (!write_extern_file(base_name)) {
        fprintf(stderr, "Error: Failed to write extern file\n");
    }

    free(base_name);
    free(ob_filename);
}

int second_pass(const char *filename, Symbol *symbol_table) {
    int status;
    int IC = 100;
    BinaryTable binary_table;
    FILE *file;
    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    char filename_with_extension[MAX_FILE_NAME];
    AssemblyLine parsed_line;


    sprintf(filename_with_extension, "%s.am", filename);
    file = fopen(filename_with_extension, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename_with_extension);
        return ERR_FILE_ACCESS;
    }

    init_binary_table(&binary_table);

    /* Process instructions */
    while (fgets(line, sizeof(line), file)) {
        line_number++;

        parsed_line = parse_assembly_line(line, line_number, 0);
        if (parsed_line.instruction == NULL) {
            free_assembly_line(&parsed_line);
            continue;
        }

        status = process_line_second_pass(&parsed_line, symbol_table, &binary_table, &IC);
        if (status != NO_ERROR) {
            fprintf(stderr, "Error on line %d: %s", line_number, line);
            free_assembly_line(&parsed_line);
            fclose(file);
            free_binary_table(&binary_table);
            return status;
        }

        free_assembly_line(&parsed_line);
    }

    write_output_files(&binary_table, symbol_table, filename);
    free_binary_table(&binary_table);
    fclose(file);
    return 0;
}