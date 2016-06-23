/**
 * @project OS_PROJECT_S2
 * @file request.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 09/06/2016 08:56
 */
#include "request.h"
#include "cmd_interpreter.h"
#include "socket.h"

#define READ "read"

#define WRITE "write"

#define STREXIT "exit"

#define TEST "test"


uint32_t send_request(int sd, data *data_struct) {
	uint32_t i = 0;
	ssize_t bwrite = 0;
	switch (data_struct->request->type) {
		case CMD_WRITE:
			if (0 > (bwrite = write(sd, (void *) data_struct->request, sizeof(cmd_data))))
				i = 1;
			else if(bwrite < sizeof(cmd_data))
				i = 2;
			else if (0 > (bwrite = write(sd, (void *) data_struct->payload, strlen(data_struct->payload))))
				i = 3;
			else if (bwrite < data_struct->request->length)
				i = 4;
			else
	            i = 0;
	        break;
		default:
			if (0 > (bwrite = write(sd, (void *) data_struct->request, sizeof(cmd_data))))
				i = 1;
			else if(bwrite < sizeof(cmd_data))
				i = 2;
			else
	            i = 0;
	        break;
	}
	return i;
}

uint32_t set_request(int sd, char *cmd, data *data_struct) {
	char *payload;
	uint32_t i = 0;
	if (0 == strcmp(cmd, READ)) {
		data_struct->request->type = htonl(CMD_READ);
		data_struct->request->offset = htonl(SIZE);
		data_struct->request->length = htonl(SIZE);
		data_struct->request->header = htonl(REQUEST);
	}
	else if(0 ==  strcmp(cmd, WRITE)){
		payload = malloc(sizeof(char) * strlen(TEST));
		payload = TEST;
		data_struct->payload = payload;
		data_struct->request->header = htonl(REQUEST);
		data_struct->request->type = htonl(CMD_WRITE);
		data_struct->request->offset = htonl(SIZE);
		data_struct->request->length = htonl(strlen(TEST));
	}
	else if(0 == strcmp(cmd, STREXIT)) {
		data_struct->request->header = htonl(REQUEST);
		data_struct->request->type = htonl(EXIT);
		data_struct->request->offset = htonl(0);
		data_struct->request->length = htonl(0);
	}
	else
		return 1;
	i = send_request(sd, data_struct);
	if (data_struct->request->type == CMD_WRITE) {
		data_struct->payload = NULL;
	}
	return i;
}