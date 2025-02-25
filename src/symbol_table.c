#include "symbol_table.h"
#include "error_handling.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "pre_assembler.h"
#include "utils.h"

ExternReference* extern_references = NULL;

void add_extern_reference(const char *name, int address) {
    ExternReference *new_ref = malloc(sizeof(ExternReference));
    if (new_ref == NULL) {
        report_error(ERR_MEMORY_ALLOCATION, 0);
        return;
    }
    new_ref->name = my_strdup(name);
    new_ref->address = address;
    new_ref->next = extern_references;
    extern_references = new_ref;
}

const ExternReference *get_extern_references(void) {
    return extern_references;
}

int count_data_symbols(const Symbol *symbol_table) {
    int count = 0;
    while (symbol_table) {
        if (symbol_table->is_data_line) {
            count++;
        }
        symbol_table = symbol_table->next;
    }
    return count;
}

int add_symbol(Symbol **symbol_table, const char *name, int address, SymbolType type, int is_data_line, int line) {
    Symbol *existing_symbol;
    Symbol *new_symbol;

    existing_symbol = find_symbol(name, *symbol_table);
    if (existing_symbol != NULL) {
        if (existing_symbol->type == SYMBOL_EXTERN && type == SYMBOL_ENTRY) {
            existing_symbol->type = SYMBOL_ENTRY;
            return 1;
        }
        report_error(ERR_SYMBOL_DUPLICATE, line);
        return 0;
    }

    

    new_symbol = malloc(sizeof(Symbol));
    if (new_symbol == NULL) {
        report_error(ERR_MEMORY_ALLOCATION, 0);
        return 0;
    }
    new_symbol->name = my_strdup(name);
    new_symbol->address = address;
    new_symbol->type = type;
    new_symbol->is_data_line = is_data_line;
    new_symbol->line = line;
    new_symbol->next = *symbol_table;
    *symbol_table = new_symbol;

    return 1;
}

Symbol *find_symbol(const char *name, Symbol *symbol_table) {
    char cleaned_symbol[MAX_LABEL_LEN + 1];
    Symbol *current;
    Symbol *found = NULL;
    char cleaned_current[MAX_LABEL_LEN + 1];

    strncpy(cleaned_symbol, name, MAX_LABEL_LEN);
    cleaned_symbol[MAX_LABEL_LEN] = '\0';
    trim_whitespace(cleaned_symbol);

    current = symbol_table;
    while (current != NULL) {
        strncpy(cleaned_current, current->name, MAX_LABEL_LEN);
        cleaned_current[MAX_LABEL_LEN] = '\0';
        trim_whitespace(cleaned_current);
        if (strcmp(cleaned_current, cleaned_symbol) == 0) {
            found = current;
            break;
        }
        current = current->next;
    }

    if (found) {
        return found;
    } else {
        return NULL;
    }
}

void free_symbol_table(Symbol *symbol_table) {
    Symbol *current = symbol_table;
    while (current != NULL) {
        Symbol *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}

void print_symbol_table(Symbol *symbol_table) {
    Symbol *current = symbol_table;
    printf("Full Symbol Table:\n");
    while (current != NULL) {
        printf("  Name: %s, Address: %d, Type: %d, Is Data: %d, Line: %d\n",
               current->name, current->address, current->type, current->is_data_line, current->line);
        current = current->next;
    }
    printf("End of Symbol Table\n");
}
