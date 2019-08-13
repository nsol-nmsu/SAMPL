

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "token.h"

// do not alter the string passed in
char **s_tokenize(const char *s, int *count)
{

	char *string = malloc(strlen(s) + 1);
	strcpy(string, s);

	int pos = 0;

	char **tokens = malloc(2048);
	char *token;
	if (!tokens) {
		printf("Error with tokenize malloc\n");
		exit(0);
	}

	token = strtok(string, " ");
	while (token != NULL) {
		tokens[pos] = token;
		pos++;
		token = strtok(NULL, " ");
	}

	*count = pos;
	tokens[pos] = NULL;

	// free(string);
	return tokens;
}

char **tokenize(char *string, int *count, int order)
{
	int pos = 0;

	char **tokens = malloc(2048);
	char *token;
	if (!tokens) {
		printf("Error with tokenize malloc\n");
		exit(0);
	}

	token = strtok(string, " ");
	while (token != NULL) {
		tokens[pos] = token;
		pos++;
		token = strtok(NULL, " ");
	}

	if (count)
		*count = pos;
	tokens[pos] = NULL;

	if (order == 1)
		return tokens;
	else if (order == 2) {
		/* reverse the tokens here */
		char **rev_tokens = malloc(2048);
		if (!rev_tokens) {
			printf("Error with rev_token malloc\n");
			exit(0);
		}
		for (int i = pos - 1, j = 0; i >= 0; i--, j++) {
			rev_tokens[j] = tokens[i];
		}
		rev_tokens[pos] = NULL;
		return rev_tokens;
	}
	else {
		printf("[*] Order must be 1 or 2\n");
		exit(0);
	}
}
