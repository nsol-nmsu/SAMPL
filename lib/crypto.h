/*
 * crypto utilities using openssl,
 * symetric key encryption,
 * hashing,
 * and base64 encode and decode for storing binary data
 */

#ifndef crypto_h
#define crypto_h
#include <stddef.h>

/*
 * just generate a random 32 bytes
 */
char *gen_sym_key(unsigned char *key, size_t num);

/*
 * simple hashing from openssl
 */
char *hash(char *data);

char *hash_file(char *file_name);

void base64_encode(const unsigned char *buffer, size_t length,
				   char **base64_text);

void base64_decode(const char *base64_text, unsigned char **buffer,
				   size_t *length);

int sym_encrypt(unsigned char *plaintext, int len, unsigned char *key,
				unsigned char *iv, unsigned char *ciphertext);

int sym_decrypt(unsigned char *ciphertext, int len, unsigned char *key,
				unsigned char *iv, unsigned char *plaintext);

#endif
