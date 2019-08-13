#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "signal.h"

void start_signals()
{
	signal(SIGTERM, quit);
	signal(SIGINT, quit);
}

void quit()
{
	printf("Recieved signal, quitting...\n");
	exit(0);
}
