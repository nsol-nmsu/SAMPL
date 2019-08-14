

#include "client.h"
#include "crypto.h"
#include "header.h"
#include "packet.h"

// message is header+payload
void send_msg(char *msg, char *ip, int port)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error with client socket create\n");
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(ip);

	socklen_t len = sizeof(servaddr);
	if ((connect(sockfd, (struct sockaddr *)&servaddr, len)) < 0) {
		printf("Error with client connect\n");
	}

	send(sockfd, msg, strlen(msg), 0);
	close(sockfd);
}

// when sending a file, send over meta information first,
// such as hash of file, and size of file
// need to build custom headers and build the payloads here
// a workflow_num is special for the enforcer
// !!!!! send_file is hardcoded to enforcer for now !!!!!!!!
void send_file(int workflow_num, char *file_name, char *ip, int port)
{
	printf("[Company] sending file\n");

	char *file_hash = hash_file(file_name);
	printf("[company] File hash: %s\n", file_hash);

	char *simple_packet = build_simple_packet(100, file_hash, 32);
	send_msg(simple_packet, ENFORCERIP, ENFORCERPORT);
	free(simple_packet);

	FILE *fp = fopen(file_name, "r");
	if (!fp) {
		printf("Error opening file\n");
		exit(0);
	}

	int file_size;
	struct stat res;
	if (stat(file_name, &res) == 0)
		file_size = res.st_size;
	else
		printf("Error\n");

	int CHUNK_SIZE = 2048;
	int num_chunks = file_size / CHUNK_SIZE;
	int last_CHUNK_SIZE = file_size % CHUNK_SIZE;

	printf("NUM_CHUNKS: %d\n", num_chunks);
	printf("last chunk %d\n", last_CHUNK_SIZE);

	// FILE* t_file = fopen("test-out.txt","w");

	char *data = (char*)malloc(CHUNK_SIZE + 1);
	int status;
	for (int i = 0; i < num_chunks; i++) {
		// printf("Writing chunk of data\n");
		status = fread(data, CHUNK_SIZE, 1, fp);
		data[CHUNK_SIZE] = '\0';
		// fwrite(data,CHUNK_SIZE,1,t_file);
		simple_packet = build_simple_packet(101, data, CHUNK_SIZE);
		send_msg(simple_packet, ENFORCERIP, ENFORCERPORT);
		free(simple_packet);
		sleep(1);
	}

	if (last_CHUNK_SIZE) {
		// printf("Writing last chunk\n");
		status = fread(data, last_CHUNK_SIZE, 1, fp);
		data[last_CHUNK_SIZE] = '\0';
		// fwrite(data,last_CHUNK_SIZE,1,t_file);
		simple_packet = build_simple_packet(101, data, last_CHUNK_SIZE);
		send_msg(simple_packet, ENFORCERIP, ENFORCERPORT);
		free(simple_packet);
		sleep(1);
	}
}
