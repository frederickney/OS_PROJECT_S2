/**
 * @project OS_PROJECT_S2
 * @file get _reply.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 09/06/2016 09:31
 */
#include "get_reply.h"
#include "cmd_interpreter.h"
#include "socket.h"

uint32_t get_reply(int sd, data *data_struct) {
	uint32_t i = 0;
	ssize_t bread = 0;
	switch (data_struct->request->type) {
		case CMD_READ:
			data_struct->payload = malloc(sizeof(char) * data_struct->request->length);
			if (0 > (bread = read(sd, (void *) data_struct->reply, sizeof(message))))
				i = 1;
			else if (bread < sizeof(message))
				i = 2;
			else if (0 > (bread = read(sd, (void *) data_struct->payload, data_struct->request->length)))
				i = 3;
			else if(bread < data_struct->request->length)
				i = 4;
			else
	            i = 0;
	            break;
		case CMD_WRITE:
			if (0 > (bread = read(sd, (void *) data_struct->reply, sizeof(message))))
				i = 1;
			else if (bread < sizeof(message))
				i = 2;
	        else
				i = 0;
	            break;
		case EXIT:
			i = 1;
	        break;
	}
	return  i;
}