#!/bin/bash

#PY_PATH=/usr/include/python3.6m/

# build the Judge Enforcment

gcc -o Judge main.c ../obj/logger.o ../obj/signal.o ../obj/crypto.o  ../obj/server.o ../obj/client.o ../obj/header.o ../obj/payload.o ../obj/packet.o  -lssl -lcrypto 
