/**
 * @project OS PROJECT S2
 * @file cmd_interpreter.h
 * @author Frederick NEY et Stephane OVERLEN
 * @date 03/29/2016 14:35
 * @revision 1.0.1
 */

#ifndef OS_PROJECT_S2_CMD_INTERPRETOR_H
#define OS_PROJECT_S2_CMD_INTERPRETOR_H


#include "socket.h"

// defining the value of CMD_READ
#define CMD_READ 0x0

// defining the value of CMD_WRITE
#define CMD_WRITE 0x1

// defining the value to exit the server and the client
#define EXIT 0xf

// defining the value of REPLY
#define REPLY 0x76767676

// defining the value of REQUEST
#define REQUEST 0x87878787

// defining the value of buffer SIZE
#define SIZE 1024

/**
 * interpreter function of the messages sent by any host connected to the server
 *
 *
 * @param int sd: socket descriptor
 * @param int fd: file descriptor
 * @param data *data_struct: structure containing payload, error, request and reply.
 * @return cmd_data *request: NULL on error
 */
cmd_data *cmd_interpreter(int sd, int fd, data *data_struct);

#endif //OS_PROJECT_S2_CMD_INTERPRETOR_H
