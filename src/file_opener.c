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
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "file_opener.h"
#include "file_access.h"

/**
 * file opener function
 *
 * @param char *path:
 * return int: file descriptor
 */
int open_file(char *path) {
	int fd;
	if(0 > (fd = open(path, O_RDWR))) {
		write(STDIN_FILENO, FAIL, strlen(FAIL));
		perror("OPEN");
		return -1;
	}
	return fd;
}

int file_exist(char *pathname) {
	if(0 <= access( pathname, F_OK)) {
		return 1;
	}
	else {
		return 0;
	}
}

int create_file(char *pathname) {
	int fd;
	struct stat buffer;
	if( 0 > (fd = open(pathname, O_CREAT))) {
		perror("CREATE");
		exit(EXIT_FAILURE);
	}
	else {
		if (0 == chmod(pathname, S_IRWXU)) {
			if (0 == stat(pathname, &buffer)){
				close(fd);
				return 0;
			}
			else {
				close(fd);
				return -1;
			}
		}
		else {
			close(fd);
			return  -2;
		}
	}
}
