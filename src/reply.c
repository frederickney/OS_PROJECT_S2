/**
 * @project OS PROJECT S2
 * @file reply.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 02/06/2016 15:02
 * @revision 1.0.1
 */

#include "reply.h"
#include "cmd_interpreter.h"
#include "socket.h"

// defining the error log of the server
#define ERROR "Error occured while replying to host\n"

/**
 * reply to host
 *
 *
 * @param int sd: socket descriptor.
 * @param data *data_struct: structure containing payload, error, request and reply.
 * @param cmd_data *request_addr: testing if successful writing or reading on error request_addr is NULL.
 * @return int: error code on send the message
 */
int reply(int sd, data *data_struct, cmd_data *request_addr) {
	int bwrite;
	if (data_struct->request != request_addr) {
		data_struct->reply->error = data_struct->error;
		data_struct->request->handle = data_struct->request->handle;
		data_struct->request->header = REPLY;
		if (0 > (bwrite = write(sd, (void *) data_struct->reply, sizeof(message)))) {
			perror("WRITE -> UNABLE TO REPLY ERROR TO HOST");
			return -1;
		}
		else {
			if (bwrite != sizeof(message)) {
				write(STDIN_FILENO, ERROR, strlen(ERROR));
				return -2;
			}
		}
	}
	else {
		data_struct->reply->header = REPLY;
		data_struct->reply->handle = data_struct->request->handle;
		data_struct->reply->error = 0;
		if (0 > (bwrite = write(sd, (void *) data_struct->reply, sizeof(message)))) {
			perror("WRITE -> UNABLE TO REPLY TO HOST");
			return -1;
		}
		else if (bwrite != sizeof(message)) {
				write(STDIN_FILENO, ERROR, strlen(ERROR));
				return -2;
		}
		if (data_struct->request->type == CMD_READ) {
			if (0 > (bwrite = write(sd, data_struct->payload, data_struct->request->length))) {
				perror("WRITE -> UNABLE TO SEND PAYLOAD TO HOST");
				return -1;
			}
			else if (bwrite != data_struct->request->length) {
				write(STDIN_FILENO, ERROR, strlen(ERROR));
				return -2;
			}
		}
	}
	return 0;
}
