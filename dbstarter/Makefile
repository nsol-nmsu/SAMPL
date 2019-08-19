CC=gcc
CFLAGS= -g -lm -lssl -lcrypto -lsqlite3

OBJDIR=../obj
SRCS=main.c
OBJS=$(shell find $(OBJDIR) -name '*.o')

TARGET=dbstarter

all: $(SRCS) $(OBJS)
	$(CC) $(SRCS) $(CFLAGS) $(OBJS) -o $(TARGET) 
	rm test.db


clean:
	rm *.o

