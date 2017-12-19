CC=gcc

CFLAGS=-Wall -Wextra -lm -ltalaris -std=c11 -g

default: game

backup:
	cp race.c .race.c.bckup
	cp race.h .race.h.bckup
	cp game.c .game.c.bckup

restore_race:
	cp .race.c.bckup race.c
	cp .race.h.bckup race.h

restore_backup:
	cp .game.c.bckup game.c

game: game.c gamef.c game.h tomlib.c tomlib.h item.c item.h die.c die.h race.c race.h glItemList.c glItemList.h
	gcc -o $@ $^ -Wall -Wextra -lm -ltalaris -std=c11 -g

make_race: make_race.c game.h
