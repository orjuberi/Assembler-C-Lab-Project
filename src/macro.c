#include "macro.h"
#include "error_handling.h"
#include <stdlib.h>
#include <string.h>
#include "lines.h"

struct macros *create_macro_node(const char *macro_name, struct macros **ptr_to_head) {
    struct macros *head = *ptr_to_head;
    struct macros *new_macro = malloc(sizeof(struct macros));
    if (new_macro == NULL) {
        report_error(ERR_MEMORY_ALLOCATION, 0);
        return NULL;
    }
    strncpy(new_macro->name, macro_name, MAX_LINE_LENGTH - 1);
    new_macro->name[MAX_LINE_LENGTH - 1] = '\0'; /* Ensure null-termination */
    new_macro->lines = NULL;
    new_macro->next = NULL;
    if (*ptr_to_head == NULL) {
        *ptr_to_head = new_macro;
        return new_macro;
    }
    while (head->next != NULL) {
        head = head->next;
    }
    head->next = new_macro;
    return new_macro;
}
struct macros *is_existing_macro(struct macros *head, const char *name) {
    struct macros *current = head;
    char macro_name[MAX_LINE_LENGTH];
    strncpy(macro_name, name, MAX_LINE_LENGTH - 1);
    macro_name[MAX_LINE_LENGTH - 1] = '\0'; /* Ensure null-termination */
    while (current != NULL) {
        if (strncmp(current->name, macro_name, strlen(macro_name)-2) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int no_macro_name(const char* line) {
    int i = 5;

    if (strlen(line) <= i) {
        return 1;
    }

    while ((line[i] == ' ' || line[i] == '\t')){
        i++;
    }
    if (line[i] == '\0' || line[i] == '\n') {
        return 1;
    }
    return 0;
}

int extra_macro_name(const char* line) {
    int i = 4;

    while ((line[i] == ' ' || line[i] == '\t')) {
        i++;
    }
    while (line[i] != ' ' && line[i] != '\t'){
        i++;
        if (line[i] == '\0' || line[i] == '\n') {
            return 0;
        }
    }
    while ((line[i] == ' ' || line[i] == '\t')) {
        i++;
    }
    if (line[i] == '\0' || line[i] == '\n') {
        return 0;
    }
    return 1;
}

void free_macros(struct macros *head) {
    struct macros *current = head;
    struct macros *next = NULL;
    while (current != NULL) {
        next = current->next;
        free_lines(current->lines);
        free(current);
        current = next;
    }
}
