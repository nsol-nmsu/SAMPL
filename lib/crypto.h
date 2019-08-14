/*
 * Some primitive cryptographic function helpers
 *
 * Crypto utilities using openssl,
 * symetric key encryption,
 * hashing,
 * and base64 encode and decode for storing binary data
 */

#ifndef crypto_h
#define crypto_h
#include <stddef.h>

char *gen_sym_key(unsigned char *key, size_t num);

/* basic md5 hash for POC concepts only!
 */
char *hash(char *data);

char *hash_file(char *file_name);

/* basic base64 helpers as given in openssl examples.
 */
void base64_encode(const unsigned char *buffer, size_t length,
				   char **base64_text);

void base64_decode(const char *base64_text, unsigned char **buffer,
				   size_t *length);

/* Symmectric encryption using AES in CBC mode with 256 bit key
 * note: iv is defaulted to all 1's, so the param makes no different.
 * This was to simply the POC code.
 */
int sym_encrypt(unsigned char *plaintext, int len, unsigned char *key,
				unsigned char *iv, unsigned char *ciphertext);

int sym_decrypt(unsigned char *ciphertext, int len, unsigned char *key,
				unsigned char *iv, unsigned char *plaintext);

#endif
