#include "line_parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

/* Define the opcodes and their corresponding attributes */
const Opcode OPCODES[] = {
    {"mov", 0, 2}, {"cmp", 1, 2}, {"add", 2, 2}, {"sub", 3, 2},
    {"lea", 4, 2}, {"clr", 5, 1}, {"not", 6, 1}, {"inc", 7, 1},
    {"dec", 8, 1}, {"jmp", 9, 1}, {"bne", 10, 1}, {"red", 11, 1},
    {"prn", 12, 1}, {"jsr", 13, 1}, {"rts", 14, 0}, {"stop", 15, 0}
};

const int OPCODES_COUNT = sizeof(OPCODES) / sizeof(OPCODES[0]);

/* Return the number of operands expected for a given opcode */
int get_operand(int opcode) {
    if (opcode < 0 || opcode > 15)
        return -1; /* Invalid instruction */
    else if (opcode < 5)
        return 2;
    else if (opcode < 14)
        return 1;
    return 0;
}

/* Get the opcode index for a given instruction name */
int get_opcode(const char* instruction) {
    size_t i;
    for (i = 0; i < OPCODES_COUNT; i++) {
        if (strcmp(instruction, OPCODES[i].name) == 0) {
            return i;
        }
    }
    return -1; /* Invalid instruction */
}

/* Determine the type of operand based on its syntax */
OperandType determine_operand_type(const char *operand) {
    if (operand[0] == '#') {
        return OPERAND_IMMEDIATE;
    } else if (operand[0] == '*') {
        return OPERAND_INDIRECT_REGISTER;
    } else if (operand[0] == 'r') {
        int reg_num = operand[1] - '0';
        if (reg_num >= r0 && reg_num <= r7) {
            return OPERAND_REGISTER;
        }
    }
    return OPERAND_DIRECT;
}

/* Parse a line of assembly code */
AssemblyLine parse_assembly_line(const char* line, int line_number, int pass) {
    AssemblyLine result;
    char* line_copy, *token;
    int opcode = 0;
    int operand = 0;
    result.error = 0;

    memset(&result, 0, sizeof(AssemblyLine));

    /* Create copies of the line */
    line_copy = (char*)malloc(strlen(line) + 1);
    result.original = (char*)malloc(strlen(line) + 1);
    if (!line_copy) {
        return result;
    }

    strcpy(line_copy, line);
    strcpy(result.original, line);

    /* Check if the line contains a label */
    token = strtok(line_copy, ":");
    if (token && strchr(line, ':')) {
        result.label = (char*)malloc(strlen(token) + 1);
        if (result.label) {
            strcpy(result.label, token);
        }
        token = strtok(NULL, " \t\n");

        /* If the token is null and we are in the first pass, it's an empty label */
        if (!token && pass) {
            fprintf(stderr, "Error line %d: %s is an empty label\n", line_number, result.label);
            result.error = 1;
        }
    } else {
        token = strtok(line_copy, " \t\n");
    }

    /* Process the instruction and operands */
    if (token) {
        result.instruction = (char*)malloc(strlen(token) + 1);
        if (result.instruction) {
            strcpy(result.instruction, token);
        }

        opcode = get_opcode(result.instruction);
        operand = get_operand(opcode);
        token = strtok(NULL, ",");

        /* Validate operand counts */
        if (operand == 0 && token && pass) {
            fprintf(stderr, "Error line %d: Additional Operands\n", line_number);
            result.error = 1;
        } else if (operand == 2 && !token && pass) {
            fprintf(stderr, "Error line %d: Missing Operand 1\n", line_number);
            result.error = 1;
        } else if (operand == 1 && !token && pass) {
            fprintf(stderr, "Error line %d: Missing Operand\n", line_number);
            result.error = 1;
        }

        /* Parse the source operand */
        if (token) {
            result.srcOperand = (Operand*)malloc(sizeof(Operand));
            if (result.srcOperand) {
                result.srcOperand->value = (char*)malloc(strlen(token) + 1);
                if (result.srcOperand->value) {
                    strcpy(result.srcOperand->value, token);
                }
                result.srcOperand->type = determine_operand_type(token);
            }

            token = strtok(NULL, " \t\n");

            /* Check for additional operands where not expected */
            if (operand == 1 && token && pass) {
                fprintf(stderr, "Error line %d: Additional Operands\n", line_number);
                result.error = 1;
            } else if (operand == 2 && !token && pass) {
                fprintf(stderr, "Error line %d: Missing Operand 2\n", line_number);
                result.error = 1;
            }

            /* Parse the destination operand */
            if (token) {
                result.destOperand = (Operand*)malloc(sizeof(Operand));
                if (result.destOperand) {
                    result.destOperand->value = (char*)malloc(strlen(token) + 1);
                    if (result.destOperand->value) {
                        strcpy(result.destOperand->value, token);
                    }
                    result.destOperand->type = determine_operand_type(token);
                }
            }

            token = strtok(NULL, " \t\n");

            /* Handle case with extra operands in two-operand instructions */
            if (operand == 2 && token && pass) {
                fprintf(stderr, "Error line %d: Additional Operands\n", line_number);
                result.error = 1;
            }
        }
    }

    free(line_copy);
    return result;
}

/* Print the contents of an AssemblyLine */
void print_assembly_line(const AssemblyLine *line) {
    if (line == NULL) {
        fprintf(stderr, "Error: Assembly line is NULL\n");
        return;
    }
    printf("Label: %s\n", line->label ? line->label : "(none)");
    printf("Instruction: %s\n", line->instruction ? line->instruction : "(none)");
    printf("Operands: %s\n", line->operands ? line->operands : "(none)");
}

/* Free memory allocated for an AssemblyLine */
void free_assembly_line(const AssemblyLine *line) {
    if (line) {
        free(line->label);
        free(line->instruction);
        free(line->operands);
        free_operand(line->srcOperand);
        free_operand(line->destOperand);
    }
}

/* Free memory allocated for an Operand */
void free_operand(Operand *op) {
    if (op) {
        free(op->value);
        free(op);
    }
}

