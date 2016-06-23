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
	uint32_t header;
	/// type of messages WRITE or READ
	uint32_t type;
	/// offset use to determine where it will start reading or writing
	uint32_t offset;
	/// the length of data which will be readied or written
	uint32_t length;
	/// unique request identifier
	uint32_t handle;
} cmd_data;

// reply to host structure
typedef struct {
	/// protocol signature
	uint32_t header;
	/// generated error to specific command
	uint32_t error;
	/// unique request identifier it will be the same as the handle on cmd_data
	uint32_t handle;
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
	uint32_t error;
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
