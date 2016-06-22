/**
 * @project OS PROJECT S2
 * @file get_request.h
 * @author Frederick NEY et Stephane OVERLEN
 * @date 02/06/2016 15:05
 * @revision 1.0.1
 */

#ifndef OS_PROJECT_S2_GET_REQUEST_H
#define OS_PROJECT_S2_GET_REQUEST_H


#include "socket.h"

/**
 * function to retrieve data structure sent by hosts to the server
 *
 *
 * @param int sd: socket descriptor.
 * @param data *data_struct: structure containing payload, error, request and reply.
 * @return data *request: NULL on error.
 */
cmd_data *get_request (int sd, data *data_struct);

/**
 * function to retrieve payload sent by the remote host
 *
 *
 * @param int sd: socket descriptor.
 * @param data *data_struct: structure containing payload, error, request and reply.
 * @return char *payload: NULL on error.
 */
char *get_payload(int sd, data *data_struct);

#endif //OS_PROJECT_S2_GET_REQUEST_H
