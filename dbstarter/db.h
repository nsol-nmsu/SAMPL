#ifndef db_h
#define db_h

#include "common.h"

/* set some constants to use here
 */

#define MERKLE_SIZE 32

/*************************************************
 * Set the name of the generated DB here
 ************************************************/
char *db_name = "test.db";

int dbstarter_open_db()
{
	int rc = 0;
	char *zErrMsg = 0;

	rc = sqlite3_open(db_name, &db);
	if (rc == SQLITE_OK)
		printf("opened database okay\n");
}

int callback_gen(void *data, int argc, char **argv, char **azColName)
{
	printf("%s\n", (const char *)data);
	for (int i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i]);
	}
	return 0;
}


// number of users is defined by the length of the list
struct gen_stats_struct {
	size_t number_of_days;
	size_t data_size;
};

/* for quick reference */
struct gen_stats_struct gen_stats;

/* for timing */
clock_t start, end;
clock_t root_start, root_end;

/* database reference */
static sqlite3 *db;


/* a counter for use later */
int num_names = 0;
char *create_user_account = "\
CREATE TABLE USER_ACCOUNT \
( ID INTEGER PRIMARY KEY AUTOINCREMENT , \
R_i varchar(255)  , \
A_i varchar(255)  , \
P_i varchar(255)  , \
P_priv varchar(255)  , \
P_pub  varchar(255)  , \
id_proof varchar(255) , \
sym_key varchar(255) );";

char *create_data_entry = "\
CREATE TABLE DATA_ENTRY \
( ID INTEGER PRIMARY KEY AUTOINCREMENT  , \
A_i varchar(255)  , \
date integer, \
enc_content blob  , \
ct_len integer , \
content_hash varchar(255)  , \
batch_number integer  );";

char *create_merkle_entry = "\
CREATE TABLE MERKLE_ENTRY \
( ID INTEGER PRIMARY KEY AUTOINCREMENT  , \
batch_number integer  , \
name varchar(255)  , \
root_hash varchar(255)  , \
root_sig blob  );";


void query_user()
{
	int rc = 0;
	char *zErrMsg = 0;
	char *sql_query = "SELECT * from USER_ACCOUNT";
	const char *data = "***  ***\n";
	rc = sqlite3_exec(db, sql_query, callback_gen, (void *)data, &zErrMsg);
	if (rc != SQLITE_OK) {
		printf("[sql error] %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		printf("query success\n");
	}
}

void query_data()
{
	int rc = 0;
	char *zErrMsg = 0;
	char *sql_query = "SELECT * from DATA_ENTRY";
	const char *data = "***  ***\n";
	rc = sqlite3_exec(db, sql_query, callback_gen, (void *)data, &zErrMsg);
	if (rc != SQLITE_OK) {
		printf("[sql error] %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		printf("query success\n");
	}
}

void query_merkle()
{
	int rc = 0;
	char *zErrMsg = 0;
	char *sql_query = "SELECT * from MERKLE_ENTRY";
	const char *data = "***  ***\n";
	rc = sqlite3_exec(db, sql_query, callback_gen, (void *)data, &zErrMsg);
	if (rc != SQLITE_OK) {
		printf("[sql error] %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		printf("query success\n");
	}
}


void create_USER_ACCOUNT()
{
	int rc = 0;
	char *zErrMsg = 0;
	char *sql_query = create_user_account;

	rc = sqlite3_exec(db, sql_query, callback_gen, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		printf("[sql error] %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		printf("Table created succesfully\n");
	}
}

void create_DATA_ENTRY()
{
	int rc = 0;
	char *zErrMsg = 0;
	char *sql_query = create_data_entry;

	rc = sqlite3_exec(db, sql_query, callback_gen, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		printf("[sql error] %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		printf("Table created succesfully\n");
	}
}

void create_MERKLE_ENTRY()
{
	int rc = 0;
	char *zErrMsg = 0;
	char *sql_query = create_merkle_entry;

	rc = sqlite3_exec(db, sql_query, callback_gen, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		printf("[sql error] %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		printf("Table created succesfully\n");
	}
}

#endif
