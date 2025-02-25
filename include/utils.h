#ifndef UTILS_H
#define UTILS_H

/*
 * @file utils.h
 * Utility functions for file name manipulation and string handling in the assembler.
 */

/*
 * Adds a file extension to a given filename.
 * filename - The base filename (without extension).
 * extension - The extension to add (e.g., ".txt").
 * Returns a new string with the filename and extension combined.
 */
char* add_file_extension(const char *filename, const char *extension);

/*
 * Checks if a given name is a valid opcode.
 * name - The name to check.
 * Returns 1 if the name is a valid opcode, 0 otherwise.
 */
int check_if_opcode(const char *name);

/*
 * Removes the extension from a given filename.
 * filename - The filename from which to remove the extension.
 * Returns a new string with the extension removed.
 */
char* remove_extension(const char* filename);

/*
 * Replaces the extension of a given filename with a new extension.
 * filename - The original filename.
 * new_extension - The new extension to replace the old one.
 * Returns a new string with the replaced extension.
 */
char* replace_file_extension(const char* filename, const char* new_extension);

/*
 * Duplicates a string.
 * s - The string to duplicate.
 * Returns a new string which is a copy of the input string.
 */
char *my_strdup(const char *s);

#endif /* UTILS_H */

