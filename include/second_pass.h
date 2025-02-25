#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "symbol_table.h"
#include "binary_table.h"
#include "line_parser.h"

/*
 * @file second_pass.h
 * Functions for the second pass of the assembler, handling symbol resolution, entry/external files, and generating output.
 */

/*
 * Executes the second pass of the assembler on the given source file.
 * filename - The name of the source file.
 * symbol_table - Pointer to the symbol table.
 * Returns 1 on success, 0 on failure.
 */
int second_pass(const char *filename, Symbol *symbol_table);

/*
 * Processes a single line during the second pass.
 * line - Pointer to the parsed assembly line.
 * symbol_table - Pointer to the symbol table.
 * binary_table - Pointer to the binary table for storing instructions.
 * ic - Pointer to the instruction counter.
 * Returns 1 if successful, 0 on failure.
 */
int process_line_second_pass(const AssemblyLine *line, Symbol *symbol_table, BinaryTable *binary_table, int *ic);

/*
 * Resolves a symbol name in the symbol table.
 * symbol_name - The name of the symbol to resolve.
 * symbol_table - Pointer to the symbol table.
 * Returns the address of the symbol, or -1 if not found.
 */
int resolve_symbol(const char *symbol_name, Symbol *symbol_table);

/*
 * Handles the .entry directive during the second pass.
 * line - Pointer to the parsed assembly line.
 * symbol_table - Pointer to the symbol table.
 * Returns 1 if successful, 0 on failure.
 */
int handle_entry_directive(const AssemblyLine* line, Symbol* symbol_table);

/*
 * Writes the entry symbols to an entry file.
 * base_name - Base name of the output file (without extension).
 * symbol_table - Pointer to the symbol table.
 * Returns 1 on success, 0 on failure.
 */
int write_entry_file(const char *base_name, const Symbol *symbol_table);

/*
 * Processes an operand during the second pass.
 * operand - Pointer to the operand to process.
 * symbol_table - Pointer to the symbol table.
 * binary_table - Pointer to the binary table.
 * current_address - Pointer to the current address being processed.
 * src - Pointer to the source operand, if applicable.
 * dest - Pointer to the destination operand, if applicable.
 * Returns 1 if successful, 0 on failure.
 */
int process_operand(const Operand *operand, Symbol *symbol_table, BinaryTable *binary_table, int *current_address, const Operand* src, const Operand* dest);

/*
 * Writes the external symbols to an extern file.
 * base_name - Base name of the output file (without extension).
 * Returns 1 on success, 0 on failure.
 */
int write_extern_file(const char *base_name);

/*
 * Processes a symbol during the second pass.
 * symbol_name - The name of the symbol to process.
 * line_number - The current line number being processed.
 * symbol_table - Pointer to the symbol table.
 */
void process_symbol_in_second_pass(const char *symbol_name, int line_number, Symbol *symbol_table);

/*
 * Writes the final output files (object, entry, and extern) after the second pass.
 * table - Pointer to the binary table.
 * symbol_table - Pointer to the symbol table.
 * filename - The base name of the output file (without extension).
 */
void write_output_files(const BinaryTable *table, const Symbol *symbol_table, const char *filename);

#endif /* SECOND_PASS_H */

