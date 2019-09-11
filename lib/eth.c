
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "eth.h"


int write_tx(char *data, char *filename)
{
	
}

char *search_tx(char *filename, char *return_file)
{
	char *to_write = malloc(1028);
	sprintf(to_write,"python lib/python/eth_calls/search_tx.py company %s %s",filename,return_file);
	fprintf(stderr,"to_write > %s\n",to_write);
	system(to_write);

	while( access(return_file,F_OK) == -1 ) {
		fprintf(stderr,"Waiting for python to find ethereum transaction\n");
		sleep(1);
	}

	//FILE *fp=

}
