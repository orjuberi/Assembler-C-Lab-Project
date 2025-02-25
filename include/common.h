#ifndef COMMON_H
#define COMMON_H

/*
 * @file common.h
 * General constants and macros used throughout the project.
 */

/* 
 * Maximum length of a label in the assembly code.
 * Labels in assembly can be up to 31 characters long.
 */
#define MAX_LABEL_LEN 31

/* 
 * Keyword indicating the start of a macro definition.
 * Used to detect the beginning of macro definitions in the assembly code.
 */
#define MACRO_START "macr"

/* 
 * Keyword indicating the end of a macro definition.
 * Used to mark the end of macro definitions in the assembly code.
 */
#define MACRO_END "endmacr"

/* 
 * Maximum length of a file name.
 * Limits the length of file names to 100 characters.
 */
#define MAX_FILE_NAME 100

/* 
 * Maximum number of memory words.
 * Defines the maximum number of memory words available in the assembler.
 */
#define MAX_MEMORY_WORDS 4096

/* 
 * Directive for defining data in assembly.
 * Used to declare a data section in the assembly code.
 */
#define DATA_DIRECTIVE ".data"

/* 
 * Directive for defining a string in assembly.
 * Used to declare a string section in the assembly code.
 */
#define STRING_DIRECTIVE ".string"

/* 
 * Directive for defining an external symbol.
 * Used to declare an external symbol in the assembly code.
 */
#define EXTERN_DIRECTIVE ".extern"

/* 
 * Directive for defining an entry point.
 * Used to declare an entry point in the assembly code.
 */
#define ENTRY_DIRECTIVE ".entry"

/* 
 * Length of binary code lines.
 * Defines the length of each binary line in the assembler output.
 */
#define BINARY_CODE_LEN 15

/* 
 * Size of the opcode array.
 * Defines the number of opcodes supported by the assembler.
 */
#define OP_CODE_ARRAY_SIZE 16

/* 
 * Maximum length of a line in the source file.
 * Limits the length of each line in the assembly source code.
 */
#define MAX_LINE_LENGTH 81

#endif /* COMMON_H */

