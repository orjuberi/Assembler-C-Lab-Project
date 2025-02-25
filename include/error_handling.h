#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

/*
 * @file error_handling.h
 * Error codes and functions for handling errors in the assembler.
 */

/* 
 * @enum ErrorCode
 * Enumeration of possible error codes in the assembler.
 */
typedef enum {
    NO_ERROR = 0,              /* No error occurred */
    ERR_FILE_ACCESS,           /* Error accessing a file */
    ERR_MEMORY_ALLOCATION,     /* Memory allocation failed */
    ERR_MACRO_NAME_ERROR,      /* Invalid macro name */
    ERR_INSTRUCTION_INVALID,   /* Invalid instruction encountered */
    ERR_SYMBOL_DUPLICATE,      /* Duplicate symbol found */
    ERR_SYMBOL_SYNTAX,         /* Syntax error in symbol definition */
    ERR_SYMBOL_LENGTH,         /* Symbol length exceeds allowed limit */
    ERR_SYMBOL_OPCODE,         /* Symbol matches an opcode name */
    ERR_SYMBOL_REGISTER,       /* Symbol matches a register name */
    ERR_SYMBOL_MACRO,          /* Symbol matches a macro name */
    ERR_DATA_SYNTAX,           /* Syntax error in data directive */
    ERR_MEMORY_OVERFLOW,       /* Memory overflow error */
    ERR_DIRECTIVE_UNDEFINED,   /* Undefined directive encountered */
    ERR_STRING_SYNTAX,         /* Syntax error in string directive */
    ERR_OPERANDS_INSUFFICIENT, /* Insufficient operands for the instruction */
    ERR_OPERAND_EMPTY,         /* Empty operand found */
    ERR_INTEGER_INVALID,       /* Invalid integer value */
    ERR_OPERANDS_TOO_MANY,     /* Too many operands provided */
    ERR_MIUN_MISMATCH,         /* Addressing mode mismatch */
    ERR_SYMBOL_SHORT,          /* Symbol name is too short */
    ERR_SYMBOL_NOT_FOUND,      /* Symbol not found in symbol table */
    ERR_INVALID_OPERAND,       /* Invalid operand provided */
    ERR_PROCESSING_FAILED      /* General error during processing */
} ErrorCode;

/* 
 * Reports an error with a specific error code and line number.
 * error - The error code to report.
 * line_number - The line number where the error occurred.
 */
void report_error(ErrorCode error, int line_number);

/* 
 * Validates a label name according to assembler rules.
 * label - The label to validate.
 * Returns 1 if valid, 0 if invalid.
 */
int validate_label(const char *label);

/* 
 * Validates a macro name according to assembler rules.
 * name - The macro name to validate.
 * Returns 1 if valid, 0 if invalid.
 */
int validate_macro_name(const char *name);

#endif /* ERROR_HANDLING_H */

