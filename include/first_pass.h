#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "symbol_table.h"
#include "line_parser.h"

/*
 * @file first_pass.h
 * Functions and structures for the first pass of the assembler.
 */

/*
 * @struct MemoryCounters
 * Tracks the instruction and data counters during the first pass.
 */
typedef struct {
    int instructionCounter;  /* Counter for instruction memory */
    int dataCounter;         /* Counter for data memory */
} MemoryCounters;

/*
 * @struct FirstPassResult
 * Holds the result of the first pass, including the symbol table,
 * memory counters, and error flag.
 */
typedef struct {
    Symbol *symbolTable;      /* Pointer to the symbol table */
    MemoryCounters memoryCounters; /* Memory counters (IC and DC) */
    int errorFlag;            /* Flag indicating if any errors occurred during the first pass */
} FirstPassResult;

/*
 * Executes the first pass of the assembler on the given source file.
 * filename - Name of the source file to process.
 * Returns a FirstPassResult structure containing the result of the first pass.
 */
FirstPassResult first_pass(const char* filename);

/*
 * Processes a single line during the first pass.
 * line - Pointer to the parsed assembly line.
 * ic - Pointer to the instruction counter.
 * dc - Pointer to the data counter.
 * symbol_table - Pointer to the symbol table.
 * line_number - The current line number in the source file.
 * Returns 1 if successful, 0 if an error occurred.
 */
int process_line_first_pass(const AssemblyLine *line, int *ic, int *dc, Symbol **symbol_table, int line_number);

/*
 * Handles a directive (e.g., .data, .string) during the first pass.
 * line - Pointer to the parsed assembly line.
 * dc - Pointer to the data counter.
 * symbol_table - Pointer to the symbol table.
 * line_number - The current line number in the source file.
 * Returns 1 if successful, 0 if an error occurred.
 */
int handle_directive_first_pass(const AssemblyLine *line, int *dc, Symbol **symbol_table, int line_number);

/*
 * Handles a label during the first pass.
 * label - The label to process.
 * address - The memory address associated with the label.
 * is_data_line - Flag indicating if the label is in a data line.
 * line - The current line number in the source file.
 * symbol_table - Pointer to the symbol table.
 * Returns 1 if successful, 0 if an error occurred.
 */
int handle_label_first_pass(const char* label, int address, int is_data_line, int line, Symbol** symbol_table);

/*
 * Handles an instruction during the first pass.
 * line - Pointer to the parsed assembly line.
 * ic - Pointer to the instruction counter.
 * Returns 1 if successful, 0 if an error occurred.
 */
int handle_instruction_first_pass(const AssemblyLine *line, int *ic);

/*
 * Updates the addresses of all data symbols after the first pass.
 * symbol_table - Pointer to the symbol table.
 * ic - The final instruction counter value after the first pass.
 */
void update_data_symbols(Symbol *symbol_table, int ic);

/*
 * Counts the number of data values in a .data directive string.
 * data_string - String containing the .data directive.
 * Returns the number of data values.
 */
int count_data_values(const char *data_string);

/*
 * Handles the .extern directive during the first pass.
 * line - Pointer to the parsed assembly line.
 * symbol_table - Pointer to the symbol table.
 * line_number - The current line number in the source file.
 * Returns 1 if successful, 0 if an error occurred.
 */
int handle_extern_directive(const AssemblyLine *line, Symbol **symbol_table, int line_number);

#endif /* FIRST_PASS_H */

