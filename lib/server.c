
#include "server.h"
#include "header.h"
#include "packet.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void start_server(int port, void (*f)(int, struct header, char *), int *targets,
				  int debug)
{

	printf("alskjdf;dsjfa;sj\n");
	int listenfd, connfd;
	struct sockaddr_in servaddr, cliaddr;
	char buff[BUFF_SIZE];

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error creating socket\n");
	}
	memset(&servaddr, '\0', sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	if ((bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0) {
		printf("Error with bind\n");
	}
	if ((listen(listenfd, 5)) < 0) {
		printf("error with listen\n");
	}

	socklen_t len;
	char in_message[BUFF_SIZE];
	memset(in_message, '\0', BUFF_SIZE);
	len = sizeof(cliaddr);
	pid_t child;

	size_t s_len = sizeof(struct header);

	for (;;) {
		if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) < 0) {
			printf("Error on accept\n");
		}

		/* fork a child for the server handling */
		if ((child = fork()) == 0) {
			close(listenfd);
			read(connfd, &in_message, BUFF_SIZE);
			if (debug)
				dump_packet(in_message);
			struct header *_header = (struct header *)in_message;
			struct header _h = *_header;
			char *p_load = (char *)(in_message + s_len);
			char *payload = malloc(BUFF_SIZE + 1);
			strcpy(payload, p_load);
			// free(p_load);
			int result = parse_input(atoi(_header->workflow_number), targets);

			(*f)(result, _h, payload);
			exit(0); // exit the child process
		}
		close(connfd);
	}
}

int parse_input(int id, int *targets)
{

	// special workflow for the enforcer
	if (id == 100) {
		return 100;
	}
	else if (id == 101) {
		return 101;
	}

	int num = 1;
	while (targets != NULL) {
		if (id == *targets) {
			return num;
		}
		targets++;
		num++;
	}
	return 0;
}
