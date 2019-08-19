
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "merkle.h"
#include "parser.h"
#include "token.h"


int verify_file(char *file_name)
{
	FILE *fp;
	fp = fopen(file_name, "r");
	FILE *tt = fopen("root_to_total.txt", "w");
	fclose(tt);

	// The fields correspond to the pieces of info that
	// will be used to verify the zkp, and signatures.
	char **fields = malloc(1024);
	fields[0] = malloc(4096);
	fields[1] = malloc(4096);
	fields[2] = malloc(4096);
	fields[3] = malloc(4096);
	fields[4] = malloc(4096);
	fields[5] = malloc(4096);
	fields[6] = malloc(4096);

	size_t counter = 0;

	// get the size of the file
	int file_size;
	struct stat res;
	stat(file_name, &res);
	file_size = res.st_size;

	// a lot of messy variables.
	char c;
	int index = 0;
	int i = 0;
	int count = 0;
	char **rev_tokens;
	char **enc_cont_list;
	char **hash_cont;
	char *tree_root;
	FILE *o_file;
	size_t decode_len;
	unsigned char *decode_buffer;
	FILE *out_file;
	int NUM = 0;

	/* Process the field char by char until reached end of segment.
	 * The file is delimited by '!', so each section is unique.
	 * Once the end ']' is reached, each of the fields is used
	 * to verify the data.
	 *
	 * Each '[' begins a new batch of data to verify.
	 * The batch of data is the specific batch of data
	 * that makes up the merkle tree used in the authenticity 
	 * of the data
	 *
	 * Not implemented here is verification of non-full 
	 * merkle trees. Operating under the assumption we
	 * have a perfect sized batch, although verification of the 
	 * zero knowledge proof has been tested and works with non-full
	 * merkle trees. 
	 */
	while ((c = getc(fp)) != EOF) {

		switch (c) {

		// start new test
		case '[':
			//printf("[*] Starting process of ");
			index = 0;
			i = 0;
			break;

		// finish test: compare hashes here
		case ']':
			counter++;
			printf("%s\n", fields[2]);
	
			NUM = atoi(fields[6]);

			// enc_cont is the second field of the serialized batch
			enc_cont_list = s_tokenize(fields[1],&count);

			// the content needs to be reversed to be
			// processed in the correct order
			// enough space to hold 64 char*
			rev_tokens = malloc(4096); 

			for (int i = count-1, j = 0; i >= 0; i--, j++) {
				rev_tokens[j] = enc_cont_list[i]; //hash_list[i];
			}

			// terminate the list of content
			rev_tokens[NUM] = NULL;

			// now that have content list, need to create hash of all content
			hash_cont = malloc(4096);

			
			// only hash the encrypted content
			for(int i = 0; i <= NUM-1; i++) {
				hash_cont[i] = hash(rev_tokens[i]);
			}

			// fill in the rest of the content
			for(int i = NUM+1; i < count; i++) {
				hash_cont[i] = rev_tokens[i];
			}

			//hash_cont[NUM] = '\0'; 
			hash_cont[count] = '\0';

			/* with the hashes recreated from the encrypted content,
			 * calculate the root hash of the merkle tree
			 */
			if(NUM == 32) {
				tree_root = get_root(hash_cont, 32);
			} else if(NUM < 32) {
				tree_root = get_root_from_siblings(hash_cont,NUM);
			}

			// verify the root hash is good
			printf("\n=============\n=============\n");
			if ((strcmp(tree_root, fields[0])) == 0) {
				printf("[*] Merkle root is good\n");
				fflush(stdout);
			} else {
				printf("[*] Merkle root is NOT good\n");
				fflush(stdout);
			}


			// now verify the signature of that root hash
			// can call the verify python
			out_file = fopen("id_proof.txt", "w");

			// decode before writing
			base64_decode(fields[5],&decode_buffer,&decode_len);
			fwrite(decode_buffer,decode_len,1,out_file);
			fwrite(fields[5], strlen(fields[5]), 1, out_file);
			fclose(out_file);
			free(decode_buffer);

			/***********************************
			 * dump the contents for python to 
			 * perform the zkp
			 ************************************/
			out_file = fopen("root_hash.txt", "w");
			fwrite(tree_root, strlen(tree_root), 1, out_file);
			fclose(out_file);

			out_file = fopen("root_sig.txt", "w");
			fwrite(fields[3], strlen(fields[3]), 1, out_file);
			fclose(out_file);

			out_file = fopen("p_pub.txt", "w");
			fwrite(fields[4], strlen(fields[4]), 1, out_file);
			fclose(out_file);

			system("python3 lib/python/verify.py");

			// reset the fields for the next batch
			for (int i = 0; i <= 6; i++) 
				memset(fields[i], '\0', 4096);

			free(rev_tokens);
			free(hash_cont);
			free(enc_cont_list);

			break;

		// next field of current case
		case '!':
			fields[index][i] = '\0';
			index++;
			i = 0;
			break;

		default:
			fields[index][i++] = c;
			break;
		}
	}

	fclose(fp);
	//printf("COUNTER: %ld\n", counter);
	free(fields[0]);
	free(fields[1]);
	free(fields[2]);
	free(fields[3]);
	free(fields[4]);
	free(fields);
}
