/*
 * Used to interact with ethereum from c
 * Calls are made using "system" calls for simplicity.
 *
 */


#ifndef eth_h
#define eth_h

/* Write a transaction to ethereum
 * data:  is what data to embed in the ethereum transaction
 * filename: is the name of the file to write the tx_hash
 */
int write_tx(char *data, char *filename);

/* Search ethereum for a transaction
 * Param takes a filename that contains the transaction hash
 * to be used to search
 */
char *search_tx(char *filename, char *return_file);




#endif
