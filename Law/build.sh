#!/bin/bash

#PY_PATH=/usr/include/python3.6m/

# build the Law Enforcment

gcc -g -o Law  main.c ../obj/stack.o ../obj/signal.o ../obj/crypto.o  ../obj/logger.o ../obj/server.o ../obj/client.o ../obj/header.o ../obj/packet.o ../obj/payload.o  -lssl -lcrypto 
