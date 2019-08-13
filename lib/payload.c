#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "payload.h"

char **split_payload(struct header *_header, char *payload)
{

	/* how many fields is hardcoded in the for loop
	 * if more/less fields are added/removed, update this value
	 * or make it dynamic
	 */

	char **_split_payload = malloc(1024);
	if (!_split_payload) {
		printf("Error with mem alloc in split_payload.. exitiint...\n");
		exit(0);
	}
	memset(_split_payload, '\0', 1024);

	size_t len_1 = atoi(_header->field1_len);
	size_t len_2 = atoi(_header->field2_len);
	size_t len_3 = atoi(_header->field3_len);
	size_t len_4 = atoi(_header->field4_len);

	if (len_1) {
		_split_payload[0] = malloc(len_1 + 1);
		strncpy(_split_payload[0], payload, len_1);
		_split_payload[0][len_1] = '\0';
	}
	else {
		_split_payload[0] = NULL;
	}

	if (len_2) {
		_split_payload[1] = malloc(len_2 + 1);
		strncpy(_split_payload[1], (payload + len_1), len_2);
		_split_payload[1][len_2] = '\0';
	}
	else {
		_split_payload[1] = NULL;
	}

	if (len_3) {
		_split_payload[2] = malloc(len_3 + 1);
		strncpy(_split_payload[2], (payload + len_1 + len_2), len_3);
		_split_payload[2][len_3] = '\0';
	}
	else {
		_split_payload[2] = NULL;
	}

	if (len_4) {
		_split_payload[3] = malloc(len_4 + 1);
		strncpy(_split_payload[3], (payload + len_1 + len_2 + len_3), len_4);
		_split_payload[3][len_4] = '\0';
	}
	else {
		_split_payload[3] = NULL;
	}

	_split_payload[4] = NULL;

	return _split_payload;
}

void dump_payload_fields(char **payload_fields)
{
	char **p = payload_fields;
	while (*p) {
		printf("%s\n", *p);
		p++;
	}
}

void free_payload_fields(char **payload_fields)
{
	char **p = payload_fields;
	while (*p) {
		free(*p);
		*p = NULL;
		p++;
	}
	free(payload_fields);
}
