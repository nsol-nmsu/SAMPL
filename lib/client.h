/*
 * This is the client portion of each entity
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

/* msg = full packet ( including application level headers )
 *
 * Send a message to another entity.
 * Each send_msg triggers the next event in the protocol to happen
 */
void send_msg(char *msg, char *ip, int port);

/* A special routine for the company to use to send a completed
 * file of data to the enforcer to verify
 */
void send_file(int workflow_num, char *file_name, char *ip, int port);

#endif
