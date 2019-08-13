
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *build_header(size_t flow_num, size_t f1_len, size_t f2_len, size_t f3_len,
				   size_t f4_len)
{
	char *buff = malloc(sizeof(struct header) + 1);
	memset(buff, '\0', sizeof(struct header) + 1);
	sprintf(buff, "%4ld%20ld%20ld%20ld%20ld", flow_num, f1_len, f2_len, f3_len,
			f4_len);

	return buff;
}

/*
 * When printing, header fields are casted to ints
 * to verify they are casted correctly
 */
void dump_header(struct header _header)
{
	printf("******* header *********\n");

	size_t flow = atoi(_header.workflow_number);
	size_t len_1 = atoi(_header.field1_len);
	size_t len_2 = atoi(_header.field2_len);
	size_t len_3 = atoi(_header.field3_len);
	size_t len_4 = atoi(_header.field4_len);

	printf("flow number =   %ld\n", flow);
	printf("field 1 length: %ld\n", len_1);
	printf("field 2 length: %ld\n", len_2);
	printf("field 3 length: %ld\n", len_3);
	printf("field 4 length: %ld\n", len_4);
	printf("************************\n");
}
