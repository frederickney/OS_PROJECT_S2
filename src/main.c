/**
 * @project OS PROJECT S2
 * @file main.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 03/29/2016 14:35
 * @revision 1.0.1
 */

#include <stdbool.h>
#include <signal.h>
#include "reply.h"
#include "file_opener.h"
#include "cmd_interpreter.h"

// defining ip error message
#define IPERROR "NO IP ADDRESS FOUND\n"

// defining successful connection to the server message
#define SUCCESS "HOST CONNECTED\n"

// defining disconnection message
#define EXITING "HOST DISCONNECTED\n"

// defining connection message
#define CONNECTED "HOST CONNECTED\n"

/**
 * Display usage.
 *
 *
 * @param string pointer containing the program name
 */
void usage(char *program_name) {
	write(STDIN_FILENO, program_name, strlen(program_name));
	write(STDIN_FILENO, "  <File system image>\n", 22);
}

void freecontent(data *data_struct) {
	free(data_struct->request);
	free(data_struct->reply);
	free(data_struct->payload);
	data_struct->reply = NULL;
	data_struct->request = NULL;
	data_struct->payload = NULL;
}

/**
 * main program.
 *
 *
 * @param number of string parse int argv
 * @param string array poiter argv
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */

int main(int argc, char **argv) {
	signal(SIGPIPE, SIG_IGN);
	struct sockaddr_in serversock;
	cmd_data *request_addr;
	socklen_t *addr;
	data *data_struct;
	int sd, fd, connectd;
	if (argc != 2) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	else {
		fd = open_file(argv[1]);
		if (0 > sockgen(&serversock)) {
			write(STDIN_FILENO, IPERROR, strlen(IPERROR));
			return EXIT_FAILURE;
		}
		if (0 > (sd = socket(AF_INET, SOCK_STREAM, 0))) {
			perror("SOCKET");
			return EXIT_FAILURE;
		}
		if (0 > bind(sd, (const struct sockaddr *) &serversock, sizeof(struct sockaddr_in))) {
			perror("BIND");
			return EXIT_FAILURE;
		}
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
			write(STDIN_FILENO, CONNECTED, strlen(CONNECTED));
			while (true) {
				data_struct = malloc(sizeof(data));
				data_struct->request = malloc(sizeof(cmd_data));
				data_struct->reply = malloc(sizeof(message));
				data_struct->error = 0;
				request_addr = cmd_interpreter(connectd, fd, data_struct);
				if (-10 != data_struct->error)
					reply(connectd, data_struct, request_addr);
				else if(-10 == data_struct->error) {
					freecontent(data_struct);
					free(data_struct);
					data_struct = NULL;
					write(STDIN_FILENO, EXITING, strlen(EXITING));
					break;
				}
				freecontent(data_struct);
				free(data_struct);
				data_struct = NULL;
			}
		}
		free(addr);
		addr = NULL;
		close(connectd);
		close(sd);
		return EXIT_SUCCESS;
	}
}