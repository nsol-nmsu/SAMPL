#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

stack create_stack()
{
	stack n = (struct node *)malloc(sizeof(struct node));
	n->next = NULL;
	return n;
}

void push(stack s, char *data)
{
	stack n_node = (struct node *)malloc(sizeof(struct node));

	size_t len = strlen(data);
	n_node->data = (char*)malloc(len + 1);
	strncpy(n_node->data, data, len);
	n_node->data[len] = '\0';

	n_node->next = s->next;
	s->next = n_node;
}

char *pop(stack s)
{
	if (!s->next)
		return 0;
	stack to_delete;
	size_t len = strlen(s->next->data);
	char *data = (char*)malloc(len + 1);
	strcpy(data, s->next->data);
	data[len] = '\0';
	free(s->next->data);
	to_delete = s->next;
	s->next = s->next->next;
	free(to_delete);
	return data;
}

int free_stack(stack s)
{
	stack current = s;
	if (!current->next) {
		free(current);
		return 1;
	}
	// free(s);
	int c = 1;
	while (current) {
		free(current->data);
		s = current;
		current = current->next;
		free(s);
	}
	return 1;
}


// No error checking after malloc, assuming all is good
char *convert_to_string(stack s, int order)
{
	char *string_res = (char*)malloc(2048);
	memset(string_res, '\0', 2048);
	char *pop_res;
	size_t len_1 = 2048;
	size_t len_2 = 0;
	size_t len_3 = 0;

	while ((pop_res = pop(s)) != 0) {
		len_2 += strlen(pop_res);

		// if greater than available memory, realloc
		if ((len_2 + len_3) > len_1) {
			string_res = (char*)realloc(string_res, len_1 * 2);
			len_1 *= 2;
		}
		len_3 += len_2;
		sprintf(string_res, "%s %s", string_res, pop_res);
		free(pop_res);
	}

	// tokenize and reverse here ( not implemented )

	return string_res;
}

// use comma delimiter instead of space ( not used )
char *convert_to_string_no_space(stack s, int order)
{

	char *string_res = (char*)malloc(2048);
	memset(string_res, '\0', 2048);
	char *pop_res;
	size_t len_1 = 2048;
	size_t len_2 = 0;
	size_t len_3 = 0;

	while ((pop_res = pop(s)) != 0) {
		len_2 += strlen(pop_res);

		// if greater than available memory, realloc
		if ((len_2 + len_3) > len_1) {
			string_res = (char*)realloc(string_res, len_1 * 2);
			len_1 *= 2;
		}
		len_3 += len_2;
		sprintf(string_res, "%s,%s", string_res, pop_res);
		free(pop_res);
	}

	// tokenize and reverse here

	return string_res;
}
