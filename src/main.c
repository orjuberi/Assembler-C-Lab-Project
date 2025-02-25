#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "second_pass.h"
#include "symbol_table.h"
#include "first_pass.h"
#include "pre_assembler.h"
#include "utils.h"
#include "error_handling.h"

int main(int argc, char* argv[]) {
    const char* input_filename;
    char* base_filename;
    struct macros* macro_head = NULL;
    int status;
    char* dot;
    FirstPassResult first_pass_result;
    
    /* Ensure the correct number of arguments are provided */
    if (argc < 2) {
        printf("Usage: %s <assembly_file>\n", argv[0]);
        return 1;
    }

    input_filename = argv[1];

    /* Check file extension for ".as" */
    dot = strrchr(input_filename, '.');
    if (dot && (strcmp(dot + 1, "as") != 0)) {
        printf("Error: Wrong File Extension\n");
        return 1;
    }

    /* Remove the file extension for further processing */
    base_filename = remove_extension(input_filename);

    /* Pre-assembler stage */
    status = pre_process(input_filename, macro_head);
    if (status != NO_ERROR) {
        printf("Error in pre-assembler stage\n");
        free(base_filename);
        return status;
    }

    /* First pass */
    first_pass_result = first_pass(base_filename);
    if (first_pass_result.errorFlag != NO_ERROR) {
        printf("Error in first pass\n");
        free(base_filename);
        return first_pass_result.errorFlag;
    }

    /* Second pass */
    status = second_pass(base_filename, first_pass_result.symbolTable);
    if (status != NO_ERROR) {
        printf("Error in second pass\n");
        free(base_filename);
        free_symbol_table(first_pass_result.symbolTable);
        return status;
    }

    /* Free allocated resources */
    free(base_filename);
    free_symbol_table(first_pass_result.symbolTable);
    
    return 0;
}

