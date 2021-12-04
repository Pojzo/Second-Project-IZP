CFLAGS=-std=c99 -Wall -Wextra -Werror -g
CC=gcc

setcal: setcal.c
	$(CC) $(CFLAGS) setcal.c -o setcal

run: 
	./setcal sets

valgrind:
	valgrind -s --track-origins=yes ./setcal sets

valgrindp:
	valgrind -s --track-origins=yes --leak-check=full ./setcal sets

valgrindpp:
	valgrind -s --track-origins=yes --leak-check=full --show-leak-kinds=all ./setcal sets

test: test.c
	gcc test.c -o test

valgtest:
	valgrind -s --track-origins=yes --leak-check=full --show-leak-kinds=all ./test

kuchartest:
	py test.py setcal

debiltest:
	py setcal.py
