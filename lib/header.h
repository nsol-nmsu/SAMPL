#ifndef header_h
#define header_h

#include <stddef.h>

/*
 * header structure, not set in stone...
 */
struct header {
	char workflow_number[4];
	char field1_len[20];
	char field2_len[20];
	char field3_len[20];
	char field4_len[20];
};

/*
 * build a header into a string, to use
 * in sending messages
 */
char *build_header(size_t flow_num, size_t f1_len, size_t f2_len, size_t f3_len,
				   size_t f4_len);

/*
 * print the header out formated for easy reading
 */
void dump_header(struct header _header);

#endif
