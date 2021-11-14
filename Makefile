#Projekkt ISA zima 2021
#athor Denis Dzíbela xdzibe00

CC = g++

CFLAGS = -Wall -Wextra -pedantic

.PHONY: build_debug clean pack

build: main.cpp client.cpp server.cpp
	$(CC) $(CFLAGS) $^ -o secret -lssl -lcrypto -lpcap

build_debug: main.cpp client.cpp server.cpp
	$(CC) -g $(CFLAGS) $^ -o secret -lssl -lcrypto -lpcap

clean:
	-rm ./secret

pack: