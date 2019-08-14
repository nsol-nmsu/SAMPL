/*
 * Server code for all entities
 *
 * Spin up their server component, given their
 * unique needs
 *
 * The server will retrieve the packet from a sender
 * and extract the workflow_number from the header, then
 * call the specified callback function with this workflow number
 */

#ifndef server_h
#define server_h

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include "header.h"

/* Start the server component of the entitiy
 *
 * A function pointer needs to be passed to handle their portion of the
 * protocol
 *
 * target: Is what workflow numbers the caller is able to handle
 *
 */
void start_server(int port, void (*f)(int, struct header, char *), int *targets,
				  int debug);

/* Used internally by the server component to extract the workflow_number
 */
int parse_input(int id, int *targets);

#endif
