/**
 * @project OS PROJECT S2
 * @file socket.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 05/24/2016 14:35
 * @revision 1.0.1
 */

#include "socket.h"

// defining the port number
#define PORT "6600"

/**
 * generating socket.
 *
 *
 * @param struct sockaddr_in *serversock: Structure of the socket
 * @return int: 0 on success -1 on failure
 */
int sockgen(struct sockaddr_in *serversock) {
	serversock->sin_family = AF_INET;
	serversock->sin_port = htons(atoi(PORT));
	serversock->sin_addr.s_addr = inet_addr("0.0.0.0");
	return 0;
}
