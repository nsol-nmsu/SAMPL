

#include "crypto.h"
#include <openssl/conf.h>
#include <openssl/md5.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *gen_sym_key(unsigned char *key, size_t num)
{
	if (!RAND_bytes(key, num))
		printf("Error with generating the sym_key\n");
}

/* md5 right now
 */
char *hash(char *data)
{
	//printf("[hash] %s [len] %d\n",data,strlen(data));
	unsigned char md5_sum[MD5_DIGEST_LENGTH];
	MD5_CTX md;

	MD5_Init(&md);
	MD5_Update(&md, data, strlen(data));
	MD5_Final(md5_sum, &md);

	char md_string[33];
	for (int i = 0; i < 16; i++)
		sprintf(&md_string[i * 2], "%02x", (unsigned int)md5_sum[i]);

	char *md5sum = (char*)malloc(33);
	memcpy(md5sum, md_string, 32);
	md5sum[32] = '\0';
	return md5sum;
}

char *hash_file(char *file_name)
{
	unsigned char md5_sum[MD5_DIGEST_LENGTH];
	FILE *fp = fopen(file_name, "rb");
	if (!fopen) {
		printf("Error opening file for hashing\n");
		exit(0);
	}
	MD5_CTX md;
	int bytes;
	unsigned char data[1024];

	MD5_Init(&md);
	while ((bytes = fread(data, 1, 1024, fp)) != 0)
		MD5_Update(&md, data, bytes);
	MD5_Final(md5_sum, &md);

	char *md5sum = (char*)malloc(33);

	// change this
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
		sprintf(&md5sum[i * 2], "%02x", (unsigned int)md5_sum[i]);

	fclose(fp);
	return md5sum;
}

size_t calc_decode_len(const char *base64_input)
{

	size_t len = strlen(base64_input), padding = 0;

	if (base64_input[len - 1] == '=' && base64_input[len - 2] == '=')
		padding = 2;
	else if (base64_input[len - 1] == '=')
		padding = 1;
	return (len * 3) / 4 - padding;
}

void base64_encode(const unsigned char *buffer, size_t length,
				   char **base64_text)
{

	BIO *bio, *b64;
	BUF_MEM *bufferPtr;

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

	BIO_write(bio, buffer, length);
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bufferPtr);
	BIO_set_close(bio, BIO_NOCLOSE);
	BIO_free_all(bio);

	*base64_text = (*bufferPtr).data;
}

void base64_decode(const char *base64_text, unsigned char **buffer,
				   size_t *length)
{
	BIO *bio, *b64;

	int decode_len = calc_decode_len(base64_text);
	*buffer = (unsigned char *)malloc(decode_len + 1);
	(*buffer)[decode_len] = '\0';

	bio = BIO_new_mem_buf(base64_text, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	*length = BIO_read(bio, *buffer, strlen(base64_text));
	BIO_free_all(bio);
}

int sym_encrypt(unsigned char *plaintext, int pt_len, unsigned char *key,
				unsigned char *iv, unsigned char *ciphertext)
{
	EVP_CIPHER_CTX *ctx;
	int length;
	int ciphertext_len;

	unsigned char *ivv = (unsigned char*)"1111111111111111111111111111111";

	if (!(ctx = EVP_CIPHER_CTX_new()))
		printf("Error with new ctx\n");

	if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, ivv) != 1)
		printf("Error with encrypt init\n");

	if (EVP_EncryptUpdate(ctx, ciphertext, &length, plaintext, pt_len) != 1)
		printf("Error with evp update\n");

	ciphertext_len = length;

	if (EVP_EncryptFinal_ex(ctx, ciphertext + length, &length) != 1)
		printf("Error with evp final\n");

	ciphertext_len += length;

	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}

int sym_decrypt(unsigned char *ciphertext, int ct_len, unsigned char *key,
				unsigned char *iv, unsigned char *plaintext)
{

	EVP_CIPHER_CTX *ctx;

	int len;

	int plaintext_len;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		printf("Error with decypt new\n");

	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		printf("Error with decrypt init\n");

	if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ct_len))
		printf("Error with decypt update\n");

	plaintext_len = len;

	if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
		printf("Error with decypt final\n");

	plaintext_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}
