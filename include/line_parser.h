#ifndef LINE_PARSER_H
#define LINE_PARSER_H

/*
 * @file line_parser.h
 * Definitions and functions for parsing assembly lines.
 */

/*
 * @enum Register
 * Enumeration of the available registers in the assembler.
 */
typedef enum {
    r0 = 0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7
} Register;

/*
 * @enum OperandType
 * Enumeration of the possible types of operands.
 */
typedef enum {
    OPERAND_IMMEDIATE,          /* Immediate value operand */
    OPERAND_DIRECT,             /* Direct addressing operand */
    OPERAND_INDIRECT_REGISTER,  /* Indirect register addressing */
    OPERAND_REGISTER            /* Register operand */
} OperandType;

/*
 * @struct Operand
 * Represents an operand in an assembly instruction.
 */
typedef struct Operand {
    char *value;        /* Value of the operand (as a string) */
    OperandType type;   /* Type of the operand */
} Operand;

/*
 * @struct Opcode
 * Represents an opcode and its attributes.
 */
typedef struct {
    char *name;     /* Name of the opcode */
    int opcode;     /* Numeric code of the opcode */
    int operands;   /* Number of operands required */
} Opcode;

/*
 * @struct AssemblyLine
 * Represents a single parsed line of assembly code.
 */
typedef struct AssemblyLine {
    char *label;        /* Label of the line, if present */
    char *instruction;  /* Instruction part of the line */
    char *operands;     /* Operands part of the line */
    Operand *srcOperand;    /* Source operand */
    Operand *destOperand;   /* Destination operand */
    char *original;     /* The original line (before parsing) */
    const Opcode *opcode;   /* Pointer to the associated opcode */
    int error;          /* Error flag for the line */
} AssemblyLine;

/*
 * Parses an assembly line and returns the result in an AssemblyLine structure.
 * line - The assembly line as a string.
 * line_number - The line number in the source file.
 * pass - Indicates the current pass (first or second).
 * Returns an AssemblyLine structure containing the parsed components.
 */
AssemblyLine parse_assembly_line(const char* line, int line_number, int pass);

/*
 * Prints the contents of a parsed AssemblyLine structure.
 * parsedLine - Pointer to the parsed assembly line to be printed.
 */
void print_assembly_line(const AssemblyLine *parsedLine);

/*
 * Frees the memory allocated for an AssemblyLine structure.
 * line - Pointer to the AssemblyLine structure to be freed.
 */
void free_assembly_line(const AssemblyLine *line);

/*
 * Frees the memory allocated for an Operand structure.
 * operand - Pointer to the Operand structure to be freed.
 */
void free_operand(Operand *operand);

/*
 * Retrieves the number of operands required for a given opcode.
 * opcode - The numeric opcode.
 * Returns the number of operands.
 */
int get_operand(int opcode);

/*
 * Retrieves the opcode for a given instruction name.
 * instruction - The instruction name as a string.
 * Returns the numeric opcode, or -1 if not found.
 */
int get_opcode(const char* instruction);

/*
 * External array of opcode definitions.
 */
extern const Opcode OPCODES[];

/*
 * External constant representing the number of opcodes.
 */
extern const int OPCODES_COUNT;

#endif /* LINE_PARSER_H */

