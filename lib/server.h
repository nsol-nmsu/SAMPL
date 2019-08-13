

/*
 * notes on what to work on here
 *
 * Thread/fork the server.
 *  copy the memory to newly allocated blocks to
 *  avoid memory leaks, then free once fork is finished
 *
 *
 */

/*
 * Server code for all entities
 *
 * Spin up their server component, given their
 * unique needs
 *
 * can pass in buffers needed to hold some memory
 * can pass 'callback' funtions and function pointers to handle routines
 * can pass port numbers
 *
 *
 *
 *
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

void start_server(int port, void (*f)(int, struct header, char *), int *targets,
				  int debug);

int parse_input(int id, int *targets);

#endif
