#!/bin/bash


if [ "$1" = "-c" ]; then
	rm *.txt
	rm p
	exit
fi

if [ "$1" = "-r" ]; then
	echo "[!] REMOVING DATABASE as well"
	rm test.db
else
	echo "[*] Only building, NOT cleaning db"
	echo "[*] Run with options -r to clean old database ( recommended )"
fi



OBJ="../obj/stack.o ../obj/merkle.o ../obj/crypto.o"

gcc -g  -o dbstarter main.c $OBJ -lsqlite3 -lcrypto -lssl -lm
