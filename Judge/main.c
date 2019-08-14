/*
 * Main for the Judge Enforcment
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
#include <stdio.h>

unsigned char *sym_key_from_judge;

FILE *log_file;
int dup_log_file;

char *original_SR; // from Law

/* payload fields 1->4 */
char **payload_fields;

/*************************
 * Routines to handle the
 * specific tasks at each step
 ************************/

/*****************************************
 * Send 2 to Law Agency
 */
void f1()
{

	char *msg = build_packet(2, payload_fields, 0);
	send_msg(msg, LAWIP, LAWPORT);
}

/******************************************
 * Write 6 to blockchain
 * Send keys 7 to ALL
 */
void f5()
{
	/* STEPS
	 * 1. generate keys to send
	 * 2. send keys and transaction hash
	 * 3. encrypt and write SO to blockchain
	 */
	// send keys here

	// gen sym key
	unsigned char *sym_key = malloc(33);
	gen_sym_key(sym_key, 32);
	// payload_fields[2] = (char*) sym_key;
	payload_fields[2] = malloc(33);
	strncpy(payload_fields[2], sym_key, 32);

	printf("[Judge]    Write to BC, step 6\n");
	//system("python3 lib/python/eth_calls/write_t.py judge test-data-is-here");

	char *msg = build_packet(7, payload_fields, 0);
	send_msg(msg, COMPANYIP, COMPANYPORT);
	msg = build_packet(7, payload_fields, 0);
	send_msg(msg, LAWIP, LAWPORT);
	msg = build_packet(7, payload_fields, 0);
	send_msg(msg, ENFORCERIP, ENFORCERPORT);
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

	/* split up the fields of the payload for easy use later
	 * supports emtpy fields, will fill up payload_fields with what is there
	 * now payload_fields can be used later
	 */
	payload_fields = split_payload(&_header, payload);
	// dump_payload_fields(payload_fields);
	// exit(0);

	printf("[target] %d\n", target);
	switch (target) {
	case 1:
		printf("[Judge]    recieved step 1\n");
		f1();
		break;
	case 2:
		printf("[Judge]    recieved step 5\n");
		f5();
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
int main(int argc, char **argv)
{
	start_signals();

	dup_log_file = dup_logger_init("Judge.log");
	dup2(dup_log_file,1);

	payload_fields = malloc(1024);
	if (!payload_fields) {
		printf("Error allocating memory.. quitting..\n");
		exit(0);
	}

	/* targets are what workflow numbers this is expecting to recieve */
	int targets[] = {1, 5};
	start_server(JUDGEPORT, f, targets, 0);
}
