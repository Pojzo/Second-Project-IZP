CFLAGS=-std=c99 -Wall -Wextra -Werror -g
CC=gcc

setcal: setcal.c
	$(CC) $(CFLAGS) setcal.c -o setcal

run: 
	./setcal sets

valgrind:
	valgrind -s --track-origins=yes ./setcal sets

valgrindplus:
	valgrind -s --track-origins=yes --leak-check=full ./setcal sets
