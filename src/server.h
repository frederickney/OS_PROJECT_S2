/**
 * @project OS PROJECT S2
 * @file server.h
 * @author Frederick NEY et Stephane OVERLEN
 * @date 19/07/2016 14:59
 * @revision 1.0.1
 */

#ifndef OS_PROJECT_S2_SERVER_H
#define OS_PROJECT_S2_SERVER_H


// defining ip error message
#define IP_ERROR "NO IP ADDRESS FOUND\n"

// defining disconnection message
#define EXITING "HOST DISCONNECTED\n"

// defining connection message
#define CONNECTED "HOST CONNECTED\n"

#define STARTING "Starting the server...\t\t\t\t"

#define RESTARTING "Restarting the server...\t\t\t"

#define ALREADY_STOPPED "Server already stopped\n"


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
int main_server(char **argv, char *workfile, int start);

#endif //OS_PROJECT_S2_SERVER_H
