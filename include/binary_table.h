#ifndef BINARY_TABLE_H
#define BINARY_TABLE_H

#include "line_parser.h"
#include "symbol_table.h"

/* 
 * @file binary_table.h
 * Declarations for managing the binary table in the assembler.
 */

/* 
 * Structure to represent a binary instruction with opcode, addressing modes, registers, and ARE value.
 */
typedef struct {
    unsigned int opcode : 4;           /* Operation code */
    unsigned int source_addressing : 2; /* Source operand addressing mode */
    unsigned int target_addressing : 2; /* Target operand addressing mode */
    unsigned int source_register : 3;   /* Source register number */
    unsigned int target_register : 3;   /* Target register number */
    unsigned int are : 2;               /* ARE value (absolute, relocatable, external) */
} BinaryInstruction;

/* 
 * Structure to represent a binary word in memory with an address and a 15-bit value.
 */
typedef struct {
    int address;        /* Memory address of the binary word */
    unsigned short value; /* 15-bit binary value */
} BinaryWord;

/* 
 * Structure to represent the binary table used in the assembler.
 */
typedef struct {
    BinaryWord *words;  /* Dynamic array of binary words */
    int size;           /* Current number of binary words in the table */
    int capacity;       /* Maximum capacity of the table */
    int data;           /* Data segment counter */
} BinaryTable;

/* 
 * Initializes a binary table with default size and values.
 * table - Pointer to the binary table to be initialized.
 */
void init_binary_table(BinaryTable *table);

/* 
 * Adds a binary word to the binary table.
 * table - Pointer to the binary table.
 * address - Memory address of the binary word.
 * value - 15-bit value of the binary word.
 * Returns 1 on success, 0 on failure (e.g., table full).
 */
int add_binary_word(BinaryTable *table, int address, unsigned short value);

/* 
 * Writes the binary table to a file.
 * table - Pointer to the binary table.
 * filename - Name of the output file.
 * ic - Instruction counter value.
 * dc - Data counter value.
 * Returns 1 on success, 0 on failure.
 */
int write_binary_table_to_file(const BinaryTable *table, const char *filename, int ic, int dc);

/* 
 * Frees the memory allocated for the binary table.
 * table - Pointer to the binary table.
 */
void free_binary_table(BinaryTable *table);

/* 
 * Retrieves the addressing mode of a given operand.
 * operand - Pointer to the operand.
 * Returns the addressing mode as an integer.
 */
int get_addressing_mode(const Operand *operand);

/* 
 * Converts a binary instruction to an integer representation.
 * instruction - Binary instruction to be converted.
 * Returns the integer representation of the binary instruction.
 */
int binary_instruction_to_int(BinaryInstruction instruction);

/* 
 * Processes an immediate value in assembly code.
 * value - String representation of the immediate value.
 * Returns the processed immediate value as an integer.
 */
int process_immediate_value(const char *value);

/* 
 * Retrieves the register number from a string.
 * value - String representing the register.
 * Returns the register number as an integer.
 */
int get_register_number(const char *value);

/* 
 * Assembles an instruction from the parsed assembly line and symbol table.
 * line - Pointer to the assembly line structure.
 * symbol_table - Pointer to the symbol table.
 * Returns the assembled binary instruction as an unsigned short.
 */
unsigned short assemble_instruction(const AssemblyLine *line, Symbol *symbol_table);

/* 
 * Retrieves the ARE value for a given assembly line.
 * line - Pointer to the assembly line structure.
 * symbol_table - Pointer to the symbol table.
 * Returns the ARE value as an integer.
 */
int get_are_value(const AssemblyLine *line, Symbol *symbol_table);

#endif /* BINARY_TABLE_H */

