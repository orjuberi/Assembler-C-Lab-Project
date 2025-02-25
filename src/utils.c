#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "line_parser.h"

/* Add the given file extension to the filename */
char* add_file_extension(const char *filename, const char *extension) {
    char *new_filename;

    /* If the filename already contains the extension, return a copy of it */
    if (strstr(filename, extension) != NULL) {
        new_filename = malloc(strlen(filename) + 1);
        if (new_filename == NULL) {
            return NULL;
        }
        strcpy(new_filename, filename);
        return new_filename;
    }

    /* Allocate memory for the new filename with extension */
    new_filename = malloc(strlen(filename) + strlen(extension) + 1);
    if (new_filename == NULL) {
        return NULL;
    }

    /* Copy the filename and concatenate the extension */
    strcpy(new_filename, filename);
    strcat(new_filename, extension);

    return new_filename;
}

/* Check if the given name is an opcode */
int check_if_opcode(const char *name) {
    unsigned int i;

    /* Iterate over the opcode array and compare names */
    for (i = 0; i < OP_CODE_ARRAY_SIZE; i++) {
        if (strcmp(name, OPCODES[i].name) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Remove the extension from the given filename */
char* remove_extension(const char* filename) {
    char* result = my_strdup(filename);
    char* lastdot = strrchr(result, '.');

    /* If a dot is found, terminate the string before the dot */
    if (lastdot != NULL) {
        *lastdot = '\0';
    }

    return result;
}

/* Replace the file extension with a new one */
char* replace_file_extension(const char* filename, const char* new_extension) {
    char* result;
    char* lastdot;
    char* new_filename;

    result = my_strdup(filename);
    if (result == NULL) {
        return NULL;
    }

    /* Find the last dot and remove the extension if it exists */
    lastdot = strrchr(result, '.');
    if (lastdot != NULL) {
        *lastdot = '\0';
    }

    /* Allocate memory for the new filename with the new extension */
    new_filename = malloc(strlen(result) + strlen(new_extension) + 1);
    if (new_filename == NULL) {
        free(result);
        return NULL;
    }

    /* Copy the result and concatenate the new extension */
    strcpy(new_filename, result);
    strcat(new_filename, new_extension);

    free(result);
    return new_filename;
}

/* Duplicate the given string */
char* my_strdup(const char* str) {
    char* copy = malloc(strlen(str) + 1);

    /* Copy the string if memory allocation was successful */
    if (copy != NULL) {
        strcpy(copy, str);
    }

    return copy;
}

