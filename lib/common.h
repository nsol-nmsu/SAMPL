/*
 * Some common traits to be shared
 *
 * When running SAMPL protocol on multiple machines,
 * modify the port's and ip's here. Each entitiy knows to use
 * the values provided here.
 *
 */


#ifndef common_h
#define common_h

#define BUFF_SIZE 65000

// PORTS

#define JUDGEPORT 55511
#define COMPANYPORT 55522
#define ENFORCERPORT 55533
#define LAWPORT 55544

// IP'S


// change me to run on different machines
#define JUDGEIP "127.0.0.1"
#define COMPANYIP "127.0.0.1"
#define ENFORCERIP "127.0.0.1"
#define LAWIP "127.0.0.1"


#endif
