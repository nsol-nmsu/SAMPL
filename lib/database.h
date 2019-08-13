/*
 * database access
 *
 * For now, is simple for testing.
 * Once is set, add larger features and more tables.
 */

#ifndef database_h
#define database_h

#include <sqlite3.h>
#include <stdio.h>

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

char *query_by_batch(int batch_num, char *A_i);

char *query_by_batch_and_date(int batch_num, int date, char *A_i);

int check_full_batch(int batch_number, char *A_i);

int get_first_num_batch_by_date(int date, char *A_i, int *is_full);

int get_last_num_batch_by_date(int date, char *A_i, int *is_full);

char *query_by_date(int date, char *A_i);

char *query_all_hashes_by_batch(int batch_num, char *A_i);

char *query_all_enc_cont_by_batch(int batch_num, char *A_i);

char *query_batch_by_date(int date, char *A_i);

void clean_up();

static int callback(void *data, int argc, char **argv, char **azColName);

/*
 * the populate db call
 *
 * define all tables and their fields here,
 * then call this function to create and populate the
 * database file.
 *
 */
void init_db();

#endif
