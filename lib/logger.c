
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "logger.h"
#include "payload.h"

int dup_logger_init(char *file_name)
{
	// FILE* fp = fopen(file_name,attrs);
	int fp = open(file_name, O_WRONLY | O_CREAT, 0666);
	if (!fp) {
		printf("error opening file... quitting\n");
		exit(0);
	}
	return fp;
}



void LOG_PACKET(FILE *fp, char *packet, char *additional)
{

	if (!fp) {
		printf("Passed an empty file pointer to LOG_PACKET\n");
		exit(0);
	}

	if (additional) {
		fprintf(fp, "---- additional header info ----\n");
		fprintf(fp, "%s\n", additional);
	}

	struct header *_header = (struct header *)packet;
	fprintf(fp, "%s\n", "######################################");
	fprintf(fp, "%s", "******* header *********\n");
	int flow = atoi(_header->workflow_number);
	int len_1 = atoi(_header->field1_len);
	int len_2 = atoi(_header->field2_len);
	int len_3 = atoi(_header->field3_len);
	int len_4 = atoi(_header->field4_len);

	fprintf(fp, "flow number =   %d\n", flow);
	fprintf(fp, "field 1 length: %d\n", len_1);
	fprintf(fp, "field 2 length: %d\n", len_2);
	fprintf(fp, "field 3 length: %d\n", len_3);
	fprintf(fp, "field 4 length: %d\n", len_4);
	fprintf(fp, "%s", "************************\n");

	char *payload = (char *)(packet + sizeof(struct header));
	char **s_payload = split_payload(_header, payload);

	fprintf(fp, "%s", "******** payload **********\n");
	fprintf(fp, "field 1: %s\n", s_payload[0]);
	fprintf(fp, "field 2: %s\n", s_payload[1]);
	fprintf(fp, "field 3: %s\n", s_payload[2]);
	fprintf(fp, "field 4: %s\n", s_payload[3]);
	fprintf(fp, "%s", "######################################\n");
}
