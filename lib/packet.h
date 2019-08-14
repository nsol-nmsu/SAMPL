/*
 * Build a 'packet' to send
 * The packet includes the application level header
 *
 */

#ifndef packet_h
#define packet_h

/* Build a packet, includes building the header and returns the full 
 * packet.
 */
char *build_packet(int workflow_number, char **payload_fields, int reset);

/* Build a simple packet consisting of only one payload field
 */
char *build_simple_packet(int flow_num, char *payload, size_t len);

/* Print the packet contents out to see the format and what is
 * going on
 */
void dump_packet(char *packet);

#endif
