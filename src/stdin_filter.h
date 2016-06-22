/*
 * @project OS PROJECT S2
 * @file stdin_filter.h
 * @author Frederick NEY et Stephane OVERLEN
 * @date 03/29/2016 14:38
 */

#ifndef OS_PROJECT_S2_STDIN_FILTER_H
#define OS_PROJECT_S2_STDIN_FILTER_H

#include <stdlib.h>
#include <string.h>

#define SIZE 1024

/**
 * @function read_stdin
 * filtering input text from STDIN_FILENO
 *
 * @param char *buffer
 * buffer read from read calling system
 *
 * @return char *
 */
char *read_stdin(char *buffer);

/**
 * @function copy_string
 * function used for copying string dynamically
 *
 * @param char *string
 * string to be copied.
 *
 * @return char *
 */
char *copy_string(char *string);

/**
 * @function string_unalloc
 * function used for un-allocating string after interpretation of the command line parsed in shell
 *
 * @param char *string
 * string to be un-allocated
 */
void string_unalloc(char *string);

#endif //OS_PROJECT_S2_STDIN_FILTER_H
