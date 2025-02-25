#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/*
 * @file symbol_table.h
 * Definitions and functions for managing the symbol table in the assembler.
 */

/*
 * @enum SymbolType
 * Represents the different types of symbols in the assembler.
 */
typedef enum {
    SYMBOL_LABEL,     /* A regular label symbol */
    SYMBOL_EXTERN,    /* An external symbol */
    SYMBOL_ENTRY,     /* An entry symbol */
    SYMBOL_DATA,      /* A symbol for data definition */
    SYMBOL_STRING     /* A symbol for string definition */
} SymbolType;

/*
 * @struct Symbol
 * Represents a symbol in the assembler's symbol table.
 */
typedef struct Symbol {
    char *name;              /* The name of the symbol */
    int address;             /* The address of the symbol in memory */
    SymbolType type;         /* The type of the symbol (label, extern, etc.) */
    int is_data_line;        /* Flag indicating if it's a data line */
    int line;                /* The line number where the symbol is defined */
    char *data;              /* Data associated with DATA and STRING symbols */
    struct Symbol *next;     /* Pointer to the next symbol in the table */
} Symbol;

/*
 * @struct SymbolTable
 * Represents the symbol table as a linked list of symbols.
 */
typedef struct SymbolTable {
    Symbol *head;            /* Pointer to the first symbol in the table */
    Symbol *last;            /* Pointer to the last symbol in the table */
} SymbolTable;

/*
 * @struct ExternReference
 * Represents a reference to an external symbol in the assembler.
 */
typedef struct ExternReference {
    char *name;              /* The name of the external symbol */
    int address;             /* The address where the external symbol is used */
    struct ExternReference *next; /* Pointer to the next external reference */
} ExternReference;

/* External reference to the list of external symbols */
extern ExternReference* extern_references;

/*
 * Adds a new external symbol reference to the list.
 * name - The name of the external symbol.
 * address - The address where the external symbol is referenced.
 */
void add_extern_reference(const char *name, int address);

/*
 * Retrieves the list of external symbol references.
 * Returns a pointer to the head of the external reference list.
 */
const ExternReference *get_extern_references(void);

/*
 * Counts the number of data symbols in the symbol table.
 * symbol_table - Pointer to the symbol table.
 * Returns the count of data symbols.
 */
int count_data_symbols(const Symbol *symbol_table);

/*
 * Adds a new symbol to the symbol table.
 * symbol_table - Pointer to the symbol table.
 * name - The name of the symbol.
 * address - The memory address of the symbol.
 * type - The type of the symbol (label, extern, etc.).
 * is_data_line - Flag indicating if it's a data line.
 * line - The line number where the symbol is defined.
 * Returns 1 on success, 0 on failure.
 */
int add_symbol(Symbol **symbol_table, const char *name, int address, SymbolType type, int is_data_line, int line);

/*
 * Finds a symbol by its name in the symbol table.
 * name - The name of the symbol to find.
 * symbol_table - Pointer to the symbol table.
 * Returns a pointer to the symbol if found, or NULL if not.
 */
Symbol *find_symbol(const char *name, Symbol *symbol_table);

/*
 * Frees the memory used by the symbol table.
 * symbol_table - Pointer to the symbol table to be freed.
 */
void free_symbol_table(Symbol *symbol_table);

/*
 * Prints the contents of the symbol table.
 * symbol_table - Pointer to the symbol table to print.
 */
void print_symbol_table(Symbol *symbol_table);

/*
 * Updates the addresses of data symbols after the first pass.
 * symbol_table - Pointer to the symbol table.
 * ic - The instruction counter value after the first pass.
 */
void update_data_symbols(Symbol *symbol_table, int ic);

#endif /* SYMBOL_TABLE_H */

