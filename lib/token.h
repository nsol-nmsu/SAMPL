/*
 * Tokenizer of strings, used to split payloads and to help
 * the Enforcer process the serialized data sent from
 * the Company
 */

#ifndef token_h
#define token_h

/* Copy the passed in string first to avoid corruping
 * any data passed in
 */
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
