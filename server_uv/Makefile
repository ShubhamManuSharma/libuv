#!/bin/bash

run:	uv_server.o 
	gcc -o run uv_server.o -luv  -lpcap

uv_server.o: uv_server.c declaration.h headers.h
	gcc -c uv_server.c 
