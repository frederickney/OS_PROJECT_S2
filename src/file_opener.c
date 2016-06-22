/**
 * @project OS PROJECT S2
 * @file file_opener.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 02/06/2016 17:24
 * @revision 1.0.1
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "file_opener.h"

/**
 * file opener function
 *
 * @param char *path:
 * return int: file descriptor
 */
int open_file(char *path) {
	//TODO OPEN_FILE
	int fd;
	if(0 > (fd= open(path, O_RDWR))) {
		perror("OPEN");
		exit(EXIT_FAILURE);
	}
	return fd;
}
