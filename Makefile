CC=gcc

CFLAGS=-Wall -Wextra -lm -ltalaris

default: game

game: game.c gamef.c game.h tomlib.c tomlib.h
	gcc -o $@ $^ -ltalaris -lm -Wall -Wextra
