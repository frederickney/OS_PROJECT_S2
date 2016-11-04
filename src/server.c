/**
 * @project OS PROJECT S2
 * @file server.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 19/07/2016 14:59
 * @revision 1.0.1
 */

#include <stdbool.h>
#include <signal.h>
#include <netinet/in.h>
#include "reply.h"
#include "server.h"
#include "file_access.h"
#include "file_opener.h"
#include "cmd_interpreter.h"

/**
 * free content of the structure data
 *
 * @param data pointer to data_struct
 */
void freecontent(data *data_struct) {
	free(data_struct->request);
	free(data_struct->reply);
	free(data_struct->payload);
	data_struct->reply = NULL;
	data_struct->request = NULL;
	data_struct->payload = NULL;
}

/**
 * main function of the server.
 *
 *
 * @param string array pointer argv
 * @param path of the working file
 * @param int start
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main_server(char *workfile, int start) {
	signal(SIGPIPE, SIG_IGN);
	sigset_t mask;
	struct sockaddr_in serversock;
	cmd_data *request_addr;
	socklen_t *addr;
	data *data_struct;
	int sd, fd, connectd;
	if (1 == start)
		write(STDIN_FILENO, STARTING, strlen(STARTING));
	else if (3 == start) {
		write(STDIN_FILENO, STARTING, strlen(STARTING));
	}
	if (0 > (fd = open_file(workfile))) {
		return EXIT_FAILURE;
	}
	if (0 > sockgen(&serversock)) {
		write(STDIN_FILENO, FAIL, strlen(FAIL));
		write(STDIN_FILENO, IP_ERROR, strlen(IP_ERROR));
		return EXIT_FAILURE;
	}
	if ((sigemptyset(&mask) == -1) || (sigaddset(&mask, SIGTERM) == -1)){
		write(STDIN_FILENO, FAIL, strlen(FAIL));
		perror("SIGNAL MASK");
		return EXIT_FAILURE;
	}
	if (0 > (sd = socket(AF_INET, SOCK_STREAM, 0))) {
		write(STDIN_FILENO, FAIL, strlen(FAIL));
		perror("SOCKET");
		return EXIT_FAILURE;
	}
	if (0 > bind(sd, (const struct sockaddr *) &serversock, sizeof(struct sockaddr_in))) {
		write(STDIN_FILENO, FAIL, strlen(FAIL));
		perror("BIND");
		return EXIT_FAILURE;
	}
	write(STDIN_FILENO, DONE, strlen(DONE));
	while (true) {
		addr = malloc(sizeof(socklen_t));
		if (0 > listen(sd, 10)) {
			perror("LISTEN");
			exit(EXIT_FAILURE);
		}
		if (0 > (connectd = accept(sd, (struct sockaddr *) &serversock, addr))) {
			perror("ACCEPT");
			return EXIT_FAILURE;
		}
		sigprocmask(SIG_BLOCK, &mask, NULL);
		write(STDIN_FILENO, CONNECTED, strlen(CONNECTED));
		while (true) {
			data_struct = malloc(sizeof(data));
			data_struct->request = malloc(sizeof(cmd_data));
			data_struct->reply = malloc(sizeof(message));
			data_struct->error = 0;
			request_addr = cmd_interpreter(connectd, fd, data_struct);
			if (10 != data_struct->error) {
				reply(connectd, data_struct, request_addr);
			}
			else if(10 == data_struct->error) {
				freecontent(data_struct);
				free(data_struct);
				data_struct = NULL;
				write(STDIN_FILENO, EXITING, strlen(EXITING));
				sigprocmask(SIG_UNBLOCK, &mask, NULL);
				break;
			}
			freecontent(data_struct);
			free(data_struct);
			data_struct = NULL;
		}
		free(addr);
		addr = NULL;
		close(connectd);
	}
	close(sd);
	return EXIT_SUCCESS;
}