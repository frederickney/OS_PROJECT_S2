#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "cmd_interpreter.h"
#include "stdin_filter.h"
#include "request.h"
#include "get_reply.h"
#include <stdbool.h>

#define SUCCESS "Successfull connection to the remote server\n"

#define IP "192.168.160.52"

#define PORT "6600"

#define CMDERROR "COMMAND NOT FOUND: "

#define SENDSTRUCTERROR "COULD NOT SEND STRUCTURE TO SERVER\n"

#define STRUCTNETWORKFAILURE "STRUCTURE NOT FULLY WRITEN\n"

#define SENDPAYLOADERROR "COULD NOT SEND PAYLOAD TO SERVER\n"

#define STRNETWORKFAILURE "PAYLOAD NOT FULLY WRITEN\n"

#define REPLYERROR "REPLY NOT RECEIVED\n"

#define REPLYNETWORKFAILURE "STRUCTURE NOT FULLY RECEIVED\n"

#define STRRDNETFAILURE "PAYLOAD NOT FULLY RECEIVED\n"

#define PAYLOADERROR "PAYLOAD NOT RECEIVED\n"

void io_loop(int sd, data *data_struct) {
    ssize_t nb_lus;
	char buffer[SIZE], *cmd;
	int unsigned i = 0;

    while(true) {
	    data_struct->request = malloc(sizeof(cmd_data));
	    data_struct->reply = malloc(sizeof(message));
	    data_struct->request->handle = i;
        if(0 > (nb_lus = read(STDIN_FILENO, buffer, SIZE))) {
            perror("read");
        }
		else {
			cmd = read_stdin(buffer);
			switch (set_request(sd, cmd, data_struct)) {
				case 1:
					write(STDIN_FILENO, CMDERROR, strlen(CMDERROR));
					write(STDIN_FILENO, cmd, strlen(cmd));
					write(STDIN_FILENO, "\n", 1);
					break;
				case -1:
					write(STDIN_FILENO, SENDSTRUCTERROR, strlen(SENDSTRUCTERROR));
					break;
				case -2:
					write(STDIN_FILENO, STRUCTNETWORKFAILURE, strlen(STRUCTNETWORKFAILURE));
					break;
				case -3:
					write(STDIN_FILENO, SENDPAYLOADERROR, strlen(SENDPAYLOADERROR));
					break;
				case -4:
					write(STDIN_FILENO, STRNETWORKFAILURE, strlen(STRNETWORKFAILURE));
					break;
				default:
					switch (get_reply(sd, data_struct)) {
						case 1:
							sleep(1);
							exit(EXIT_SUCCESS);
						case -1:
							write(STDIN_FILENO, REPLYERROR , strlen(REPLYERROR));
							break;
						case -2:
							write(STDIN_FILENO, REPLYNETWORKFAILURE, strlen(REPLYNETWORKFAILURE));
							break;
						case -3:
							write(STDIN_FILENO, PAYLOADERROR, strlen(PAYLOADERROR));
							break;
						case -4:
							write(STDIN_FILENO, STRRDNETFAILURE, strlen(STRRDNETFAILURE));
							break;
						default:
							//printf("HOST: %i\n", data_struct->request->handle);
							//printf("SERVER: %i\n", data_struct->reply->handle);
							//printf("SERVER ERROR: %i\n", data_struct->reply->error);
							if (data_struct->request->type == CMD_READ) {
								write(STDIN_FILENO, data_struct->payload, data_struct->request->length);
								write(STDIN_FILENO, "\n", 1);
							}
					}
			}
			free(data_struct->request);
			free(data_struct->reply);
			data_struct->request = NULL;
			data_struct->reply = NULL;
			i++;
        }
    }
}

int main(int argc, char *argv[])
{
    int fdsock;
    struct sockaddr_in remote;
	data *data_struct;
	remote.sin_family = AF_INET;
	remote.sin_port = htons(atoi(PORT));
	remote.sin_addr.s_addr = inet_addr(IP);
    if (0 > (fdsock = socket(AF_INET, SOCK_STREAM, 0)))
        perror("socket");
    if(0 > connect(fdsock, (const struct sockaddr *) &remote, sizeof(struct sockaddr_in))) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
	else {
	    write(STDIN_FILENO, SUCCESS, strlen(SUCCESS));
    }
    data_struct = malloc(sizeof(data));
    io_loop(fdsock, data_struct);
	free(data_struct);
	data_struct = NULL;
	close(fdsock);
}
