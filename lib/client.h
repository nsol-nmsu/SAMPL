/*
 * This is the client portion of each entity
 *
 * Create a socket and send out the message,
 * clean/free all memory once finished.
 *
 *
 *
 */

#ifndef client_h
#define client_h

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

void send_msg(char *msg, char *ip, int port);

// same as send message, but read chunks of a file
// and send piece by piece
void send_file(int workflow_num, char *file_name, char *ip, int port);

#endif
