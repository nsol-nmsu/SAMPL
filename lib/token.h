

#ifndef token_h
#define token_h

char **s_tokenize(const char *s, int *count);

/*
 * a Simple tokenizer
 * order specifies what order to return the tokens in,
 * used by stack
 *
 * count returns how many tokens are there
 */
char **tokenize(char *string, int *count, int order);

#endif
