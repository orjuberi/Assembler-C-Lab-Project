#include "pre_assembler.h"
#include "common.h"
#include "macro.h"
#include "error_handling.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int starts_with(const char* line, const char* prefix) {
    while (*prefix) {
        if (*line != *prefix) {
            return 0;
        }
        line++;
        prefix++;
    }
    return 1;
}

void trim_whitespace(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) {
        return;
    }
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

int case_insensitive_compare(const char *str1, const char *str2) {
    while (*str1 && *str2) {
        if (tolower((unsigned char)*str1) != tolower((unsigned char)*str2)) {
            return 0;
        }
        str1++;
        str2++;
    }
    return *str1 == *str2;
}

int clean_file(const char *input_filename, const char *output_filename) {
    FILE *input_file = fopen(input_filename, "r");
    FILE *output_file;
    char line[MAX_LINE_LENGTH];

    if (input_file == NULL) {
        printf("Error: Could not open input file %s\n", input_filename);
        return ERR_FILE_ACCESS;
    }

    output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        printf("Error: Could not create output file %s\n", output_filename);
        fclose(input_file);
        return ERR_FILE_ACCESS;
    }

    while (fgets(line, sizeof(line), input_file)) {
        int i = 0;
        while (line[i] == ' ' || line[i] == '\t') {
            i++;
        }
        if (line[i] != ';' && line[i] != '\n' && strlen(line) > 2) {
            fputs(line+i, output_file);
        }
    }

    fclose(input_file);
    fclose(output_file);
    return NO_ERROR;
}

void strip_extra_spaces(char *destination, const char *source) {
    const char *src = source;
    char *dst = destination;
    while (*src != '\0') {
        if (!isspace((unsigned char)*src) || (dst > destination && !isspace((unsigned char)*(dst - 1)))) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

int handle_macros(const char *output_filename, const char *input_filename, struct macros *macro_head) {
    FILE *input_file = fopen(input_filename, "r");
    FILE *output_file;
    char line[MAX_LINE_LENGTH];

    if (input_file == NULL) {
        printf("Error: Could not open input file %s\n", input_filename);
        return ERR_FILE_ACCESS;
    }

    output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        printf("Error: Could not create output file %s\n", output_filename);
        fclose(input_file);
        return ERR_FILE_ACCESS;
    }

    while (fgets(line, sizeof(line), input_file)) {
        
        if (starts_with(line, MACRO_START)) {
            if (verify_macro_name(line, macro_head)) {
                return 1;
            }
            if (insert_macro(input_file, output_file, &macro_head, line)) {
                return 1;
            }
        } else if (starts_with(line, MACRO_END)) {
            printf("Error: endmacr without undefine macr.\n");
            return 1;
        }
        else {
            struct macros *macro = is_existing_macro(macro_head, line);
            if (macro != NULL) {
                expand_macro(macro, output_file);
            } else {
                fputs(line, output_file);
            }
        }
    }

    fclose(input_file);
    fclose(output_file);
    return NO_ERROR;
}

int verify_macro_name(const char *line, struct macros *macro_head) {
    char macro_name[MAX_LINE_LENGTH];
    sscanf(line, "macr %s", macro_name);
    trim_whitespace(macro_name);

    if (no_macro_name(line)) {
        printf("Error: Macro name missing.\n");
        return 1;
    }
    if (extra_macro_name(line)) {
        printf("Error: Additional character in the macro definition line.\n");
        return 1;
    }
    if ((is_existing_macro(macro_head, macro_name))) {
        printf("Error: Macro name already exists.\n");
        return 1;
    }
    if (check_if_opcode(macro_name)) {
        printf("Error: Macro name restricted.\n");
        return 1;
    }
    return NO_ERROR;
}

int insert_macro(FILE *input_file, FILE *output_file, struct macros **macro_head, const char *macro_definition) {
    char macro_name[MAX_LINE_LENGTH];
    struct macros *new_macro;
    char line[MAX_LINE_LENGTH];

    sscanf(macro_definition, "macr %s", macro_name);
    trim_whitespace(macro_name);

    new_macro = create_macro_node(macro_name, macro_head);
    
    fgets(line, sizeof(line), input_file);

    while (!(starts_with(line, MACRO_END))) {

        struct lines *new_line = malloc(sizeof(struct lines));
        if (new_line == NULL) {
            report_error(ERR_MEMORY_ALLOCATION, 0);
            return 0;
        }
        
        strcpy(new_line->line, line);
        new_line->next = NULL;    

        if (new_macro->lines == NULL) {
            /*If the list is empty, make new_line the first node*/
            new_macro->lines = new_line;
        }
        else {
            /*Traverse to the end of the list*/
            struct lines* current = new_macro->lines;
            while (current->next != NULL) {
                current = current->next;
            }
            /*Append new_line at the end*/
            current->next = new_line;
        }

        if (!(fgets(line, sizeof(line), input_file))) {
            printf("Error: Macro without end.\n");
            return 1;
        }
    }
    return 0;
}

void expand_macro(const struct macros *macro, FILE *output_file) {
    struct lines *current_line = macro->lines;
    while (current_line != NULL) {
        fputs(current_line->line, output_file);
        if (current_line->line[strlen(current_line->line) - 1] != '\n') {
            fputc('\n', output_file);
        }
        current_line = current_line->next;
    }
}

int pre_process(const char *filename, struct macros *macro_head) {
    char *as_filename = replace_file_extension(filename, ".as");
    char *tmp_filename = replace_file_extension(filename, ".tmp");
    char *am_filename = replace_file_extension(filename, ".am");
    int status = NO_ERROR;

    if (as_filename == NULL || tmp_filename == NULL || am_filename == NULL) {
        printf("Error: Memory allocation failed in pre_process\n");
        status = ERR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    if (clean_file(as_filename, tmp_filename) != NO_ERROR) {
        printf("Error: Failed to clean file %s\n", as_filename);
        status = ERR_FILE_ACCESS;
        goto cleanup;
    }

    if (handle_macros(am_filename, tmp_filename, macro_head) != NO_ERROR) {
        printf("Error: Failed to handle macros in file %s\n", tmp_filename);
        status = ERR_FILE_ACCESS;
        goto cleanup;
    }

cleanup:
    free(as_filename);
    free(tmp_filename);
    free(am_filename);
    return status;
}
