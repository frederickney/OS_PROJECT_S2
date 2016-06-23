/**
 * @project OS PROJECT S2
 * @file get_request.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 02/06/2016 15:05
 * @revision 1.0.1
 */

#include <time.h>
#include "get_request.h"
#include "cmd_interpreter.h"
#include "socket.h"

// defining host structure error
#define HOSTERROR "Host structure error, request->type not matching\n"

// defining null data received error
#define NULLDATA "NULL DATA READ\n"

//defining network failure error
#define NETWORKFAILURE "something bad hapen throw the network\n"

// defining header error
#define HEADERERROR "Header error\n"

/**
 * function to retrieve data structure sent by hosts to the server
 *
 *
 * @param int sd: socket descriptor.
 * @param data *data_struct: structure containing payload, error, request and reply.
 * @return data *request: NULL on error.
 */
cmd_data *get_request(int sd, data *data_struct) {
	uint32_t bread;
	if (0 > (bread = read(sd, (void *) data_struct->request, sizeof(cmd_data)))) {
		data_struct->error = 10;
		return NULL;
	}
	else if (0 == bread) {
		data_struct->error = 10;
		return NULL;
	}
	if (data_struct->request->type != htonl(CMD_READ) && data_struct->request->type != htonl(CMD_WRITE) && data_struct->request->type != htonl(EXIT)) {
		write(STDIN_FILENO, HOSTERROR, strlen(HOSTERROR));
		data_struct->error = 4;
		return NULL;
	}
	if (data_struct->request->header != htonl(REQUEST)) {
		write(STDIN_FILENO, HEADERERROR, strlen(HEADERERROR));
		data_struct->error = 5;
		return NULL;
	}
	else {
		data_struct->request->header = ntohl(data_struct->request->header);
		data_struct->request->handle = ntohl(data_struct->request->handle);
		data_struct->request->length = ntohl(data_struct->request->length);
		data_struct->request->offset = ntohl(data_struct->request->offset);
		data_struct->request->type = ntohl(data_struct->request->type);
	}
	return data_struct->request;
}


/**
 * function to retrieve payload sent by the remote host
 *
 *
 * @param int sd: socket descriptor.
 * @param data *data_struct: structure containing payload, error, request and reply.
 * @return char *payload: NULL on error.
 */
char *get_payload(int sd, data *data_struct) {
	int bread;
	char *buffer;
	buffer = malloc(sizeof(char) * data_struct->request->length);
	if (0 > (bread = read(sd, buffer, data_struct->request->length))) {
		write(STDIN_FILENO, NULLDATA, strlen(NULLDATA));
		data_struct->error = 10;
		return NULL;
	}
	else if (bread != data_struct->request->length) {
		data_struct->error = 7;
		return NULL;
	}
	else {
		data_struct->payload = strcpy(data_struct->payload, buffer);
		return data_struct->payload;
	}
	return data_struct->payload;
}
