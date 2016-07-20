/**
 * @project OS PROJECT S2
 * @file file_opener.h
 * @author Frederick NEY et Stephane OVERLEN
 * @date 02/06/2016 17:24
 * @revision 1.0.1
 */

#ifndef OS_PROJECT_S2_FILE_OPENER_H
#define OS_PROJECT_S2_FILE_OPENER_H


/**
 * file opener function
 *
 * @param char *path:
 * return int: file descriptor
 */
int open_file(char *path);

int file_exist(char *pathname);

int create_file(char *pathname);

#endif //OS_PROJECT_S2_FILE_OPENER_H
