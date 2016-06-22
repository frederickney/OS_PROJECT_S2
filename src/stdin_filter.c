/*
 * @project OS PROJECT S2
 * @file stdin_filter.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 03/29/2016 14:38
 * @revision 1.0.1
 */
#include <stdbool.h>
#include "socket.h"
#include "stdin_filter.h"

/**
 * @function read_stdin
 * filtering input text from STDIN_FILENO
 *
 * @param char *buffer
 * buffer read from read calling system
 *
 * @return char *
 */
char *read_stdin(char *buffer) {
	//for unloosing data reasons we used the size of the buffer for allocating the string
	//returned by the function instead of the SIZE defined on the stdin_filter.h
	int i = 0;
	int buff_size;
	char *character;
    while (true) {
		if (buffer[i] == '\n' || buffer[i] == '\0' ) {
			i++;
			buff_size = i;
			break;
		}
		i++;
	}
	character = malloc(sizeof(char) * buff_size);
	for(i = 0; i < buff_size; i++)
		if (buffer[i] != '\n')
			character[i] = buffer[i];
		else {
			character[i]= '\0';
			break;
		}
	return (character);
}

/**
 * @function copy_string
 * function used for copying string dynamically
 *
 * @param char *string
 * string to be copied.
 *
 * @return char *
 */
char *copy_string(char *string) {
	char *string_alloc = malloc(sizeof(char) * strlen(string));
	string_alloc = strcpy(string_alloc, string);
	return string_alloc;
}

/**
 * @function string_unalloc
 * function used for un-allocating string after interpretation of the command line parsed in shell
 *
 * @param char *string
 * string to be un-allocated
 */
void string_unalloc(char *string) {
	free(string);
	string = NULL;
}

