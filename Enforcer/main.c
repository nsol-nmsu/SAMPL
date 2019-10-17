/*
 * Main for the Enforcer
 *
 */

#include "../lib/client.h"
#include "../lib/common.h"
#include "../lib/crypto.h"
#include "../lib/header.h"
#include "../lib/logger.h"
#include "../lib/packet.h"
#include "../lib/parser.h"
#include "../lib/payload.h"
#include "../lib/server.h"
#include "../lib/signal.h"
#include <stdio.h>
#include <time.h>

char **payload_fields;
unsigned char *sym_key_from_judge;

char *large_file_hash;
int dup_log_file;

FILE *f_file;

clock_t start, end; // timers

/*************************
 * Routines to handle the
 * specific tasks at each step
 ************************/

/*
 * save keys
 */
void f7()
{
	// save keys
	memcpy(sym_key_from_judge, payload_fields[2], 32);
}

/*
 * Write 10 to blockchain
 * Send 11 to Company
 */
void f9()
{
	printf("[Enforcer] Write to BC step 10\n");

	system("python3 lib/python/eth_calls/write_t.py company test-data tx-hash.txt");

	char *msg = build_packet(11, payload_fields, 0);
	send_msg(msg, COMPANYIP, COMPANYPORT);
}

/*
 * Write 14 to blockchain
 * Send 15 to Enforcer
 * recieve large data here to verify
 */
void f13()
{
	printf("[Enforcer] Write to BC step 14\n");

	 system("python3 lib/python/eth_calls/write_t.py company test-data enf-tx.txt");

	// char* file_name = "test-out.txt";
	char *file_name = "test_dummy.txt";
	//char *file_name = "../Company/test_dummy.txt";

	verify_file(file_name);
	fprintf(stderr,"[Enforcer] Finished verifying the file\n");

	char *msg = build_packet(15, payload_fields, 0);
	send_msg(msg, LAWIP, LAWPORT);
}

void set_file_hash(char *f_hash) { memcpy(large_file_hash, f_hash, 32); }

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
	// special targets to know a large file is comming
	if (target == 100) {
		printf("Got file meta data\n");
		fprintf(stderr, "[Enforcer] Got file meta data\n");
		set_file_hash(payload);
		return;
	}

	if (target == 101) {
		printf("Got a chunk of data: %d\n", atoi(_header.field1_len));
		fprintf(stderr, "[2>] got chunk of data\n");
		// FILE* f_file = fopen("test-out.txt","ab");
		fwrite(payload, atoi(_header.field1_len), 1, f_file);
		// fclose(fp);
		return;
	}

	payload_fields = split_payload(&_header, payload);

	switch (target) {
	case 1:
		printf("[Enforcer] recieved step 7\n");
		f7();
		break;
	case 2:
		printf("[Enforcer] recieved step 9\n");
		f9();
		break;
	case 3:
		printf("[Enforcer] recieved step 13\n");
		f13();
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
 *
 */
int main()
{
	start_signals();
	// f13();
	large_file_hash = malloc(33); // weird

	dup_log_file = dup_logger_init("Enforcer.log");
	dup2(dup_log_file,1);

	f_file = fopen("test-out.txt", "w");

	sym_key_from_judge = malloc(33);

	/* targets are what workflow numbers this is expecting to recieve */
	int targets[] = {7, 9, 13};
	start_server(ENFORCERPORT, f, targets, 0);
}
