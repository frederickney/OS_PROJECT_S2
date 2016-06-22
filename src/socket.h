/**
 * @project OS PROJECT S2
 * @file socket.h
 * @author Frederick NEY et Stephane OVERLEN
 * @date 05/24/2016 14:35
 * @revision 1.0.1
 */

#ifndef OS_PROJECT_S2_SOCKET_H
#define OS_PROJECT_S2_SOCKET_H


#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#pragma pack(push, 1)
// cmd_data structure received
typedef struct {
	/// protocol signature
	int header;
	/// type of messages WRITE or READ
	int type;
	/// offset use to determine where it will start reading or writing
	int offset;
	/// the length of data which will be readied or written
	size_t length;
	/// unique request identifier
	int handle;
} cmd_data;

// reply to host structure
typedef struct {
	/// protocol signature
	int header;
	/// generated error to specific command
	int error;
	/// unique request identifier it will be the same as the handle on cmd_data
	int handle;
} message;
#pragma pack(pop)

// struct use both by server and hosts
typedef struct {
	/// see message structure
	message *reply;
	/// see cmd_data structure
	cmd_data *request;
	/// use to send data from host to server and from server to host
	char *payload;
	/// use to correct errors on the server side
	int error;
} data;
/**
 * generating socket.
 *
 *
 * @param struct sockaddr_in *serversock: Structure of the socket
 * @return int: 0 on success -1 on failure
 */
int sockgen(struct sockaddr_in *serversock);


#endif //OS_PROJECT_S2_SOCKET_H
