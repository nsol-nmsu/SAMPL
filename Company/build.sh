#!/bin/bash

# build the Company Enforcment

gcc -o Company  main.c ../obj/signal.o ../obj/stack.o ../obj/logger.o ../obj/payload.o ../obj/packet.o ../obj/server.o ../obj/client.o ../obj/header.o ../obj/database.o ../obj/crypto.o ../obj/token.o -lcrypto -lssl -lsqlite3
