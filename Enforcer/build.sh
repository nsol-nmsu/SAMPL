#!/bin/bash

# build the Enforcer

gcc  -o Enforcer main.c  ../obj/stack.o ../obj/logger.o ../obj/token.o ../obj/merkle.o ../obj/parser.o ../obj/signal.o ../obj/crypto.o ../obj/payload.o ../obj/packet.o ../obj/server.o ../obj/client.o ../obj/header.o -lssl -lcrypto -lm
