#include "binary_table.h"
#include "error_handling.h"
#include "line_parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "common.h"

int decimalToOctal(int decimalNumber) {
    int octalNumber = 0, placeValue = 1;

    /* Convert a decimal number to octal */
    while (decimalNumber > 0) {
        int remainder = decimalNumber % 8;
        octalNumber += remainder * placeValue;
        decimalNumber /= 8;
        placeValue *= 10;
    }

    return octalNumber;
}

void init_binary_table(BinaryTable *table) {
    table->size = 0;
    table->data = 0;
    table->capacity = 10;

    /* Allocate memory for binary words */
    table->words = malloc(sizeof(BinaryWord) * table->capacity);
    if (table->words == NULL) {
        report_error(ERR_MEMORY_ALLOCATION, 0);
        exit(1);
    }
}

int add_binary_word(BinaryTable *table, int address, unsigned short value) {
    BinaryWord *temp;

    /* Check if capacity needs to be doubled */
    if (table->size >= table->capacity) {
        table->capacity *= 2;
        temp = realloc(table->words, sizeof(BinaryWord) * table->capacity);
        if (temp == NULL) {
            report_error(ERR_MEMORY_ALLOCATION, 0);
            free_binary_table(table);
            return 0;
        }
        table->words = temp;
    }

    /* Add the new binary word to the table */
    table->words[table->size].address = address;
    table->words[table->size].value = value & 0x7FFF; /* Ensure 15-bit limit */
    table->size++;
    return 1;
}

int write_binary_table_to_file(const BinaryTable *table, const char *filename, int ic, int dc) {
    FILE *file = NULL;
    int i;
    int instruction_count = 0;
    int data_count = 0;

    if (table->size == 0) {
        return 1;
    }

    /* Open file for writing */
    file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s for writing\n", filename);
        return 0;
    }

    /* Calculate instruction and data counts */
    data_count = table->data;
    instruction_count = table->size - data_count;

    /* Write IC (instruction count) and DC (data count) */
    fprintf(file, "%d %d\n", instruction_count, data_count);

    /* Write binary words to file */
    for (i = 0; i < table->size; i++) {
        fprintf(file, "%04d %05d\n", table->words[i].address, decimalToOctal(table->words[i].value));
    }

    fclose(file);
    return 1;
}

void free_binary_table(BinaryTable *table) {
    if (table->words) {
        free(table->words);
        table->words = NULL;
    }
    table->size = 0;
    table->capacity = 0;
}

int get_register_number(const char *value) {
    /* If the value is indirect, use second character as register number */
    if (value[0] == '*') {
        return value[2] - '0';
    }
    /* Direct register access */
    return value[1] - '0';
}

unsigned short assemble_instruction(const AssemblyLine *line, Symbol *symbol_table) {
    unsigned short binary = 0;
    int opcode;

    if (!line || !line->instruction) {
        fprintf(stderr, "Error: Invalid instruction line\n");
        return 0;
    }

    /* Get opcode from instruction */
    opcode = get_opcode(line->instruction);

    /* Special case for "stop" instruction */
    if (!strcmp(line->instruction, "stop")) {
        return 30724;
    }

    /* Encode opcode */
    binary |= (unsigned short)(opcode << 11);

    /* Process operands */
    if (line->srcOperand && line->destOperand) {
        if (line->srcOperand) {
            binary |= get_addressing_mode(line->srcOperand) << 7;
        }
        if (line->destOperand) {
            binary |= get_addressing_mode(line->destOperand) << 3;
        }
    } else if (line->srcOperand) {
        binary |= get_addressing_mode(line->srcOperand) << 3;
    }

    /* Set ARE bits (default to 4) */
    binary |= 4;

    return binary;
}

int get_are_value(const AssemblyLine *line, Symbol *symbol_table) {
    /* Check if destination operand is an external symbol */
    if (line->destOperand && find_symbol(line->destOperand->value, symbol_table) && symbol_table->type == SYMBOL_EXTERN) {
        return 1;
    }

    /* Check if source operand is an external symbol */
    if (line->srcOperand && find_symbol(line->srcOperand->value, symbol_table) && symbol_table->type == SYMBOL_EXTERN) {
        return 1;
    }

    /* Check if destination operand is a regular symbol */
    if (line->destOperand && find_symbol(line->destOperand->value, symbol_table)) {
        return 2;
    }

    /* Default ARE value */
    return 4;
}

int process_immediate_value(const char *value) {
    const char *num_start = value + 1; /* Skip '#' */
    int immediate_value = atoi(num_start);

    /* Warn if immediate value is out of range */
    if (immediate_value < -2048 || immediate_value > 2047) {
        fprintf(stderr, "Warning: Immediate value %d out of range (-2048 to 2047). Truncating.\n", immediate_value);
        immediate_value &= 0xFFF; /* Truncate to 12 bits */
    }

    return immediate_value;
}

int get_addressing_mode(const Operand *operand) {
    if (operand == NULL) {
        return 0;
    }

    /* Determine addressing mode based on operand type */
    switch (operand->type) {
        case OPERAND_IMMEDIATE:
            return 1;
        case OPERAND_DIRECT:
            return 2;
        case OPERAND_INDIRECT_REGISTER:
            return 4;
        case OPERAND_REGISTER:
            return 8;
        default:
            fprintf(stderr, "Error: Invalid operand type: %d\n", operand->type);
            return -1;
    }
}

int binary_instruction_to_int(BinaryInstruction instruction) {
    /* Encode instruction as an integer */
    return (instruction.opcode << 12) |
           (instruction.source_register << 9) |
           (instruction.source_addressing << 7) |
           (instruction.target_register << 4) |
           (instruction.target_addressing << 2) |
           instruction.are;
}

