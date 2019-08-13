#ifndef merkle_h
#define merkle_h

#include "crypto.h"

char *get_root(char **hashes, int size);

char *get_siblings(char **target_hash_list, char **hashes);

char *get_root_from_siblings(char **target_hash_list, char **siblings);

#endif
