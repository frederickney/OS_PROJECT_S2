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


int send_request(int sd, data *data_struct) {
	int i = 0;
	ssize_t bwrite = 0;
	switch (data_struct->request->type) {
		case CMD_WRITE:
			if (0 > (bwrite = write(sd, (void *) data_struct->request, sizeof(cmd_data))))
				i = -1;
			else if(bwrite < sizeof(cmd_data))
				i = -2;
			else if (0 > (bwrite = write(sd, (void *) data_struct->payload, strlen(data_struct->payload))))
				i = -3;
			else if (bwrite < data_struct->request->length)
				i = -4;
			else
	            i = 0;
	        break;
		default:
			if (0 > (bwrite = write(sd, (void *) data_struct->request, sizeof(cmd_data))))
				i = -1;
			else if(bwrite < sizeof(cmd_data))
				i = -2;
			else
	            i = 0;
	        break;
	}
	return i;
}

int set_request(int sd, char *cmd, data *data_struct) {
	char *payload;
	int i = 0;
	if (0 == strcmp(cmd, READ)) {
		data_struct->request->type = CMD_READ;
		data_struct->request->offset = SIZE;
		data_struct->request->length = SIZE;
		data_struct->request->header = REQUEST;
	}
	else if(0 ==  strcmp(cmd, WRITE)){
		payload = malloc(sizeof(char) * strlen(TEST));
		payload = TEST;
		data_struct->payload = payload;
		data_struct->request->header = REQUEST;
		data_struct->request->type = CMD_WRITE;
		data_struct->request->offset = SIZE;
		data_struct->request->length = strlen(TEST);
	}
	else if(0 == strcmp(cmd, STREXIT)) {
		data_struct->request->header = REQUEST;
		data_struct->request->type = EXIT;
		data_struct->request->offset = 0;
		data_struct->request->length = 0;
	}
	else
		return 1;
	i = send_request(sd, data_struct);
	if (data_struct->request->type == CMD_WRITE) {
		data_struct->payload = NULL;
	}
	return i;
}