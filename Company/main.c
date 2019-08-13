/*
 * Main for the Company
 */

#include "../lib/client.h"
#include "../lib/common.h"
#include "../lib/crypto.h"
#include "../lib/database.h"
#include "../lib/header.h"
#include "../lib/logger.h"
#include "../lib/packet.h"
#include "../lib/payload.h"
#include "../lib/server.h"
#include "../lib/signal.h"
#include "../lib/stack.h"
#include "../lib/token.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

char **payload_fields;

unsigned char *sym_key_from_judge;

int dup_log_file;
FILE *log_file;

clock_t start, end; // timers

/*************************
 * Routines to handle the
 * specific tasks at each step
 ************************/

/*
 * Recieve R_i from Law
 * Send corresponding A_i to Law
 */
void f3()
{

	/* split up the R_i passed in to process individually
	 * Use s_tokenize to not alter payload_fields[0]
	 */
	int count = 0;
	char **R_is = s_tokenize(payload_fields[0], &count);

	/* query the A_i for each R_i, push results to stack
	 * turn this stack to a string, make this string payload_fields[0]
	 */
	int status = open_db();

	stack query_stack = create_stack();
	char *query_res;

	for (int i = 0; i < count; i++) {
		if ((query_res = query_USER_ACCOUNT_R_i(USER_A_i, R_is[i])) == 0)
			printf("NO value\n");

		push(query_stack, query_res);
		free(query_res);
	}

	char *stack_string = convert_to_string(query_stack, 0);
	free_stack(query_stack);

	free(payload_fields[0]);
	payload_fields[0] = stack_string;

	char *msg = build_packet(4, payload_fields, 0);
	send_msg(msg, LAWIP, LAWPORT);
	free(msg);
	free_payload_fields(payload_fields);
	free_payload_fields(R_is);

	clean_up(); // close db
}

/*
 * save keys
 */
void f7()
{
	// save keys from judge
	memcpy(sym_key_from_judge, payload_fields[2], 32);
}

/*
 * Write 12 to blockchain
 * Send 13 to Enforcer
 * query large data here
 */
void f11()
{
	printf("[Company] Write to BC step 12\n");
	fprintf(stderr, "[Company] Doing query for data\n");
	//start = clock();
	//printf("C_START: %ld\n", start);

	/* Call ethereum here, python */
	//system("python3 lib/python/eth_calls/write_t.py company temp_data");

	/* Get the start and end date from the payload fields
	 */
	int date_count = 0;
	printf("\n\n[date company] %s\n", payload_fields[1]);
	char **date_strings = s_tokenize(payload_fields[1], &date_count);
	int start_date = atoi(date_strings[0]);
	int end_date = atoi(date_strings[1]);

	printf("[*] Date range for all: start: %d  end: %d\n", start_date,
		   end_date);

	int status = open_db();
	int count;
	/* This works */
	char **A_i_to_query = s_tokenize(payload_fields[0], &count);

	stack _stack = create_stack();
	for (int i = 0; i < count; i++)
		push(_stack, A_i_to_query[i]);

	FILE *out_file = fopen("test_dummy.txt", "w");

	int first_batch = 0;
	int last_batch = 0;

	printf("Query for batches, %d -> %d\n", first_batch, last_batch);
	
	// used ??
	int is_full = 0; //  merkle full?  1 == full :: 0 == not

	// loop all users, then loop each batch in range

	char *t_name;
	// count = how many users
	char *batch;
	for (int i = 0; i < count; i++) {
		t_name = pop(_stack);
		first_batch = get_first_num_batch_by_date(start_date, t_name, &is_full);
		last_batch = get_last_num_batch_by_date(end_date, t_name, &is_full);

		printf("[C Query] %s [batches] first: %d last: %d\n", t_name,
			   first_batch, last_batch);

		//////////////////////////////////////////////////////////
		for (int j = first_batch; j <= last_batch; j++) {
			open_db();
			printf("\t[*] %s batch: %d  ", t_name, j);
			//fprintf(stderr, "\t[2>] %s batch: %d \n", t_name, j);
			fflush(stdout);

			printf("AAAAAAA\n");
			batch = query_by_batch(j, t_name);
			printf("BBBBBBB\n");

			fwrite(batch, strlen(batch), 1, out_file);
			free(batch);
			clean_up();
		}
		//////////////////////////////////////////////////////////
		free(t_name);
	}

	// clean up
	fclose(out_file);
	// send_file(100,"test_dummy.txt",ENFORCERIP,ENFORCERPORT);
	char *msg = build_packet(13, payload_fields, 1);
	send_msg(msg, ENFORCERIP, ENFORCERPORT);
	clean_up();
	free(msg);
	free_payload_fields(payload_fields);
	free_payload_fields(A_i_to_query);
	free_payload_fields(date_strings);
	//end = clock();
	//printf("C_END: %ld\n", end);
	//printf("C_TIME: %f\n", ((double)(end - start) / CLOCKS_PER_SEC));
}

/*
 * This function will be called with the number that
 * corresponds to the workflow number. ( their index position in targets )
 * example targets = "135"
 *
 * if this function is called with 1, that means
 * that 1 is the workflow number,
 * if this function is called with 2, that means
 * that 3 is the workflow number
 *
 * Each chase corresponds to a routine to run
 * based off of the workflow number
 *
 * This function is passed the whole payload, and header
 * to for the info to disect the payload
 *
 */
void f(int target, struct header _header, char *payload)
{

	payload_fields = split_payload(&_header, payload);
	free(payload);

	switch (target) {
	case 1:
		printf("[Company]  recieved step 3\n");
		f3();
		break;
	case 2:
		printf("[Company]  recieved step 7(keys)\n");
		f7();
		break;
	case 3:
		printf("[Company]  recieved step 11\n");
		f11();
		break;
	default:
		printf("Error, no response to request\n");
		break;
	}
}

/*
 * targets is the workflow number that relates to
 * this entitiy. In reality, any time this entity recieves
 * a tcp connections, the first byte of the the message
 * should be one of these targets.
 *
 */
int main(int argc, char **argv)
{

	start_signals();
	sym_key_from_judge = malloc(33);

	// redirect stdio to file
	// dup_log_file = dup_logger_init("Company.log");
	// dup2(dup_log_file,1);

	payload_fields = malloc(1024);
	if (!payload_fields) {
		printf("Error with memory for payload fields\n");
		exit(0);
	}

	/*
	 * Company starting point
	 */

	/* targets are what workflow numbers this is expecting to recieve */
	int targets[] = {3, 7, 11};
	start_server(COMPANYPORT, f, targets, 0);
	// clean_up();
}
