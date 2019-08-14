/*
 * Application level helper to break apart a packet
 * and seperate the payload fields for use
 *
 */

#ifndef payload_h
#define payload_h

#include "header.h"

/* Use the header to split the payload
 */
char **split_payload(struct header *_header, char *payload);

/* Print all the payload fields to see what is 
 * happening
 */
void dump_payload_fields(char **payload_fields);

/* Free any fields held in payload_fields, leave
 * payload_fields itself with memory
 */
void clean_payload_fields(char **payload_fields);

/* free everything
 */
void free_payload_fields(char **payload_fields);

#endif
