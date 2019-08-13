CC=gcc
CFLAGS= -g -lm -lssl -lcrypto -lsqlite3

OBJDIR=../obj
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)


all: $(OBJS)
	cp $(OBJS) $(OBJDIR)


clean:
	rm *.o

