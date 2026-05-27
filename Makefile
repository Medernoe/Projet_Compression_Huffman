CC=gcc
CFLAGS = -std=c11 -Wpedantic -Wall -Wconversion -Wextra -Wwrite-strings \
   -Werror -Wfatal-errors -O2

# 1. Ajoute compression.o aux dépendances et à la commande de liaison
main : main.o compression.o arbrebinaire.o lsc.o 
	${CC} main.o compression.o arbrebinaire.o lsc.o -o main

main.o : main.c
	${CC} -c main.c ${CFLAGS}

compression.o : compression.c
	${CC} -c compression.c ${CFLAGS}

arbrebinaire.o : arbrebinaire.c
	${CC} -c arbrebinaire.c ${CFLAGS}

lsc.o : lsc.c
	${CC} -c lsc.c ${CFLAGS}	