#ifndef MACROS_H
#define MACROS_H

#include "lines.h"

/*
 * @file macros.h
 * Defines structures and functions for managing macros in the assembler.
 */

/*
 * @struct macros
 * Represents a macro in the assembler, containing a name, its lines of code,
 * and a pointer to the next macro in a linked list.
 */
struct macros {
    char name[MAX_LINE_LENGTH];  /* Name of the macro */
    struct lines *lines;         /* Lines of code in the macro */
    struct macros *next;         /* Pointer to the next macro in the list */
};

/*
 * Creates a new macro node and inserts it into the macro list.
 * macro_name - Name of the new macro.
 * ptr_to_head - Pointer to the pointer of the head of the macro list.
 * Returns a pointer to the newly created macro node, or NULL if allocation fails.
 */
struct macros *create_macro_node(const char *macro_name, struct macros **ptr_to_head);

/*
 * Checks if a macro with the given name already exists in the list.
 * head - Pointer to the head of the macro list.
 * name - Name of the macro to check for.
 * Returns a pointer to the macro if found, or NULL if not.
 */
struct macros *is_existing_macro(struct macros *head, const char *name);

/*
 * Checks if a line contains no macro name.
 * line - The line of code to check.
 * Returns 1 if no macro name is found, 0 otherwise.
 */
int no_macro_name(const char* line);

/*
 * Checks if a line contains extra macro names.
 * line - The line of code to check.
 * Returns 1 if extra macro names are found, 0 otherwise.
 */
int extra_macro_name(const char* line);

/*
 * Frees the memory of the macro list.
 * head - Pointer to the head of the macro list.
 */
void free_macros(struct macros *head);

#endif /* MACROS_H */

