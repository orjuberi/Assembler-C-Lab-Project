#include "lines.h"
#include "error_handling.h"
#include <stdlib.h>
#include <string.h>

lines* malloc_line(const char *content) {
    lines *new_line = (lines *)malloc(sizeof(lines));
    
    /* Check if memory allocation was successful */
    if (new_line == NULL) {
        report_error(ERR_MEMORY_ALLOCATION, 0);
        exit(1); /* Exit on memory allocation failure */
    }

    /* Copy the content into the line, ensuring null-termination */
    strncpy(new_line->line, content, MAX_LINE_LENGTH - 1);
    new_line->line[MAX_LINE_LENGTH - 1] = '\0'; /* Ensure null-termination */

    new_line->next = NULL;
    return new_line;
}

void free_lines(lines *head) {
    lines *current = head;

    /* Traverse the linked list and free each line */
    while (current != NULL) {
        lines *next = current->next;
        free(current);
        current = next;
    }
}

