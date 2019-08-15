/*
 * Database access helpers
 *
 * Database access is primitive and queries are bare bones.
 * Functionality includes only what was needed to accomplish the
 * protocol.
 */

#ifndef database_h
#define database_h

#include <sqlite3.h>
#include <stdio.h>

/* enum used to help make queries more
 * readable throughout.
 */
enum USER_ACCOUNT_FIELD {
	USER_R_i = 1,
	USER_A_i = 2,
	USER_P_i = 3,
	USER_P_priv = 4,
	USER_P_pub = 5,
	USER_id_proof = 6,
	USER_sym_key = 7
};

enum MERKLE_field {
	MERKLE_batch_number = 1,
	MERKLE_name = 2,
	MERKLE_root_hash = 3,
	MERKLE_root_sig = 4
};

static sqlite3 *db;
int open_db();
void create_table();
void insert();

/* Use R_i as the key */
char *query_USER_ACCOUNT_R_i(int user_field, char *R_i);

/* Use A_i as the key */
char *query_USER_ACCOUNT(int user_field, char *A_i);

/* A_i provides unique look up
 * merkle_field tells what field to pull from the merkle entry
 */
char *query_MERKLE_ENTRY(int merkle_field, char *A_i, int batch_number);

/* query a whole batch, by name, and batch number
 * return a formated string, that can be parsed later,
 * including all info about the batch.
 *
 * include root_hash, all hashes of the batch, and
 * the A_i corresponding keys
 */

/* Get all the encrypted content of a batch
 * ! Ignores if the batch consists of a full merkle tree
 * * To query only the relevent data from the batch that
 * belongs to the SO, user query_by_batch_notfull
 */
char *query_by_batch(int batch_num, char *A_i);

/* Query only the relevent content that falls under the
 * SO.
 * This function will utilize the get_siblings, and return
 * a properly serialized c string with relevant information
 * and the sibling hashes in the merkle tree so the root
 * hash can be recreated with the subset of data
 *
 */
char *query_by_batch_notfull(int batch_num, char *A_i, int target_date);


/* Used to check if a batch contains a complete merkle tree.
 */
int check_full_batch(int batch_number, char *A_i);


/* Return the first batch number based on the data
 */
int get_first_num_batch_by_date(int date, char *A_i, int *is_full);


/* Return the last batch number based on the data
 */
int get_last_num_batch_by_date(int date, char *A_i, int *is_full);


/* Get all the hashes of the merkle tree by the batch
 * number
 * returns: hashes of full merkle tree ( space delimited )
 */
char *query_all_hashes_by_batch(int batch_num, char *A_i);


/* query all the enc_content of the merkle tree by
 * the batch number
 * returns: base64 encoded encrypted content of full 
 * 		merkle tree ( space delimited )
 */
char *query_all_enc_cont_by_batch(int batch_num, char *A_i);


char *query_all_hashes_by_batch_exact(int batch_num, char *A_i, int target_date);


char *query_all_enc_cont_by_batch_exact(int batch_num, char *A_i, int target_date);


/* close the db descriptor
 */
void clean_up();


/* callback for sqlite3 query
 */
static int callback(void *data, int argc, char **argv, char **azColName);


#endif
