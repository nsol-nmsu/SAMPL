#ifndef common_h
#define common_h

#define BUFF_SIZE 65000

// PORTS

#define JUDGEPORT 55511
#define COMPANYPORT 55522
#define ENFORCERPORT 55533
#define LAWPORT 55544

// IP'S

// remote testing
/*
#define JUDGEIP "127.0.0.1"
#define COMPANYIP "127.0.0.1"
#define ENFORCERIP "127.0.0.1"
#define LAWIP "127.0.0.1"
*/

// local testing
#define JUDGEIP "127.0.0.1"
#define COMPANYIP "127.0.0.1"
#define ENFORCERIP "127.0.0.1"
#define LAWIP "127.0.0.1"

// not needed
struct args {
	int port;
	void (*func)();
	char *targets;
};

#endif
