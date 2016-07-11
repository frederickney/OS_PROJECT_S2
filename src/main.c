/**
 * @project OS PROJECT S2
 * @file main.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 03/29/2016 14:35
 * @revision 1.0.1
 */

#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
#include "reply.h"
#include "file_opener.h"
#include "cmd_interpreter.h"
#include "stdin_filter.h"

// defining ip error message
#define IPERROR "NO IP ADDRESS FOUND\n"

// defining successful connection to the server message
#define SUCCESS "HOST CONNECTED\n"

// defining disconnection message
#define EXITING "HOST DISCONNECTED\n"

// defining connection message
#define CONNECTED "HOST CONNECTED\n"

#define FORKERROR "*** ERROR: forking child process failed\n"

#define MAINFORKERROR "*** ERROR: unable to create the server\n"

#define KILL_SERVER "kill\0"

#define EXIT_SERVER "exit\0"

#define RESTART_SERVER "restart\0"

#define SAFE_EXIT ":\n\t\t\tWill stop the server safety\n"

#define FORCE_EXIT ":\n\t\t\tWill force the server to stop\n"

#define RESTART ":\n\t\t\tRestart the server\n"

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

int genfile(char *filename, char *workfile) {
	pid_t  pid;
	pid = fork();
	int status;
	if (pid < 0) {
		write(STDERR_FILENO, FORKERROR, strlen(FORKERROR));
	}
	else if (pid == 0) {
		char *IF, *OF, *bs;
		bs = "bs=1024";
		printf("%s\n", bs);
		IF = malloc(sizeof(char) * 1027);
		OF = malloc(sizeof(char) * 1027);
		IF = strcpy(IF, "if=");
		OF = strcpy(OF, "of=");
		IF = strcat(IF, filename);
		OF = strcat(OF, workfile);
		printf("%s\n", IF);
		printf("%s\n", OF);
		char **argv;
		argv = malloc(sizeof(char *) * 4);
		for (int i = 0; i < 4; i++) {
			argv[i] = malloc(sizeof(char) * 1024);
		}
		argv[0] = strcpy(argv[0], "/bin/dd");
		argv[1] = strcpy(argv[1], IF);
		argv[2] = strcpy(argv[2], OF);
		argv[3] = strcpy(argv[3], bs);
		if (0 > execvp("/bin/dd", argv))
			return EXIT_FAILURE;
		else
			return EXIT_SUCCESS;
	}
	else {
		while (wait(&status) != pid);
		return 0;
	}
	return -1;
}

void display_cmd_usage() {
	write(STDIN_FILENO, EXIT_SERVER, strlen(EXIT_SERVER));
	write(STDIN_FILENO, SAFE_EXIT, strlen(SAFE_EXIT));
	write(STDIN_FILENO, KILL_SERVER, strlen(KILL_SERVER));
	write(STDIN_FILENO, FORCE_EXIT, strlen(FORCE_EXIT));
	write(STDIN_FILENO, RESTART_SERVER, strlen(RESTART_SERVER));
	write(STDIN_FILENO, RESTART, strlen(RESTART));
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
int main_server(char **argv, char *workfile) {
	signal(SIGPIPE, SIG_IGN);
	struct sockaddr_in serversock;
	cmd_data *request_addr;
	socklen_t *addr;
	data *data_struct;
	int sd, fd, connectd;
		if (0 > genfile(argv[1], workfile))
			return EXIT_FAILURE;
		fd = open_file(workfile);
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
				if (10 != data_struct->error) {
					reply(connectd, data_struct, request_addr);
				}
				else if(10 == data_struct->error) {
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
			free(addr);
			addr = NULL;
			close(connectd);
		}
		close(sd);
		unlink(workfile);
		return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
	pid_t pid;
	char buffer[SIZE], *fbuffer;
	if (argc != 2) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	else {
		char *workfile = malloc(sizeof(char) * SIZE);
		workfile = strcpy(workfile, argv[1]);
		workfile = strcat(workfile, ".genserver");
		pid = fork();
		if (0 > pid) {
			write(STDIN_FILENO, MAINFORKERROR, strlen(MAINFORKERROR));
			return EXIT_FAILURE;
		}
		else if (0 < pid) {
			while (true) {
				if (0 > read(STDIN_FILENO, buffer, SIZE)) {
				}
				else {
					fbuffer = read_stdin(buffer);
					if (0 == (strcmp(fbuffer, EXIT_SERVER))) {
						kill(pid, SIGKILL);
						unlink(workfile);
						return EXIT_SUCCESS;
					}
					else if (0 == strcmp(fbuffer, KILL_SERVER)) {
						kill(pid, SIGTERM);
						unlink(workfile);
						return EXIT_SUCCESS;
					}
					else if (0 == strcmp(fbuffer, RESTART_SERVER)) {
						kill(pid, SIGTERM);
						pid = fork();
						if (0 == pid) {
							return main_server(argv, workfile);
						}
					}
					else {
						display_cmd_usage();
					}
				}
			}
		}
		else {
			return main_server(argv, workfile);
		}
		return EXIT_SUCCESS;
	}
}