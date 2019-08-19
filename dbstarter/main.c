/* main for database generator */

#include "common.h"

/* For each user generated, all fields need to be generated at the
 * same time
 *
 * for generation
 * 1. get a name from names.txt
 * 2. generate the sym_key
 * 3. generate zkp
 * 4. submit
 */
void insert_USER_ACCOUNT()
{
	printf("populating database USER_ACCOUNT\n");
	int rc = 0, i = 0, res = 0;
	char *zErrMsg = 0;
	char *o_query =
		"INSERT INTO USER_ACCOUNT(R_i,A_i,P_i,P_priv,P_pub,id_proof,sym_key)\
					 VALUES(?,?,'temp',?,?,?,?)";

	char *sql_query = malloc(strlen(o_query) + 1);
	unsigned char *sym_key = malloc(33);
	sqlite3_stmt *stmt = 0;
	char *name = malloc(150);
	char A_i[150];
	FILE *fp = fopen("names.txt", "r");
	char *base64_sym_key;
	size_t len, read;
	char id_proof[4096], p_priv[1024], p_pub[1024], c;
	FILE *ffp;

	while ((read = getline(&name, &len, fp)) != -1) {

		/*******************************************************
		 * generate all zkp and write them out to read from c */
		system("python3 python/gen.py");

		/* id proof */
		ffp = fopen("id_proof.txt", "r");
		while ((c = getc(ffp)) != EOF) {
			id_proof[i] = c;
			i++;
		}
		id_proof[i] = '\0';
		i = 0;
		fclose(ffp);

		/* private key */
		ffp = fopen("p_priv.txt", "r");
		while ((c = getc(ffp)) != EOF) {
			p_priv[i] = c;
			i++;
		}
		p_priv[i] = '\0';
		i = 0;
		fclose(ffp);

		/* public key */
		ffp = fopen("p_pub.txt", "r");
		while ((c = getc(ffp)) != EOF) {
			p_pub[i] = c;
			i++;
		}
		p_pub[i] = '\0';
		i = 0;
		fclose(ffp);

		/******************************************************/

		// create their P_i ( email )
		name[read - 1] = '\0';
		sprintf(A_i, "%s%s", name, "@gmail.com");

		gen_sym_key(sym_key, 32);
		strcpy(sql_query, o_query);
		rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);

		if (rc != SQLITE_OK) {
			printf("Error.... quiting\n");
			exit(0);
		}

		base64_encode(sym_key, 32, &base64_sym_key);

		rc = sqlite3_bind_text(stmt, 1, name, strlen(name), SQLITE_STATIC);
		rc = sqlite3_bind_text(stmt, 2, A_i, strlen(A_i), SQLITE_STATIC);
		rc = sqlite3_bind_text(stmt, 3, p_priv, strlen(p_priv), SQLITE_STATIC);
		rc = sqlite3_bind_text(stmt, 4, p_pub, strlen(p_pub), SQLITE_STATIC);
		rc = sqlite3_bind_text(stmt, 5, id_proof, strlen(id_proof),
							   SQLITE_STATIC);
		rc = sqlite3_bind_text(stmt, 6, base64_sym_key, strlen(base64_sym_key),
							   SQLITE_STATIC);

		rc = sqlite3_step(stmt);

		if (rc != SQLITE_DONE)
			printf("Final error inserting into table\n");

	} // end while

	sqlite3_finalize(stmt);
	free(sym_key);
	fclose(fp);
	free(name);
}

/* given the parameters, just pass them into their place and create a merkle
 * entry
 */
void insert_MERKLE_ENTRY(int batch_number, const char *name, char *root_hash,
						 unsigned char *root_sig, size_t sig_len)
{
	printf("[merkle] name: %s batch#: %d\n", name, batch_number);
	int rc = 0;
	sqlite3_stmt *stmt = 0;
	const char *zErrMsg = 0;

	char *i_query =
		"INSERT INTO MERKLE_ENTRY(batch_number,name,root_hash,root_sig)\
				   VALUES(?,?,?,?)";

	rc = sqlite3_prepare_v2(db, i_query, -1, &stmt, 0);
	rc = sqlite3_bind_int(stmt, 1, batch_number);
	rc = sqlite3_bind_text(stmt, 2, name, strlen(name), SQLITE_STATIC);
	rc =
		sqlite3_bind_text(stmt, 3, root_hash, strlen(root_hash), SQLITE_STATIC);
	rc = sqlite3_bind_text(stmt, 4, root_sig, strlen(root_sig), SQLITE_STATIC);

	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

// simple insert of data for a user A_i
void insert_data(const char *A_i, int date, const unsigned char *enc_cont,
				 int ct_len, const char *content_hash, int batch_num)
{
	int rc = 0;
	const char *zErrMsg = 0;
	char *sql_query =
		"INSERT INTO DATA_ENTRY(A_i,date,enc_content,ct_len,content_hash,batch_number)\
				   VALUES(?,?,?,?,?,?)";
	size_t query_length = strlen(sql_query);
	sqlite3_stmt *stmt = 0;

	rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);

	if ((rc = sqlite3_bind_text(stmt, 1, A_i, strlen(A_i), SQLITE_STATIC)) ||
		(rc = sqlite3_bind_int(stmt, 2, date)) ||
		(rc = sqlite3_bind_blob(stmt, 3, enc_cont, ct_len, SQLITE_STATIC)) ||
		(rc = sqlite3_bind_int(stmt, 4, ct_len)) ||
		(rc = sqlite3_bind_text(stmt, 5, content_hash, strlen(content_hash),
								SQLITE_STATIC)) ||
		(rc = sqlite3_bind_int(stmt, 6, batch_num)) != SQLITE_OK) {
		printf("!!!!! Error with data entry query\n");
	}

	rc = sqlite3_step(stmt); // error here
	sqlite3_finalize(stmt);
}

/* the bread and butter
 * Populate the merkle entries for each user
 * using random data generation, populate entries for
 * the user, sign, and save the root sigs under MERKLE_ENTRY
 */
void insert_DATA_ENTRY_and_MERKLE_ENTRY(size_t num_entries)
{
	printf("\nGenerating DATA_ENTRY && MERKLE_ENTRY\n");
	int rc = 0;
	sqlite3_stmt *sql_statement = 0, *stmt = 0;
	const char *zErrMsg = 0;
	char *sql_query = "SELECT * from USER_ACCOUNT"; // select all users
	size_t query_length = strlen(sql_query);

	/* needed fields from USER_ACCOUNT */
	const char *P_priv, *P_pub, *A_i;
	unsigned char *sym_key;

	/* Fields for DATA_ENTRY */
	int date, ct_len;
	unsigned char *enc_cont = malloc(gen_stats.data_size + 100); // extra to strcat the date
	if(!enc_cont) { printf("[!] Error with malloc\n"); exit(0); }
	char *content_hash;
	unsigned int batch_num;

	char *i_query =
		"INSERT INTO DATA_ENTRY(A_i,date,enc_content,ct_len,content_hash,batch_number)\
				   VALUES(?,?,?,?,?,?)";

	// create the mem for the hashes
	char **content_hash_list = malloc(1024);
	for (int i = 0; i < 32; i++)
		content_hash_list[i] = malloc(33);

	FILE *in_file, *out_file;
	char *b64_con, *root_hash;
	char *b64;
	char *b64_enc_cont;
	char *t_b64 = malloc(gen_stats.data_size + 100);
	int days_counter = 0;
	size_t dec_len;
	unsigned char *decoded;
	int index = 0;
	char *data = malloc(gen_stats.data_size);
	int E_PER_DAY;
	char *temp;
	char c;
	int i;
	int z;
	unsigned char sig[4096];
	int enc_len = 0;
	int set_flag = 0;

	srand(time(0));

	rc = sqlite3_prepare_v2(db, sql_query, -1, &sql_statement, 0);

	if (rc != SQLITE_OK)
		printf("[in query] Error with query for insert into data\n");

	/* loop each user, create their entries */
	while ((rc = sqlite3_step(sql_statement)) == SQLITE_ROW) {
		num_names++;

		rc = sqlite3_prepare_v2(db, i_query, -1, &stmt, 0);

		A_i = sqlite3_column_text(sql_statement, USER_A_i);
		printf("Starting for %s\n", A_i);
		P_priv = sqlite3_column_text(sql_statement, USER_P_priv);
		P_pub = sqlite3_column_text(sql_statement, USER_P_pub);
		sym_key = (unsigned char *)sqlite3_column_text(sql_statement, USER_sym_key);

		base64_decode(sym_key, &decoded, &dec_len);

		/* set the rest of the values for DATA_ENTRY */
		date = 11;
		batch_num = 1;

		/************** loop here *****************/
		index = 0;


		/* loop the number of days */
		for (int j = 0; j < gen_stats.number_of_days; j++) {
			//E_PER_DAY = rand() % 20 + 22; // rand between 22 and 42
			//E_PER_DAY = 32;
			E_PER_DAY = 34;

			/* loop how many entries per day */
			for (int i = 0; i < E_PER_DAY; i++) {
				date = j;

				// not actually a key gen, but random data gen
				gen_sym_key(data, gen_stats.data_size); 
				ct_len = sym_encrypt(data, gen_stats.data_size, sym_key, 0, enc_cont);

				// encode enc_content
				base64_encode(enc_cont,ct_len,&b64);

				if(!set_flag) {
					enc_len = strlen(b64) - 1;
					set_flag = 1;
				}

				b64[enc_len] = '\0';

				// encode enc_content again for later
				base64_encode(enc_cont,ct_len,&b64_enc_cont);
				b64_enc_cont[enc_len] = '\0';

				//printf("[*] %d  \n",enc_len);

				temp = malloc(strlen(b64)+1);
				sprintf(temp,"%s%d",b64,date);

				content_hash = hash(temp); // hash includes date
				strncpy(content_hash_list[index++], content_hash, 33);

				insert_data(A_i, date, b64_enc_cont, strlen(b64_enc_cont), content_hash, batch_num);

				free(b64_enc_cont);
				free(b64);
				free(temp);

				/* build a 32 size of merkle tree */
				if (index == 32) {
					index = 0;

					// get the root hash of the built merkle tree
					root_hash = get_root(content_hash_list, 32);

					/***** write out the countents for python script *****/
					out_file = fopen("p_priv.txt", "w");
					fwrite(P_priv, strlen(P_priv), 1, out_file);
					fclose(out_file);

					out_file = fopen("p_pub.txt", "w");
					fwrite(P_pub, strlen(P_pub), 1, out_file);
					fclose(out_file);

					// write the root hash to get signed
					out_file = fopen("to_sign.txt", "w");
					fwrite(root_hash, strlen(root_hash), 1, out_file);
					fclose(out_file);

					system("python3 python/sign.py");
					/********************************************************/

					/* signature is now available under sig.txt... read it */
					/**************not meant to be
					 * readable**************************/
					in_file = fopen("sig.txt", "r");
					memset(sig, '\0', 4096);
					z = 0;
					while ((c = getc(in_file)) != EOF) {
						sig[z] = c;
						z++;
					}
					sig[z] = '\0';
					z = 0;
					fclose(in_file);
					/****************************************************************/

					// insert this merkle entry into the database
					insert_MERKLE_ENTRY(batch_num++, A_i, root_hash, sig, 0);

					memset(sig, '\0', 4096);
					free(root_hash);


					// reset
					for (int i = 0; i < 32; i++)
						memset(content_hash_list[i], '\0', 33);

				} // end merkle if
			}	 // end data generation for user
		}		  // end number of days
	}			  // end all users
	sqlite3_finalize(stmt);
}

void dbstarter_clean_up()
{
	printf("Closing the database\n");
	sqlite3_close(db);
}

/*  Options:
 *  0         1         2
 * {./p} {user/data} {# days}
 */
int main(int argc, char *argv[])
{

	dbstarter_open_db();

	if (argc < 2) {
		printf("Please enter what to do\n");
		printf("\t--user\n\t--data\n\t--query-{user/data/merkle}\n");
		exit(0);
	}

	if (strcmp("--user", argv[1]) == 0) {
		printf("[*] tail -f test.db to make sure still running\n");
		create_USER_ACCOUNT();
		insert_USER_ACCOUNT();
	}
	else if (strcmp("--data", argv[1]) == 0) {

		if (argc >= 2 && argc < 4) {
			printf("Please enter args for data generation\n");
			printf("Options: { number of days }  { data size }\n");
			printf("HINT: 75000 = 75kb  and  2000000 = 2mb\n");
			exit(0);
		}

		printf("Gen Options:\n");
		printf("[*] # of days                 = %d\n", atoi(argv[2]));
		printf("[*] size of data              = %d\n", atoi(argv[3]));

		gen_stats.number_of_days = atoi(argv[2]);
		gen_stats.data_size = atoi(argv[3]);

		create_DATA_ENTRY();
		create_MERKLE_ENTRY();
		insert_DATA_ENTRY_and_MERKLE_ENTRY(atoi(argv[2]));

		/* display */
		printf("Database generated with options:\n");
		printf("[*] # of days for each user: %d\n", atoi(argv[2]));
		printf("[*] # of bytes per entry: %d\n", atoi(argv[3]));
		printf("[*] # of users: %d\n", num_names);
	}
	else if (strcmp("--query-user", argv[1]) == 0) {
		query_user();
	}
	else if (strcmp("--query-data", argv[1]) == 0) {
		query_data();
	}
	else if (strcmp("--query-merkle", argv[1]) == 0) {
		query_merkle();
	}
	else {
		printf("Please Enter a valid option\n");
	}

	clean_up();
}
