/**************************************************************
 * parser.h
 *
 * The enforcer uses verify_file to process the company's 
 * collected data for the law
 *
 * No personal information is in the file, only the need 
 * information to complete the zero knowledge proof, and 
 * corresponding signatures, and the encrypted content.
 *
 * The file that is processed is generated in one of two ways:
 * 1. In local tests, where the company writes all this out
 * 	to a file for the enforcer to verify.
 * 2. Or, the file the enforcer generates by packets it 
 * 	receives from the company.
 *
 *
 ************************************************************/

#ifndef parser_h
#define parser_h

/*
 * read a file, parsing out the information, and verify
 * that information
 */
int verify_file(char *file_name);

#endif
