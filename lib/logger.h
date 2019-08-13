/*
 * Make logging easier
 * write info to a file, rather stdio for easier parsing
 *
 * At each entities start, use this to keep open a file
 * descriptor for easy writing and logging while running
 *
 */

#ifndef logger_h
#define logger_h

int dup_logger_init(char *file_name);

FILE *logger_init(char *file_name, char *attrs);

void LOG(FILE *fp, char *entry);

// additional will be written first
void LOG_PACKET(FILE *fp, char *packet, char *additional);

#endif
