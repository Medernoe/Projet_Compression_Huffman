CC=gcc
CFLAGS = -std=c2x -Wall -Wconversion -Werror -Wextra -Wfatal-errors -Wpedantic -Wwrite-strings -O2

all : hcode unhcode

hcode : hcode.o compression.o arbrebinaire.o lsc.o utils.o
	${CC} hcode.o compression.o arbrebinaire.o lsc.o utils.o -o hcode

unhcode : unhcode.o arbrebinaire.o utils.o
	${CC} unhcode.o arbrebinaire.o utils.o -o unhcode

main : main.o compression.o arbrebinaire.o lsc.o hcode.o utils.o unhcode.o
	${CC} main.o compression.o arbrebinaire.o lsc.o utils.o -o main

main.o : main.c
	${CC} -c main.c ${CFLAGS}

compression.o : compression.c
	${CC} -c compression.c ${CFLAGS}

arbrebinaire.o : arbrebinaire.c
	${CC} -c arbrebinaire.c ${CFLAGS}

lsc.o : lsc.c
	${CC} -c lsc.c ${CFLAGS}	

utils.o : utils.c
	${CC} -c utils.c ${CFLAGS}

hcode.o : hcode.c
	${CC} -c hcode.c ${CFLAGS}

unhcode.o : unhcode.c
	${CC} -c unhcode.c ${CFLAGS}

clean :
	rm -f *.o main hcode unhcode