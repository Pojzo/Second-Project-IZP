CFLAGS=-std=c99 -Wall -Wextra -Werror
CC=gcc

setcal: setcal.c
	$(CC) $(CFLAGS) setcal.c -o setcal

run: 
	./setcal sets.txt
