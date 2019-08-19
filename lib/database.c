
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "stack.h"
#include "token.h"
#include "merkle.h"

/*
 * Open the database object, for use with the company
 */
int open_db()
{
	db; // defined in db.h
	int rc = 0;
	char *zErrMsg = 0;

	rc = sqlite3_open("test.db", &db);
	if (rc == SQLITE_OK) {
		printf("opened database okay\n");
	}
}

char *query_USER_ACCOUNT_R_i(int user_field, char *R_i)
{
	int rc = 0;
	sqlite3_stmt *sql_statement;
	const char *zErrMsg = 0;
	char *s_q;
	char *sql_query = (char*)malloc(2048);

	/* Choose what field of the user to retrieve */
	switch (user_field) {
	case USER_R_i:
		s_q = "SELECT R_i from USER_ACCOUNT";
		break;
	case USER_A_i:
		s_q = "SELECT A_i from USER_ACCOUNT";
		break;
	case USER_P_i:
		s_q = "SELECT P_i from USER_ACCOUNT";
		break;
	case USER_P_priv:
		s_q = "SELECT P_priv from USER_ACCOUNT";
		break;
	case USER_P_pub:
		s_q = "SELECT P_pub from USER_ACCOUNT";
		break;
	case USER_id_proof:
		s_q = "SELECT id_proof from USER_ACCOUNT";
		break;
	case USER_sym_key:
		s_q = "SELECT sym_key from USER_ACCOUNT";
		break;
	default:
		break;
	}

	/* Add the where clause to specify what user, by A_i */
	sprintf(sql_query, "%s WHERE R_i = '%s';", s_q, R_i);
	size_t query_length = strlen(sql_query);
	// printf("the query: %s\n",sql_query);

	/* Copy the retrieved data, and return
	 *  ie. return the sym_key for the A_i that was passed
	 */
	char *result = NULL;
	size_t res_size;
	rc = sqlite3_prepare_v2(db, sql_query, query_length, &sql_statement,
							&zErrMsg);

	if (rc == SQLITE_OK) {
		if ((rc = sqlite3_step(sql_statement)) == SQLITE_ROW) {
			res_size = strlen(sqlite3_column_text(sql_statement, 0));
			result = (char*)malloc(res_size + 1);
			strncpy(result, sqlite3_column_text(sql_statement, 0), res_size);
			result[res_size] = '\0';
		}
		else {
			free(sql_query);
			return 0;
		}
	}
	else {
		printf("[sql error query R_i] %s\n", zErrMsg);
		printf("[sql rc error status] %d\n", rc);
	}
	return result;
	free(sql_query);
}

/*
 * Query a certain field from the user
 * specified by A_i
 *
 * Used to get specific info about a user, such as keys and id_proof
 *
 * user_field = enum defined in database.h
 *  it is the column number of the entry
 *
 * A_i = the A_i of the account, used to filter by user
 */
char *query_USER_ACCOUNT(int user_field, char *A_i)
{
	int rc = 0;
	sqlite3_stmt *sql_statement;
	const char *zErrMsg = 0;
	char *s_q;
	char *sql_query = (char*)malloc(2048);

	/* Choose what field of the user to retrieve */
	switch (user_field) {
	case USER_R_i:
		s_q = "SELECT R_i from USER_ACCOUNT";
		break;
	case USER_A_i:
		s_q = "SELECT A_i from USER_ACCOUNT";
		break;
	case USER_P_i:
		s_q = "SELECT P_i from USER_ACCOUNT";
		break;
	case USER_P_priv:
		s_q = "SELECT P_priv from USER_ACCOUNT";
		break;
	case USER_P_pub:
		s_q = "SELECT P_pub from USER_ACCOUNT";
		break;
	case USER_id_proof:
		s_q = "SELECT id_proof from USER_ACCOUNT";
		break;
	case USER_sym_key:
		s_q = "SELECT sym_key from USER_ACCOUNT";
		break;
	default:
		break;
	}

	/* Add the where clause to specify what user, by A_i */
	sprintf(sql_query, "%s WHERE A_i = '%s'", s_q, A_i);
	size_t query_length = strlen(sql_query);

	/* Copy the retrieved data, and return
	 *  ie. return the sym_key for the A_i that was passed
	 */
	char *result;
	size_t res_size;
	rc = sqlite3_prepare_v2(db, sql_query, query_length, &sql_statement,
							&zErrMsg);
	if (rc == SQLITE_OK) {
		if ((rc = sqlite3_step(sql_statement)) == SQLITE_ROW) {
			res_size = strlen(sqlite3_column_text(sql_statement, 0));
			result = (char*)malloc(res_size + 1);
			strncpy(result, sqlite3_column_text(sql_statement, 0), res_size);
			result[res_size] = '\0';
		}
		else {
			free(sql_query);
			return 0;
		}
	}
	free(sql_query);
	return result;
}

char *query_all_hashes_by_batch(int batch_num, char *A_i)
{
	int rc = 0;
	sqlite3_stmt *sql_statement;
	const char *zErrMsg = 0;
	char *sql_query = (char*)malloc(2048);

	/* Add the where clause to specify what user, by A_i */
	sprintf(sql_query,
			"SELECT content_hash FROM DATA_ENTRY WHERE A_i = '%s' AND "
			"batch_number = '%d';",
			A_i, batch_num);
	size_t query_length = strlen(sql_query);

	char *result;
	size_t res_size;

	stack _stack = create_stack();

	rc = sqlite3_prepare_v2(db, sql_query, query_length, &sql_statement,
							&zErrMsg);
	if (rc == SQLITE_OK) {
		while ((rc = sqlite3_step(sql_statement)) == SQLITE_ROW) {
			result = (char *)sqlite3_column_text(sql_statement, 0);
			push(_stack, result);
		}
	}

	char *hashes_list = convert_to_string(_stack, 0);
	free(sql_query);
	free_stack(_stack);
	return hashes_list;
}

char *query_all_enc_cont_by_batch(int batch_num, char *A_i)
{
	int rc = 0;
	sqlite3_stmt *sql_statement;
	const char *zErrMsg = 0;
	char *sql_query = (char*)malloc(2048);

	/* Add the where clause to specify what user, by A_i */
	sprintf(sql_query, \
			"SELECT enc_content, date FROM DATA_ENTRY WHERE A_i = '%s' AND " \
			"batch_number = '%d';", A_i, batch_num);
	size_t query_length = strlen(sql_query);

	char *result;
	char *concat_res;
	size_t res_size;
	int date = 0;

	stack _stack = create_stack();

	rc = sqlite3_prepare_v2(db, sql_query, query_length, &sql_statement,
							&zErrMsg);
	if (rc == SQLITE_OK) {
		printf("[sql query] good status from get all enc content\n");
		while ((rc = sqlite3_step(sql_statement)) == SQLITE_ROW) {
			result = (char *)sqlite3_column_text(sql_statement, 0);
			concat_res = (char*)malloc(strlen(result)+100);
			date = sqlite3_column_int(sql_statement,1);
			sprintf(concat_res,"%s%d",result,date);
			push(_stack, concat_res);
			//free(result);
			free(concat_res);
		}
	} else {
		printf("[!!!] Query fail\n");
		printf("[ value of error ] %d\n",rc);
	}

	char *hashes_list = convert_to_string(_stack, 0);
	free(sql_query);
	free_stack(_stack);
	return hashes_list;
}


char *query_all_enc_cont_by_batch_exact(int batch_num, char *A_i, int target_date, int *num)
{
	int rc = 0;
	sqlite3_stmt *sql_statement;
	const char *zErrMsg = 0;
	char *sql_query = (char*)malloc(2048);

	/* Add the where clause to specify what user, by A_i */
	sprintf(sql_query, \
			"SELECT enc_content, date FROM DATA_ENTRY WHERE A_i = '%s' AND " \
			"batch_number = '%d';", A_i, batch_num);
	size_t query_length = strlen(sql_query);

	char *result;
	char *concat_res;
	size_t res_size;
	int date = 0;

	stack _stack = create_stack();

	rc = sqlite3_prepare_v2(db, sql_query, query_length, &sql_statement,
							&zErrMsg);
	if (rc == SQLITE_OK) {
		printf("[sql query] good status from get all enc content\n");
		while ((rc = sqlite3_step(sql_statement)) == SQLITE_ROW) {
			result = (char *)sqlite3_column_text(sql_statement, 0);
			concat_res = (char*)malloc(strlen(result)+100);
			date = sqlite3_column_int(sql_statement,1);
			sprintf(concat_res,"%s%d",result,date);
			if(target_date == date) {
				push(_stack, concat_res);
				*num = *num + 1;
			}
			//free(result);
			free(concat_res);
		}
	} else {
		printf("[!!!] Query fail\n");
		printf("[ value of error ] %d\n",rc);
	}

	char *hashes_list = convert_to_string(_stack, 0);
	printf("[sub target] %s\n",hashes_list);
	free(sql_query);
	free_stack(_stack);
	return hashes_list;
}


char *query_all_hashes_by_batch_exact(int batch_num, char *A_i, int target_date)
{
	int rc = 0;
	sqlite3_stmt *sql_statement;
	const char *zErrMsg = 0;
	char *sql_query = (char*)malloc(2048);

	/* Add the where clause to specify what user, by A_i */
	sprintf(sql_query, \
			"SELECT content_hash, date FROM DATA_ENTRY WHERE A_i = '%s' AND " \
			"batch_number = '%d';", A_i, batch_num);
	size_t query_length = strlen(sql_query);

	char *result;
	char *concat_res;
	size_t res_size;
	int date = 0;

	stack _stack = create_stack();

	rc = sqlite3_prepare_v2(db, sql_query, query_length, &sql_statement,
							&zErrMsg);
	if (rc == SQLITE_OK) {
		printf("[sql query] good status from get all enc content\n");
		while ((rc = sqlite3_step(sql_statement)) == SQLITE_ROW) {
			result = (char *)sqlite3_column_text(sql_statement, 0);
			date = sqlite3_column_int(sql_statement,1);
			if(target_date == date)
				push(_stack, result);
			//free(result);
		}
	} else {
		printf("[!!!] Query fail\n");
		printf("[ value of error ] %d\n",rc);
	}

	char *hashes_list = convert_to_string(_stack, 0);
	printf("[sub target] %s\n",hashes_list);
	free(sql_query);
	free_stack(_stack);
	return hashes_list;
}


/* the Company queries this to build a string to write to file
 * or send to the enforcer
 */
char *query_by_batch(int batch_num, char *A_i)
{
	printf("[company query database.c] %d %s\n",batch_num,A_i);
	
	char *root_hash = query_MERKLE_ENTRY(MERKLE_root_hash, A_i, batch_num);
	char *root_sig = (char *)query_MERKLE_ENTRY(MERKLE_root_sig, A_i, batch_num);
	char *A_i_P_pub = query_USER_ACCOUNT(USER_P_pub, A_i);
	char *id_proof = query_USER_ACCOUNT(USER_id_proof, A_i);
	char* cont_list = query_all_enc_cont_by_batch(batch_num,A_i);

	char *hashes = query_all_hashes_by_batch(batch_num,A_i);
	int count = 0;
	char **h = s_tokenize(hashes,&count);
	fprintf(stderr,"AAAAAAAAAAAAAAAAAA\n");
	char *rr = get_root(h,32);

	// get full size
	size_t p_len = 0;
	p_len += strlen(root_hash);
	p_len += strlen(cont_list);
	p_len += strlen(A_i);
	p_len += strlen(root_sig);
	p_len += strlen(A_i_P_pub);
	p_len += strlen(id_proof);
	p_len += 1;

	char *to_return = (char*)malloc(p_len+100);
	if(!to_return) {
		printf("Error with malloc for to_return in query_by_batch\n");
	}
	
	sprintf(to_return, "[%s!%s!%s!%s!%s!%s!%d]", root_hash, cont_list, A_i,
				root_sig, A_i_P_pub, id_proof, 32);

	free(root_hash);
	free(root_sig);
	free(A_i_P_pub);
	free(id_proof);
	free(cont_list);

	return to_return;
}


char *query_by_batch_notfull(int batch_num, char *A_i, int target_date)
{
	char *root_hash = query_MERKLE_ENTRY(MERKLE_root_hash, A_i, batch_num);
	char *root_sig = (char *)query_MERKLE_ENTRY(MERKLE_root_sig, A_i, batch_num);
	char *A_i_P_pub = query_USER_ACCOUNT(USER_P_pub, A_i);
	char *id_proof = query_USER_ACCOUNT(USER_id_proof, A_i);

	int num = 0;

	/* The subset of encrypted content that is valid under the SO */
	char *content_exact = query_all_enc_cont_by_batch_exact(batch_num,A_i,target_date,&num);

	/* Get the full list of HASHES to use to get the siblings */
	char *cont_list_full = query_all_hashes_by_batch(batch_num,A_i);
	int c = 0;

	/* tokenize the full list */
	char **sep_list = s_tokenize(cont_list_full,&c);

	/* sub_hash_list is the subset of hashes in the batch */
	char *sub_hash_list = query_all_hashes_by_batch_exact(batch_num,A_i,target_date);

	/* is the tokenized/split version of the subset of hashes */
	char **sep_sub_list = s_tokenize(sub_hash_list,&c);

	/* Get the siblings using the subset */
	char *siblings = get_siblings(sep_sub_list,sep_list);

	/* Build special section here for enforcer to parse */
	size_t n_len = strlen(content_exact) + strlen(siblings) + 1;

	char *n_sec = malloc(n_len+100);

	// special delimiter for not full batch
	sprintf(n_sec,"%s%s",siblings,content_exact);

	//fprintf(stderr,"[**] %s\n",n_sec);
	
	// get full size
	size_t p_len = 0;
	p_len += strlen(root_hash);
	p_len += strlen(n_sec);
	p_len += strlen(A_i);
	p_len += strlen(root_sig);
	p_len += strlen(A_i_P_pub);
	p_len += strlen(id_proof);
	p_len += 1;

	char *to_return = (char*)malloc(p_len+100);
	if(!to_return) {
		printf("Error with malloc for to_return in query_by_batch\n");
	}
	
	// !!!
	sprintf(to_return, "[%s!%s!%s!%s!%s!%s!%d]", root_hash, n_sec, A_i,
				root_sig, A_i_P_pub, id_proof, num);

	//fprintf(stderr,"[to_return] %s\n",to_return);

	free(root_hash);
	free(root_sig);
	free(A_i_P_pub);
	free(id_proof);
	free(cont_list_full);
	free(n_sec);

	return to_return;
}


int check_full_batch(int batch_number, char *A_i)
{
	int rc = 0;
	sqlite3_stmt *sql_statement;
	const char *zErrMsg = 0;
	char *sql_query = (char*)malloc(1024);
	sprintf(sql_query, "SELECT ID,date,batch_number from DATA_ENTRY \
      WHERE A_i = '%s' AND batch_number = %d;",
			A_i, batch_number);

	size_t query_length = strlen(sql_query);
	int date = 0;
	int b_num = 0;

	int first_num = 0;
	int last_num = 0;
	rc = sqlite3_prepare_v2(db, sql_query, query_length, &sql_statement,
							&zErrMsg);
	if (rc == SQLITE_OK) {
		if ((rc = sqlite3_step(sql_statement)) == SQLITE_ROW) {
			//date = sqlite3_column_int(sql_statement, 1);
			//b_num = sqlite3_column_int(sql_statement, 2);
			first_num = sqlite3_column_int(sql_statement, 1);
			while ((rc = sqlite3_step(sql_statement)) == SQLITE_ROW) {
				last_num = sqlite3_column_int(sql_statement, 1);
			} // end while
		}
	} else {
		printf("Error\n");
		exit(0);
	}
	free(sql_query);
	
	return (last_num == first_num);
}

int get_last_num_batch_by_date(int date, char *A_i, int *is_full)
{
	int rc = 0;
	sqlite3_stmt *sql_statement;
	const char *zErrMsg = 0;
	char *sql_query = (char*)malloc(1024);
	sprintf(sql_query, "SELECT batch_number from DATA_ENTRY \
      WHERE A_i = '%s' AND date = %d ORDER BY batch_number DESC LIMIT 1;",
			A_i, date);

	size_t query_length = strlen(sql_query);

	rc = sqlite3_prepare_v2(db, sql_query, query_length, &sql_statement,
							&zErrMsg);
	if (rc == SQLITE_OK) {
		printf("[get batch num] sql success for query\n");
		if ((rc = sqlite3_step(sql_statement)) == SQLITE_ROW) { 
			*is_full = check_full_batch(sqlite3_column_int(sql_statement,0),A_i);
			return sqlite3_column_int(sql_statement, 0);
		} else {
			printf("Error\n");
			return -1;
		}
	}
	free(sql_query);
}

int get_first_num_batch_by_date(int date, char *A_i, int *is_full)
{
	int rc = 0;
	sqlite3_stmt *sql_statement;
	const char *zErrMsg = 0;
	char *sql_query = (char*)malloc(1024);
	sprintf(sql_query, "SELECT batch_number from DATA_ENTRY \
      WHERE A_i = '%s' AND date = %d ORDER BY batch_number ASC LIMIT 1;",
			A_i, date);

	size_t query_length = strlen(sql_query);

	rc = sqlite3_prepare_v2(db, sql_query, query_length, &sql_statement,
							&zErrMsg);
	if (rc == SQLITE_OK) {
		printf("[get batch num] sql success for query\n");
		if ((rc = sqlite3_step(sql_statement)) == SQLITE_ROW) {
			*is_full = check_full_batch(sqlite3_column_int(sql_statement,0),A_i);
			return sqlite3_column_int(sql_statement, 0);
		}
		else {
			printf("Error\n");
			return -1;
		}
	}
	free(sql_query);
}

char *query_by_date(int date, char *A_i) {}

char *query_MERKLE_ENTRY(int merkle_field, char *A_i, int batch_number)
{
	int rc = 0;
	sqlite3_stmt *sql_statement;
	const char *zErrMsg = 0;
	char *s_q;
	char *sql_query = (char*)malloc(2048);

	/* Choose what field of the user to retrieve */
	switch (merkle_field) {
	case MERKLE_batch_number:
		s_q = "SELECT batch_number from MERKLE_ENTRY";
		break;
	case MERKLE_name:
		s_q = "SELECT name from MERKLE_ENTRY";
		break;
	case MERKLE_root_hash:
		s_q = "SELECT root_hash from MERKLE_ENTRY";
		break;
	case MERKLE_root_sig:
		s_q = "SELECT root_sig from MERKLE_ENTRY";
		break;
	default:
		break;
	}

	/* Add the where clause to specify what user, by A_i */
	sprintf(sql_query, "%s WHERE name = '%s' AND batch_number = %d;", s_q, A_i,
			batch_number);
	size_t query_length = strlen(sql_query);

	/* Copy the retrieved data, and return
	 *  ie. return the sym_key for the A_i that was passed
	 */
	char *result;
	size_t res_size;
	rc = sqlite3_prepare_v2(db, sql_query, query_length, &sql_statement,
							&zErrMsg);
	if (rc == SQLITE_OK) {
		if ((rc = sqlite3_step(sql_statement)) == SQLITE_ROW) {
			res_size = strlen(sqlite3_column_text(sql_statement, 0));
			result = (char*)malloc(res_size + 1);
			memcpy(result, sqlite3_column_text(sql_statement, 0), res_size);
			result[res_size] = '\0';
		}
		else {
			free(sql_query);
			return 0;
		}
	}
	free(sql_query);
	return result;
}

/**/
void clean_up()
{
	// printf("Cleaning up the database\n");
	sqlite3_close(db);
}

int callback(void *data, int argc, char **argv, char **azColName)
{
	printf("%s\n", (const char *)data);
	for (int i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i]);
	}
	return 0;
}
