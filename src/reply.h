/**
 * @project OS PROJECT S2
 * @file reply.h
 * @author Frederick NEY et Stephane OVERLEN
 * @date 02/06/2016 15:02
 * @revision 1.0.1
 */

#ifndef OS_PROJECT_S2_REPLY_H
#define OS_PROJECT_S2_REPLY_H


#include "socket.h"

/**
 * reply to host
 *
 *
 * @param int sd: socket descriptor.
 * @param data *data_struct: structure containing payload, error, request and reply.
 * @param cmd_data *request_addr: testing if successful writing or reading on error request_addr is NULL.
 * @return int: error code on send the message
 */
int reply(int sd, data *data_struct, cmd_data *request_addr);

#endif //OS_PROJECT_S2_REPLY_H
