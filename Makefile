CC=gcc
CFLAGS = -std=c2x -Wall -Wconversion -Werror -Wextra -Wfatal -errors -Wpedantic -Wwrite -strings -O2


main : main.o compression.o
	${CC} main.o compression.o -o main

main.o : main.c
	${CC} -c main.c ${CFLAGS}

compression.o : compression.c
	${CC} -c compression.c ${CFLAGS}
