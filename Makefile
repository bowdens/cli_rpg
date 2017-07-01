default: game

game: game.c gamef.c input.c input.h game.h
	gcc -o $@ game.c gamef.c input.c input.h game.h tomlib.c tomlib.h -lm -Wall -Wextra
