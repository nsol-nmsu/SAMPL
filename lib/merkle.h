#ifndef merkle_h
#define merkle_h

#include "crypto.h"

/* Given a list of hashes, calculate the root of the merkle
 * tree
 */
char *get_root(char **hashes, int size);

/* Given a subset of the list of hashes, and the full list of hashes
 * return a list of the siblings.
 *
 * This is to calculate a root hash of the merkle tree, while only
 * passing along a subset of the list of hashes.
 *
 * Used when the Law entity requests data that falls somewhere in a subset
 * of the merkle tree, So the company can collect the siblings, to be
 * able to pass on the subset of hashes, with the siblings, so the Enforcer
 * can recreate the root hash of the merkle tree with the subset.
 */
char *get_siblings(char **target_hash_list, char **hashes);

/* Calcule the root of the merkle tree, given the siblings, and a sub
 * set of the list of hashes of the merkle tree
 */
char *get_root_from_siblings(char **target_hash_list, char **siblings);

#endif
