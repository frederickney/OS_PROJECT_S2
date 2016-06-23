/**
 * @project OS PROJECT S2
 * @file cmd_interpreter.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 03/29/2016 14:35
 * @revision 1.0.1
 */

#include "get_request.h"
#include "file_access.h"
#include "cmd_interpreter.h"
#include "socket.h"

/**
 * interpreter function of the messages sent by any host connected to the server
 *
 *
 * @param int sd: socket descriptor
 * @param int fd: file descriptor
 * @param data *data_struct: structure containing payload, error, request and reply.
 * @return cmd_data *request: NULL on error
 */
cmd_data *cmd_interpreter(int sd, int fd, data *data_struct) {
	uint32_t i = 0;
	cmd_data *request_addr;
	request_addr = get_request(sd, data_struct);
	if (0 < data_struct->error) {
		return NULL;
	}
	else {
		data_struct->request = request_addr;
	}
	data_struct->payload = malloc(sizeof(char) * data_struct->request->length);
	switch (htonl(data_struct->request->type)) {
		case CMD_READ:
			i = read_file(fd, data_struct);
			break;
		case CMD_WRITE:
			data_struct->payload = get_payload(sd, data_struct);
			if (0 > data_struct->error) {
				i = data_struct->error;
				break;
			}
			i = write_file(fd, data_struct);
			break;
		case EXIT:
			data_struct->payload = malloc(sizeof(char));
			i = 10;
			break;
		default:
			i = 4;
		break;
	}
	if (0 > i) {
		data_struct->error = i;
		return NULL;
	}
	else
		return data_struct->request;
}