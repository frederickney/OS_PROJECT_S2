/**
 * @project OS PROJECT S2
 * @file reply.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 02/06/2016 15:02
 * @revision 1.0.1
 */

#include <time.h>
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
uint32_t reply(int sd, data *data_struct, cmd_data *request_addr) {
	int bwrite;
	if (data_struct->request != request_addr) {
		data_struct->reply->error = htonl(data_struct->error);
		data_struct->reply->handle = htonl(data_struct->request->handle);
		data_struct->reply->header = htonl(REPLY);
		if (0 > (bwrite = write(sd, (void *) data_struct->reply, sizeof(message)))) {
			perror("WRITE -> UNABLE TO REPLY ERROR TO HOST");
			return 1;
		}
		else {
			if (bwrite != sizeof(message)) {
				write(STDIN_FILENO, ERROR, strlen(ERROR));
				return 2;
			}
		}
	}
	else {
		data_struct->reply->header = htonl(REPLY);
		data_struct->reply->handle = htonl(data_struct->request->handle);
		data_struct->reply->error = htonl(0);
		if (0 > (bwrite = write(sd, (void *) data_struct->reply, sizeof(message)))) {
			perror("WRITE -> UNABLE TO REPLY TO HOST");
			return 1;
		}
		else if (bwrite != sizeof(message)) {
				write(STDIN_FILENO, ERROR, strlen(ERROR));
				return 2;
		}
		if (data_struct->request->type == ntohl(CMD_READ)) {
				return send_payload(sd, data_struct);
		}
	}
	return 0;
}

uint32_t send_payload(int sd, data *data_struct) {
	int bwrite, totalbwrite, loop, i;
	char *buffer;
	loop = 3600;
	i = totalbwrite = 0;
	buffer = malloc(sizeof (char) * data_struct->request->length);
	buffer = strcpy(buffer, data_struct->payload);
	while( i < loop && totalbwrite != data_struct->request->length) {
		if (0 > (bwrite = write(sd, buffer, data_struct->request->length))) {
			perror("WRITE -> UNABLE TO SEND PAYLOAD TO HOST");
		}
		else if (bwrite != data_struct->request->length) {
			totalbwrite += bwrite;
			printf("total = %i\n", totalbwrite);
			free(buffer);
			printf("%i\n", bwrite);
			buffer = malloc(sizeof(char) * (data_struct->request->length - totalbwrite));
			for (int j = 0; j < data_struct->request->length - totalbwrite; j++) {
				buffer[j] = data_struct->payload[totalbwrite + j];
			}
		}
		else {
			totalbwrite = data_struct->request->length;
			printf("WRITE :%i\n", bwrite);
			printf("%i\n", bwrite);
			return 0;
		}
		if(bwrite == 0) {
			printf("%i\n", i);
			i++;
		}
		else if (0 > bwrite) {
			return 10;
		}
	}
	if (i == loop) {
		return 2;
	}
	else {
		return 0;
	}
}