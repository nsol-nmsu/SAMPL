/*
 * Main for the Law Enforcment
 */

#include "../lib/client.h"
#include "../lib/common.h"
#include "../lib/crypto.h"
#include "../lib/header.h"
#include "../lib/logger.h"
#include "../lib/packet.h"
#include "../lib/payload.h"
#include "../lib/server.h"
#include "../lib/signal.h"
#include "../lib/stack.h"
#include "../lib/merkle.h"
#include "../lib/token.h"
#include "../lib/database.h"
#include <stdio.h>

/*
 * A place to hold info for the working SR/SO
 *
 * This means only one SR/SO can be processed at time,
 * other wise these values will be overwritten.
 *
 * This will keep data Such as working original SR/SO
 * for use later, instead of passing it back and forth
 * many times. Need to change this if wish to process
 * many SR at one
 */

/* the R_i for who to surviel */
/* will need a way to read in these names  */
//
// read in file here
//
char *original_SR; // read from file
char *original_SO;

/* hold the payload fields here on incoming connections
 * held globablly so there are easily accessable to create
 * the next order of packets
 */
char **payload_fields;

unsigned char *sym_key_from_judge;

FILE *log_file;
int dup_log_file;

/* start the process by sending of the SR
 */
void trigger_system() {}

/*************************
 * Routines to handle the
 * specific tasks at each step
 ************************/

/*
 * Send 3 to company
 */
void f2()
{
	char *msg = build_packet(3, payload_fields, 0);
	//dump_packet(msg);
	send_msg(msg, COMPANYIP, COMPANYPORT);
	free(msg);
	free_payload_fields(payload_fields);
}

/*
 * Send 5 to judge
 */
void f4()
{
	char *msg = build_packet(5, payload_fields, 0);
	send_msg(msg, JUDGEIP, JUDGEPORT);
	free(msg);
	free_payload_fields(payload_fields);
}

/*
 * Write 8 to blockchain
 * Send 9 to Enforcer
 */
void f7()
{
	printf("[Law]      Write to BC step 8\n");

	char *SR = malloc(strlen(payload_fields[0]) + strlen(payload_fields[1] + 10));
	sprintf(SR,"%s %s",payload_fields[0],payload_fields[1]);
	char *hash_SR = hash(SR);
	char *to_write = malloc(strlen(hash_SR) + 100);
	sprintf(to_write,"python lib/python/eth_calls/write_t.py company %s hash_SR.dat",hash_SR);

	system(to_write);

	memcpy(sym_key_from_judge, payload_fields[2], 32);
	free(payload_fields[2]);
	payload_fields[2] = NULL;

	char *msg = build_packet(9, payload_fields, 0);
	send_msg(msg, ENFORCERIP, ENFORCERPORT);
	free(msg);
	free_payload_fields(payload_fields);
}

/*
 * Write 16 to blockchain
 * System Finished
 */
void f15()
{

	system("python3 lib/python/eth_calls/write_t.py company test-data finished-tx.txt");

	printf("!!! Finished\n");
	fprintf(stderr, "[*] Finished, logs should be written\n");

	system("rm fin.lock");

	free_payload_fields(payload_fields);
}

/*
 * This function will be called with the number that
 * corresponds to the workflow number.
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
 */
void f(int target, struct header _header, char *payload)
{

	// if the packet is not in one packet, do NOT call split_payload
	// there will need to be special handling, writing to a file

	// if a special target is sent, write the payload to a file
	if (target == -1) {

		return; // quit to not process any more
	}

	payload_fields = split_payload(&_header, payload);
	free(payload);

	switch (target) {
	case 1:
		printf("[Law]      recieved step 2\n");
		// dump_payload_fields(payload_fields);
		f2();
		break;
	case 2:
		printf("[Law]      recieved step 4\n");
		f4();
		break;
	case 3:
		printf("[Law]      recieved step 7\n");
		f7();
		break;
	case 4:
		printf("[Law]      recieved step 15\n");
		f15();
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
 * targets in used as an individual char, ex.
 * "12" would mean there exists targets 1 and 2
 *
 * arg 1 = number of users
 * arg 2 = number of days
 * arg 3 = offset for same tests w/ different names ( 0 - 4 )
 */
int main(int argc, char **argv)
{

	if (argc < 4) {
		printf("[!] Need how many users to run\n");
		printf("[!] Need how many days to run\n");
		printf("[!] Need offset for names ( 0 - 4 )\n");
		exit(0);
	}

	int names_offset = atoi(argv[1]) * atoi(argv[3]);

	/* some initializing for the entitiy */
	start_signals();
	dup_log_file = dup_logger_init("Law.log");
	dup2(dup_log_file,1);
	sym_key_from_judge = malloc(33);
	payload_fields = malloc(1024);
	if (!payload_fields) {
		printf("Error with payload field malloc\n");
		exit(0);
	}

	int num_users = atoi(argv[1]);
	int start_date = 0;
	int end_date = start_date + atoi(argv[2]) - 1;
	printf("NUM_USERS: %d\n", num_users);
	printf("NUM_DAYS: %d\n", atoi(argv[2]));

	FILE *sr_in_file = fopen("SR_names.txt", "r");
	size_t len;
	char *c = malloc(100);
	stack _stack = create_stack();
	while ((getline(&c, &len, sr_in_file) != EOF) && num_users != 0) {
		printf("\nGetting name\n");
		if (names_offset > 0) {
			names_offset--;
		}
		else {
			len = strlen(c);
			c[len - 1] = '\0';
			push(_stack, c);
			num_users--;
		}
	}

	original_SR = convert_to_string(_stack, 0);
	free_stack(_stack);
	fclose(sr_in_file);

	/* targets are what workflow numbers this is expecting to recieve */
	int targets[] = {2, 4, 7, 15};

	char *og_SR = malloc(strlen(original_SR) + 1);
	char *date = malloc(100);

	/* SET THE DATE RANGE HERE */
	// strcpy(date,"0 3");
	sprintf(date, "0 %d", (atoi(argv[2])) - 1);
	strcpy(og_SR, original_SR);

	payload_fields[0] = original_SR;
	payload_fields[1] = date;
	payload_fields[2] = NULL;
	payload_fields[3] = NULL;

	char *msg = build_packet(1, payload_fields, 0);
	printf("[Law] is getting things started\n");
	send_msg(msg, JUDGEIP, JUDGEPORT);
	free(msg);
	free_payload_fields(payload_fields);

	start_server(LAWPORT, f, targets, 0);
}
