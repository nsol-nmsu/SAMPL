
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "merkle.h"
#include "parser.h"
#include "token.h"

/*
 * file format:
 *
 * { A_i:root_hash:root_sig:hashes_list{repeat}}
 *
 */

int verify_file(char *file_name)
{
	FILE *fp;
	fp = fopen(file_name, "r");
	FILE *tt = fopen("root_to_total.txt", "w");
	fclose(tt);

	// these fields may not be large enough...

	char **fields = malloc(1024);
	fields[0] = malloc(4096);
	fields[1] = malloc(4096);
	fields[2] = malloc(4096);
	fields[3] = malloc(4096);
	fields[4] = malloc(4096);
	fields[5] = malloc(4096);

	size_t counter = 0;

	// get the size of the file
	int file_size;
	struct stat res;
	stat(file_name, &res);
	file_size = res.st_size;

	clock_t start, end;

	char c;
	int index = 0;
	int i = 0;
	int count;
	char **rev_tokens;
	char **enc_cont_list;
	char **hash_cont;
	char *tree_root;
	FILE *o_file;
	size_t decode_len;
	unsigned char *decode_buffer;
	FILE *out_file;
	while ((c = getc(fp)) != EOF) {

		switch (c) {

		// start new test
		case '[':
			printf("[*] Starting process of ");
			index = 0;
			i = 0;
			break;

		// finish test: compare hashes here
		case ']':
			counter++;
			printf("%s\n", fields[2]);

			//char **hash_list = s_tokenize(fields[1], &count);

			// enc_cont is the second field of the serialized batch
			enc_cont_list = s_tokenize(fields[1],&count);

			// the content needs to be reversed to be
			// processed in the correct order
			// enough space to hold 64 char*
			rev_tokens = malloc(4096); 

			for (int i = 31, j = 0; i >= 0; i--, j++) {
				//printf("enc_cont_list[%d] %s\n",i,enc_cont_list[i]);
				rev_tokens[j] = enc_cont_list[i]; //hash_list[i];
			}

			// terminate the list of content
			rev_tokens[32] = NULL;

			// now that have content list, need to create hash of all content

			start = clock();
			hash_cont = malloc(4096);

			for(int i = 0; i <= 31; i++) {
				hash_cont[i] = hash(rev_tokens[i]);
				//printf("hash_cont[%d] %s\n",i,hash_cont[i]);
			}
			
			
			hash_cont[32] = '\0';
			
			//char *tree_root = get_root(rev_tokens, 32);
			tree_root = get_root(hash_cont, 32);

			printf("\n=============\n=============\n");
			// verify the root hash is good
			printf("calced root = %s\n",tree_root);
			printf("known root  = %s\n",fields[0]);
			if ((strcmp(tree_root, fields[0])) == 0) {
				printf("[*] Merkle root is good\n");
				fflush(stdout);
			} else {
				printf("[*] Merkle root is NOT good\n");
				fflush(stdout);
			}

			end = clock();
			//double total = ((double)(end - start) / CLOCKS_PER_SEC);
			//o_file = fopen("root_to_total.txt", "a");
			//fprintf(o_file, "%f\n", total);
			//fclose(o_file);
			// printf("E_ROOT_TIME: %f\n",((double)(end - start) /
			// CLOCKS_PER_SEC));

			// now verify the signature of that root hash
			// can call the verify python
			out_file = fopen("id_proof.txt", "w");
			// decode before writing
			// !!!
			base64_decode(fields[5],&decode_buffer,&decode_len);
			fwrite(decode_buffer,decode_len,1,out_file);
			fwrite(fields[5], strlen(fields[5]), 1, out_file);
			fclose(out_file);
			free(decode_buffer);

			out_file = fopen("root_hash.txt", "w");
			fwrite(tree_root, strlen(tree_root), 1, out_file);
			fclose(out_file);

			out_file = fopen("root_sig.txt", "w");
			// base64_decode(fields[3],&decode_buffer,&decode_len);
			fwrite(fields[3], strlen(fields[3]), 1, out_file);
			fclose(out_file);
			// free(decode_buffer);

			out_file = fopen("p_pub.txt", "w");
			fwrite(fields[4], strlen(fields[4]), 1, out_file);
			fclose(out_file);

			// all should be written for the python now
			// sleep(1);

			printf("Starting python\n\n");
			system("python3 lib/python/verify.py");
			printf("Finished python\n\n");

			// reset the fields for the next batch
			for (int i = 0; i < 6; i++) {
				memset(fields[i], '\0', 4096);
			}
			// system("./temp_clean.sh");
			free(rev_tokens);
			free(hash_cont);
			free(enc_cont_list);

			break;

		// next field of current case
		case '!':
			// printf("[*] moving to next field\n");
			fields[index][i] = '\0';

			// this dumps the fields being worked with
			/*
			if(index == 0)
			  printf("A_i:");
			else if(index == 1)
			  printf("root_hash:");
			else if(index == 2)
			  printf("root_sig:");
			else if(index == 3)
			  printf("P_pub:");
			else if(index == 4)
			  printf("hashes:");
			printf(" %s\n",fields[index]);
			*/
			index++;
			i = 0;
			break;

		default:
			fields[index][i++] = c;
			break;
		}
	}

	fclose(fp);
	printf("COUNTER: %ld\n", counter);
	// free(fields[0]);
	// free(fields[1]);
	// free(fields[2]);
	// free(fields[3]);
	// free(fields[4]);
	// free(fields);
}
