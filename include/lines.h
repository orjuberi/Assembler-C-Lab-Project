#ifndef LINES_H
#define LINES_H

#include "common.h"

/*
 * @file lines.h
 * Defines structures and functions for managing linked list of lines.
 */

/*
 * @struct lines
 * Represents a single line of text in a linked list.
 */
typedef struct lines {
    char line[MAX_LINE_LENGTH]; /* Content of the line */
    struct lines *next;         /* Pointer to the next line in the list */
} lines;

/*
 * Allocates memory for a new line and copies the content into it.
 * content - The content to be stored in the new line.
 * Returns a pointer to the newly allocated line, or NULL if allocation failed.
 */
lines* malloc_line(const char *content);

/*
 * Frees the memory of a linked list of lines.
 * head - Pointer to the head of the linked list.
 */
void free_lines(lines *head);

#endif /* LINES_H */

