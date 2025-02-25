#include "error_handling.h"
#include <stdio.h>
#include <stdlib.h>

void report_error(ErrorCode error, int line_number) {
    /* Handle different error cases */
    switch (error) {
        case ERR_FILE_ACCESS:
            fprintf(stderr, "Error on line %d: File access error\n", line_number);
            break;

        case ERR_MEMORY_ALLOCATION:
            fprintf(stderr, "Error on line %d: Memory allocation failed\n", line_number);
            exit(1); /* Memory allocation failure is critical */
            break;

        case ERR_MACRO_NAME_ERROR:
            fprintf(stderr, "Error on line %d: Macro name error\n", line_number);
            break;

        case ERR_INSTRUCTION_INVALID:
            fprintf(stderr, "Error on line %d: Invalid instruction\n", line_number);
            break;

        case ERR_SYMBOL_DUPLICATE:
            fprintf(stderr, "Error on line %d: Duplicate symbol\n", line_number);
            break;

        case ERR_SYMBOL_SYNTAX:
            fprintf(stderr, "Error on line %d: Symbol syntax is wrong\n", line_number);
            break;

        case ERR_SYMBOL_LENGTH:
            fprintf(stderr, "Error on line %d: Symbol too long\n", line_number);
            break;

        case ERR_SYMBOL_OPCODE:
            fprintf(stderr, "Error on line %d: Symbol name is an opcode\n", line_number);
            break;

        case ERR_SYMBOL_REGISTER:
            fprintf(stderr, "Error on line %d: Symbol name is a register\n", line_number);
            break;

        case ERR_SYMBOL_MACRO:
            fprintf(stderr, "Error on line %d: Symbol name is a macro\n", line_number);
            break;

        case ERR_DATA_SYNTAX:
            fprintf(stderr, "Error on line %d: Invalid data directive syntax\n", line_number);
            break;

        case ERR_MEMORY_OVERFLOW:
            fprintf(stderr, "Error on line %d: Memory overflow\n", line_number);
            break;

        case ERR_DIRECTIVE_UNDEFINED:
            fprintf(stderr, "Error on line %d: Undefined directive\n", line_number);
            break;

        case ERR_STRING_SYNTAX:
            fprintf(stderr, "Error on line %d: Invalid string syntax\n", line_number);
            break;

        case ERR_OPERANDS_INSUFFICIENT:
            fprintf(stderr, "Error on line %d: Not enough operands\n", line_number);
            break;

        case ERR_OPERAND_EMPTY:
            fprintf(stderr, "Error on line %d: Operand is empty\n", line_number);
            break;

        case ERR_INTEGER_INVALID:
            fprintf(stderr, "Error on line %d: Invalid integer\n", line_number);
            break;

        case ERR_OPERANDS_TOO_MANY:
            fprintf(stderr, "Error on line %d: Too many operands\n", line_number);
            break;

        case ERR_MIUN_MISMATCH:
            fprintf(stderr, "Error on line %d: Miun types do not match\n", line_number);
            break;

        case ERR_SYMBOL_SHORT:
            fprintf(stderr, "Error on line %d: Symbol name too short\n", line_number);
            break;

        case ERR_SYMBOL_NOT_FOUND:
            fprintf(stderr, "Error on line %d: Symbol not found\n", line_number);
            break;

        case ERR_INVALID_OPERAND:
            fprintf(stderr, "Error on line %d: Invalid operand\n", line_number);
            break;

        default:
            fprintf(stderr, "Unknown error occurred on line %d\n", line_number);
            break;
    }
}

