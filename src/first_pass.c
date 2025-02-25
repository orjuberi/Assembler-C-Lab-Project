#include "first_pass.h"
#include "symbol_table.h"
#include "common.h"
#include "error_handling.h"
#include "line_parser.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

FirstPassResult first_pass(const char* filename) {
    FirstPassResult result;
    FILE *file;
    char am_filename[MAX_FILE_NAME];
    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    int status;
    int flag = 0;
    AssemblyLine parsed_line;

    /* Initialize result variables */
    result.symbolTable = NULL;
    result.memoryCounters.instructionCounter = 100;
    result.memoryCounters.dataCounter = 0;
    result.errorFlag = NO_ERROR;

    /* Check if filename length is within buffer limit and create .am filename */
    if (strlen(filename) + 3 < sizeof(am_filename)) {
        sprintf(am_filename, "%s.am", filename);
    } else {
        fprintf(stderr, "Filename too long to fit in buffer\n");
    }

    /* Try to open .am file, if not found, open the original file */
    file = fopen(am_filename, "r");
    if (file == NULL) {
        printf("Error: Could not open .am file, trying original file: %s\n", filename);
        file = fopen(filename, "r");
        if (file == NULL) {
            printf("Error: Could not open file %s\n", filename);
            result.errorFlag = ERR_FILE_ACCESS;
            return result;
        }
    }

    /* Process each line in the file */
    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        parsed_line = parse_assembly_line(line, line_number, 1);

        if (parsed_line.error) {
            flag = 1;
        }

        status = process_line_first_pass(&parsed_line, &result.memoryCounters.instructionCounter, 
            &result.memoryCounters.dataCounter, &result.symbolTable, line_number);

        if (status != NO_ERROR) {
            free_assembly_line(&parsed_line);
            result.errorFlag = status;
            break;
        }

        free_assembly_line(&parsed_line);
    }

    /* Update addresses of data symbols */
    update_data_symbols(result.symbolTable, result.memoryCounters.instructionCounter);
    result.memoryCounters.dataCounter += result.memoryCounters.instructionCounter - 100;

    fclose(file);

    /* Set error flag if any errors were encountered */
    if (flag) {
        result.errorFlag = 1;
    }

    return result;
}

int process_line_first_pass(const AssemblyLine *line, int *ic, int *dc, Symbol **symbol_table, int line_number) {
    int status = NO_ERROR;
    int is_data_line;

    /* Check if the line has a label and handle it */
    if (line->label) {
        is_data_line = (line->instruction && (strcmp(line->instruction, ".data") == 0 || strcmp(line->instruction, ".string") == 0));
        status = handle_label_first_pass(line->label, is_data_line ? *dc : *ic, is_data_line, line_number, symbol_table);
        if (status != NO_ERROR) {
            return status;
        }
    }

    /* Handle directives or instructions */
    if (line->instruction && line->instruction[0] == '.') {
        status = handle_directive_first_pass(line, dc, symbol_table, line_number);
    } else if (line->instruction) {
        status = handle_instruction_first_pass(line, ic);
    }

    if (status != NO_ERROR) {
        printf("Error processing line %d: %d\n", line_number, status);
    }

    return status;
}

int handle_label_first_pass(const char *label, int address, int is_data_line, int line, Symbol **symbol_table) {
    Symbol *existing;

    /* Check if the label length is valid */
    if (strlen(label) > MAX_LABEL_LEN) {
        printf("Error: Label too long\n");
        return ERR_SYMBOL_LENGTH;
    }

    /* Check if the label already exists */
    existing = find_symbol(label, *symbol_table);
    if (existing) {
        printf("Error: Duplicate symbol\n");
        return ERR_SYMBOL_DUPLICATE;
    }

    /* Check if the label is an opcode */
    if (check_if_opcode(label)) {
        printf("Error Line %d: %s is restricted name\n", line, label);
        return 1;
    }

    /* Add the symbol to the symbol table */
    if (!add_symbol(symbol_table, label, address, SYMBOL_LABEL, is_data_line, line)) {
        printf("Error: Failed to add symbol\n");
        return ERR_MEMORY_ALLOCATION;
    }

    return NO_ERROR;
}

int handle_directive_first_pass(const AssemblyLine *line, int *dc, Symbol **symbol_table, int line_number) {
    const char *operand_value = line->srcOperand ? line->srcOperand->value : NULL;

    if (strcmp(line->instruction, DATA_DIRECTIVE) == 0) {
        /* Count the number of values in the data directive */
        int count = 0;
        int i;
        for (i = 0; line->original[i] != '\0'; i++) {
            if (line->original[i] == ',') {
                count++;
            }
        }
        *dc += count + 1;
    } else if (strcmp(line->instruction, STRING_DIRECTIVE) == 0) {
        /* Add string length to data counter */
        if (operand_value) {
            *dc += strlen(operand_value) - 2;
        }
    } else if (strcmp(line->instruction, EXTERN_DIRECTIVE) == 0) {
        return handle_extern_directive(line, symbol_table, line_number);
    } else if (strcmp(line->instruction, ENTRY_DIRECTIVE) == 0) {
        return NO_ERROR;
    } else {
        return ERR_DIRECTIVE_UNDEFINED;
    }

    return NO_ERROR;
}

int handle_instruction_first_pass(const AssemblyLine *line, int *ic) {
    (*ic)++; /* Increment for the opcode word */

    /* Handle source operand */
    if (line->srcOperand) {
        if (line->srcOperand->type == OPERAND_IMMEDIATE || line->srcOperand->type == OPERAND_DIRECT || line->srcOperand->type == OPERAND_REGISTER) {
            (*ic)++;
        }
    }

    /* Handle destination operand */
    if (line->destOperand) {
        if (line->destOperand->type == OPERAND_IMMEDIATE || line->destOperand->type == OPERAND_DIRECT || line->srcOperand->type == OPERAND_REGISTER) {
            (*ic)++;
        }
    }

    return NO_ERROR;
}

int handle_extern_directive(const AssemblyLine *line, Symbol **symbol_table, int line_number) {
    const char *symbol_name = line->srcOperand->value;
    Symbol *existing_symbol = find_symbol(symbol_name, *symbol_table);

    /* Check if the symbol already exists */
    if (existing_symbol != NULL) {
        printf("Warning: Symbol %s already exists in symbol table with address: %d\n", symbol_name, existing_symbol->address);
        return NO_ERROR;
    }

    /* Add the external symbol */
    if (!add_symbol(symbol_table, symbol_name, 0, SYMBOL_EXTERN, 0, line_number)) {
        printf("Error: Failed to add extern symbol: %s\n", symbol_name);
        return ERR_MEMORY_ALLOCATION;
    }

    return NO_ERROR;
}

int count_data_values(const char *data_string) {
    int count = 1;
    while (*data_string) {
        if (*data_string == ',') count++;
        data_string++;
    }
    return count;
}

void update_data_symbols(Symbol *symbol_table, int IC) {
    Symbol *current = symbol_table;
    while (current != NULL) {
        if (current->is_data_line) {
            current->address += IC; /* Update data symbol address */
        }
        current = current->next;
    }
}

