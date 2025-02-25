#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include "macro.h"
#include <stdio.h>

/*
 * @file pre_assembler.h
 * Functions for preprocessing the assembly code, handling macros, and preparing the code for the assembler.
 */

/*
 * Checks if a line starts with a specific prefix.
 * line - The line to check.
 * prefix - The prefix to compare.
 * Returns 1 if the line starts with the prefix, 0 otherwise.
 */
int starts_with(const char* line, const char* prefix);

/*
 * Preprocesses the assembly file, expanding macros and handling directives.
 * filename - The name of the assembly file to preprocess.
 * macro_head - The head of the macro list.
 * Returns 1 on success, 0 on failure.
 */
int pre_process(const char *filename, struct macros *macro_head);

/*
 * Compares two strings case-insensitively.
 * str1 - The first string.
 * str2 - The second string.
 * Returns 0 if the strings are equal, non-zero if they are different.
 */
int case_insensitive_compare(const char *str1, const char *str2);

/*
 * Handles the macro definitions in the assembly file.
 * original_filename - The name of the original assembly file.
 * temp_filename - The name of the temporary file to store processed output.
 * macro_head - The head of the macro list.
 * Returns 1 on success, 0 on failure.
 */
int handle_macros(const char *original_filename, const char *temp_filename, struct macros *macro_head);

/*
 * Verifies if the given line contains a valid macro name.
 * line - The line to check.
 * macro_head - The head of the macro list.
 * Returns 1 if the macro name is valid, 0 otherwise.
 */
int verify_macro_name(const char *line, struct macros *macro_head);

/*
 * Inserts a macro definition into the macro list.
 * input_file - The input file containing the macro definition.
 * output_file - The output file to store the expanded macro.
 * macro_head - The head of the macro list.
 * macro_definition - The macro definition to insert.
 * Returns 1 on success, 0 on failure.
 */
int insert_macro(FILE *input_file, FILE *output_file, struct macros **macro_head, const char *macro_definition);

/*
 * Expands a macro and writes its content to the output file.
 * macro - Pointer to the macro to expand.
 * output_file - The output file to write the expanded macro.
 */
void expand_macro(const struct macros *macro, FILE *output_file);

/*
 * Trims leading and trailing whitespace from a string.
 * str - The string to trim.
 */
void trim_whitespace(char *str);

#endif /* PRE_ASSEMBLER_H */

