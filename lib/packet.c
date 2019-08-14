
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"
#include "packet.h"
#include "payload.h"

// may be longer than can be passed to send()
char *build_packet(int workflow_number, char **payload_fields, int reset)
{
	size_t str_len[10] = {0};
	size_t total_length = 0;

	char **current = payload_fields;
	int i = 0;

	// 4 payload fields
	for (int i = 0; i < 4; i++) {
		if (payload_fields[i] != NULL) {
			str_len[i] = strlen(payload_fields[i]);
		} else {
			str_len[i] = 0;
		}
		total_length += str_len[i];
	}

	char *payload = (char*)malloc(total_length + 1);
	if (!payload) {
		printf("Error creating memory for payload\n");
		exit(0);
	}
	memset(payload, '\0', total_length + 1);

	// set the payload
	memcpy(payload, payload_fields[0], str_len[0]);
	memcpy(payload + str_len[0], payload_fields[1], str_len[1]);
	memcpy(payload + str_len[0] + str_len[1], payload_fields[2], str_len[2]);
	memcpy(payload + str_len[0] + str_len[1] + str_len[2], payload_fields[3],
		   str_len[3]);

	char *header = build_header(workflow_number, str_len[0], str_len[1],
								str_len[2], str_len[3]);

	char *packet = (char*)malloc(total_length + sizeof(struct header) + 1);
	memset(packet, '\0', total_length + sizeof(struct header) + 1);
	if (!packet) {
		printf("Error with malloc for packet\n");
		exit(0);
	}

	sprintf(packet, "%s%s", header, payload);
	free(header);
	free(payload);

	return packet;
}

// one payload field packet...
char *build_simple_packet(int flow_num, char *payload, size_t len)
{
	char *header = build_header(flow_num, len, 0, 0, 0);
	char *packet = (char*)malloc(3048); // large enough..

	sprintf(packet, "%s%s", header, payload);
	free(header);
	free(packet);
	return packet;
}

void dump_packet(char *packet)
{
	struct header *_header = (struct header *)packet;
	printf("######################################\n");
	dump_header(*_header);
	char *payload = (char *)(packet + sizeof(struct header));
	char **s_payload = split_payload(_header, payload);

	printf("******** payload **********\n");
	printf("field 1: %s\n", s_payload[0]);
	printf("field 2: %s\n", s_payload[1]);
	printf("field 3: %s\n", s_payload[2]);
	printf("field 4: %s\n", s_payload[3]);
	printf("######################################\n");
}
