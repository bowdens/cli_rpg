#include <stdio.h>
#include "game.h"
#include "input.h"

void init_c(Commands *c){
	c = append_command_list(c, ID_LEFT, "left", "", "try to move left");
	c = append_command_list(c, ID_RIGHT, "right", "", "try to move right");
	c = append_command_list(c, ID_FOWARD, "foward", "", "try to move foward");
	c = append_command_list(c, ID_BACK, "back", "", "try to move back");
}

int main(){
	Commands *c = init_command_list();
	init_c(c);
	Arg *a = malloc(sizeof(Arg));

	while(1){
		a = get_input(c,a);
		if(a == NULL) return 1;
		int id = a->id;

		switch(id){
			case ID_LEFT :
				printf("you move left\n");
			case ID_RIGHT :
                                printf("you move left\n");
			case ID_FOWARD :
                                printf("you move left\n");
			case ID_BACK :
                                printf("you move left\n");

		}	
	}
}
