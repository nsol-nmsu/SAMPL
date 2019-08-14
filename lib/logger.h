/*
 * Make logging easier
 * write info to a file, rather stdio
 *
 */

#ifndef logger_h
#define logger_h

/* Just returns a file descriptor for dup2 to be called
 * with to redirect stdout
 */
int dup_logger_init(char *file_name);

/* log packets, including meta data to file
 * Helpful to see how the data is being sent between entities
 * in the protocol
 */
void LOG_PACKET(FILE *fp, char *packet, char *additional);

#endif
