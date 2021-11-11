#Projekkt ISA zima 2021
#athor Denis Dzíbela xdzibe00

CC = g++

CFLAGS = -Wall -Wextra -pedantic

.PHONY: build_debug clean pack

build: main.cpp client.cpp
	$(CC) $(CFLAGS) $^ -o secret

build_debug: main.cpp client.hpp client.cpp
	$(CC) -g $(CFLAGS) $^ -o secret

clean:
	-rm ./secret

pack: