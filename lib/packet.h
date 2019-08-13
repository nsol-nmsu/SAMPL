/*
 * given a set of information, build a 'packet'
 *
 * header + payload_fields
 *
 *
 */

#ifndef packet_h
#define packet_h

char *build_packet(int workflow_number, char **payload_fields, int reset);

char *build_simple_packet(int flow_num, char *payload, size_t len);

/*
 * break apart and format print the contents of the packet
 */
void dump_packet(char *packet);

#endif
