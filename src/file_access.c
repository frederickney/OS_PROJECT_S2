/**
 * @project OS PROJECT S2
 * @file file_access.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 02/06/2016 18:06
 * @revision 1.0.1
 */

#include "socket.h"
#include "file_access.h"

/**
 * reset offset to the beginning of the file and set the offset to specific location
 *
 *
 * @param int fd: file descriptor
 * @param int offset: it's at this offset that we will start writing or reading the file
 */
void set_offset(int fd, int offset) {
	lseek(fd, 0, SEEK_SET);
	lseek(fd, offset, SEEK_SET);
}

/**
 * read file function
 *
 *
 * @param int fd: file descriptor.
 * @param data *data_struct: structure containing payload, error, request and reply.
 * @return int: error code.
 */
uint32_t read_file(int fd, data *data_struct) {
	int bread = 0;
	set_offset(fd, data_struct->request->offset);
	if(0 > (bread = read(fd, data_struct->payload, data_struct->request->length))) {
		return 3;
	}
	else {
		if(bread != data_struct->request->length) {
			return 2;
		}
		else {
			return 0;
		}
	}
}

/**
 * write file function
 *
 *
 * @param int fd: file descriptor.
 * @param data *data_struct: structure containing payload, error, request and reply.
 * @return int: error code.
 */
uint32_t write_file(int fd, data *data_struct) {
	int bwrite = 0;
	set_offset(fd, data_struct->request->offset);
	if(0 > (bwrite = write(fd, data_struct->payload, data_struct->request->length))) {
		return 3;
	}
	else {
		if(bwrite != data_struct->request->length) {
			return 2;
		}
		else {
			return 0;
		}
	}
}
