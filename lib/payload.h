/*
 * given a header,
 * and a string payload
 *
 * break apart the payload into char**
 * to return to the user to use as needed
 *
 *
 */

#ifndef payload_h
#define payload_h

#include "header.h"

/*
 * use the header to split the payload
 */
char **split_payload(struct header *_header, char *payload);

void dump_payload_fields(char **payload_fields);

/*
 * free any fields held in payload_fields, leave
 * payload_fields itself with memory
 */
void clean_payload_fields(char **payload_fields);

/* free everything
 */
void free_payload_fields(char **payload_fields);

#endif
