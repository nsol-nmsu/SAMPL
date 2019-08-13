/*
 * Simple stack to use for building a string
 * to send over socket.
 *
 * Values are malloc'd and copied to their
 * own memory to avoid losing the data
 *
 * Relies on the tokenizer
 */

#ifndef stack_h
#define stack_h

struct node {
	char *data;
	struct node *next;
};

typedef struct node *stack;

stack create_stack();

void push(stack s, char *data);

char *pop(stack s);

int free_stack(stack s);

/*
 * Given a stack, build a string of its contents
 *
 * !!! ORDER IS NOT IMPLEMENTED
 *
 * The order returned would be
 *
 * Stack:
 * A
 * B
 * C
 *
 * String ( order = 1 ):
 * CBA
 *
 * String ( order = 2):
 * ABC
 */
char *convert_to_string(stack s, int order);

char *convert_to_string_no_space(stack s, int order);

#endif
