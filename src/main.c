/**
 * @project OS PROJECT S2
 * @file main.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 03/29/2016 14:35
 * @revision 1.0.1
 */


#include <signal.h>
#include <stdbool.h>
#include <sys/wait.h>
#include "reply.h"
#include "server.h"
#include "file_access.h"
#include "stdin_filter.h"

#define MAIN_FORK_ERROR "*** ERROR: unable to run the server\n"

#define KILL_SERVER "kill\0"

#define KILL_SERVER2 "-k\0"

#define EXIT2 "-q\0"

#define OR " or \0"

#define EXIT_SERVER "exit\0"

#define RESTART_SERVER "restart\0"

#define START_SERVER "start\0"

#define STOP_SERVER "stop\0"

#define STOPPING "Stopping the server...\t\t\t\t"

#define ALREADY_STARTED "Server already started\n"

#define ALREADY_KILLED "Server is killed\n"

#define KILLED "Server killed\n"

#define SAFE_EXIT ":\n\t\t\tWill exit main process and continue running the server\n"

#define FORCE_EXIT ":\n\t\t\tWill kill the server and exit the main process\n"

#define IMG ":\n\t\t\tBuild or rebuild image file used by the server\n"

#define RESTART ":\n\t\t\tRestart the server\n"

#define RESTART2 "-r\0"

#define START ":\n\t\t\tStart the server\n"

#define STOP ":\n\t\t\tStop the server without exiting main process\n"

#define STATUS "status\0"

#define SERVER_STATUS ":\n\t\t\tDisplaying server status\n"

#define ALIVE "Running...\n"

#define DEAD "Stopped...\n"

#define GEN_IMG "build image\0"

#define GEN2 "-b\0"

#define HELP "-h\0"

#define HELP2 "help\0"

#define __HELP_STR__ ":\n\t\t\tDisplaying usages\n"

#define LOG_PID_FILE "proccess_working"

#define SUCCESSFUL_PID "Getting the process id of the server...\t\t"

#define USAGE1 "Use "

#define USAGE2 " to display usages\n"

/**
 * Display usage of the program.
 *
 *
 * @param string pointer containing the program name
 */
void usage(char *program_name) {
	write(STDIN_FILENO, program_name, strlen(program_name));
	write(STDIN_FILENO, "  <File system image>\n", 22);
}

/**
 * Display usage of main process.
 */
void display_cmd_usage() {
	write(STDIN_FILENO, EXIT_SERVER, strlen(EXIT_SERVER));
	write(STDIN_FILENO, OR, strlen(OR));
	write(STDIN_FILENO, EXIT2, strlen(EXIT2));
	write(STDIN_FILENO, SAFE_EXIT, strlen(SAFE_EXIT));
	write(STDIN_FILENO, KILL_SERVER, strlen(KILL_SERVER));
	write(STDIN_FILENO, OR, strlen(OR));
	write(STDIN_FILENO, KILL_SERVER2, strlen(KILL_SERVER2));
	write(STDIN_FILENO, FORCE_EXIT, strlen(FORCE_EXIT));
	write(STDIN_FILENO, RESTART_SERVER, strlen(RESTART_SERVER));
	write(STDIN_FILENO, OR, strlen(OR));
	write(STDIN_FILENO, RESTART2, strlen(RESTART2));
	write(STDIN_FILENO, RESTART, strlen(RESTART));
	write(STDIN_FILENO, START_SERVER, strlen(START_SERVER));
	write(STDIN_FILENO, START, strlen(START));
	write(STDIN_FILENO, STOP_SERVER, strlen(STOP_SERVER));
	write(STDIN_FILENO, STOP, strlen(STOP));
	write(STDIN_FILENO, GEN_IMG, strlen(GEN_IMG));
	write(STDIN_FILENO, OR, strlen(OR));
	write(STDIN_FILENO, GEN2, strlen(GEN2));
	write(STDIN_FILENO, IMG, strlen(IMG));
	write(STDIN_FILENO, STATUS, strlen(STATUS));
	write(STDIN_FILENO, SERVER_STATUS, strlen(SERVER_STATUS));
	write(STDIN_FILENO, HELP2, strlen(HELP2));
	write(STDIN_FILENO, OR, strlen(OR));
	write(STDIN_FILENO, HELP, strlen(HELP));
	write(STDIN_FILENO, __HELP_STR__, strlen(__HELP_STR__));
}

/**
 * main function of the server.
 *
 *
 * @param number of the param parsed while executing
 * @param string array pointer
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char **argv) {
	pid_t pid = -10;
	int start = 1;
	int pid_status = 0;
	int status;
	signal(SIGINT, SIG_IGN);
	signal(SIGABRT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	char buffer[SIZE], *fbuffer;
	if (argc != 2) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	else {
		char *workfile = malloc(sizeof(char) * SIZE);
		workfile = strcpy(workfile, argv[1]);
		workfile = strcat(workfile, ".genserver");
		write(STDIN_FILENO, USAGE1, strlen(USAGE1));
		write(STDIN_FILENO, HELP2, strlen(HELP2));
		write(STDIN_FILENO, OR, strlen(OR));
		write(STDIN_FILENO, HELP, strlen(HELP));
		write(STDIN_FILENO, USAGE2, strlen(USAGE2));
		if (0 > (pid = log_pid(LOG_PID_FILE, pid))) {
			if (0 > genfile(argv[1], workfile))
				return EXIT_FAILURE;
			pid = fork();
		}
		else {
			write(STDIN_FILENO, SUCCESSFUL_PID, strlen(SUCCESSFUL_PID));
			pid_status = -1;
		}
		if (0 > pid && 0 == pid_status) {
			write(STDIN_FILENO, MAIN_FORK_ERROR, strlen(MAIN_FORK_ERROR));
			return EXIT_FAILURE;
		}
		else if (0 > pid_status) {
			if (0 > pid) {
				write(STDIN_FILENO, FAIL, strlen(FAIL));
			}
			else {
				write(STDIN_FILENO, DONE, strlen(DONE));
			}
		}
		if (0 < pid) {
			if (0 == pid_status) {
				log_pid(LOG_PID_FILE, pid);
			}
			while (true) {
				if (0 > read(STDIN_FILENO, buffer, SIZE)) {
				}
				else {
					fbuffer = read_stdin(buffer);
					if (0 == strcmp(fbuffer, EXIT_SERVER) || 0 == strcmp(fbuffer, EXIT2)) {
						if (0 < pid) {
							if (0 == log_pid(LOG_PID_FILE, pid))
								return EXIT_SUCCESS;
							else
								return EXIT_FAILURE;
						}
						return EXIT_SUCCESS;
					}
					else if (0 == strcmp(fbuffer, KILL_SERVER) || 0 == strcmp(fbuffer, KILL_SERVER2)) {
						if (0 < pid) {
							kill(pid, SIGKILL);
							if (0 == pid_status) {
								while (wait(&status) != pid);
							}
							unlink(LOG_PID_FILE);
							write(STDIN_FILENO, KILLED, strlen(KILLED));
							pid = -20;
						}
						else if (-20 == pid){
							write(STDIN_FILENO, ALREADY_KILLED, strlen(ALREADY_KILLED));
						}
						else {
							write(STDIN_FILENO, ALREADY_STOPPED, strlen(ALREADY_STOPPED));
						}
					}
					else if (0 == strcmp(fbuffer, RESTART_SERVER) || 0 == strcmp(fbuffer, RESTART2)) {
						if (0 < pid) {
							kill(pid, SIGTERM);
							if (0 == pid_status) {
								while (wait(&status) != pid);
							}
							unlink(LOG_PID_FILE);
							pid_status = 0;
							start = 2;
						}
						else {
							pid_status = 0;
							start = 3;
						}
						pid = fork();
						if (0 > pid) {
							write(STDIN_FILENO, MAIN_FORK_ERROR, strlen(MAIN_FORK_ERROR));
						}
						if (0 == pid) {
							return main_server(argv, workfile, start);
						}
						log_pid(LOG_PID_FILE, pid);
					}
					else if (0 == (strcmp(fbuffer, START_SERVER))) {
						if (0 < pid) {
							write(STDIN_FILENO, ALREADY_STARTED, strlen(ALREADY_STARTED));
						}
						else{
							start = 1;
							pid_status = 0;
							pid = fork();
							if (0 == pid) {
								return main_server(argv, workfile, start);
							}
							if (0 > pid) {
								write(STDIN_FILENO, MAIN_FORK_ERROR, strlen(MAIN_FORK_ERROR));
							}
							log_pid(LOG_PID_FILE, pid);
						}
					}
					else if (0 == strcmp(fbuffer, STOP_SERVER)) {
						if (0 < pid) {
							write(STDIN_FILENO, STOPPING, strlen(STOPPING));
							kill(pid, SIGTERM);
							if (0 == pid_status) {
								while (wait(&status) != pid);
							}
							unlink(LOG_PID_FILE);
							write(STDIN_FILENO, DONE, strlen(DONE));
							pid = -10;
						}
						else if (-20 == pid) {
							write(STDIN_FILENO, ALREADY_KILLED, strlen(ALREADY_KILLED));
						}
						else {
							write(STDIN_FILENO, ALREADY_STOPPED, strlen(ALREADY_STOPPED));
						}
					}
					else if (0 == strcmp(fbuffer, GEN_IMG) || 0 == strcmp(fbuffer, GEN2)) {
						if (0 > genfile(argv[1], workfile))
							return EXIT_FAILURE;
					}
					else if (0 == strcmp(fbuffer, STATUS)) {
						if (0 > pid) {
							write(STDIN_FILENO, DEAD, strlen(DEAD));
						}
						else {
							write(STDIN_FILENO, ALIVE, strlen(ALIVE));
						}
					}
					else if (0 == strcmp(fbuffer, HELP) || 0 == strcmp(fbuffer, HELP2)) {
						display_cmd_usage();
					}
					else if (0 == strcmp(fbuffer, "\0")) {
						//nothing to do
					}
					else {
						display_cmd_usage();
					}
				}
			}
		}
		else {
			return main_server(argv, workfile, start);
		}
		return EXIT_SUCCESS;
	}
}