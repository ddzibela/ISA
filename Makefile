#Projekkt ISA zima 2021
#athor Denis Dzíbela xdzibe00

CC = g++

CFLAGS = -Wall -Wextra -pedantic

.PHONY: build_debug clean pack

build: main.cpp
	$(CC) $(CFLAGS) $^ -o isa

build_debug: main.cpp
	$(CC) -g $(CFLAGS) $^ -o isa

clean:
	-rm ./isa

pack: