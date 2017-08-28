CC=gcc

CFLAGS=-Wall -Wextra -lm -ltalaris

default: game

game: game.c gamef.c game.h tomlib.c tomlib.h item.c item.h die.c die.h race.c race.h glItemList.c glItemList.h
	gcc -o $@ $^ -ltalaris -lm -Wall -Wextra -std=c11
