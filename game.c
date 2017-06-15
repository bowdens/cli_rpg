#include <stdio.h>
#include "game.h"
#include "input.h"
#include <stdlib.h>
#include <assert.h>
#include <time.h>

void init_c(Commands *c){
	c = append_command_list(c, ID_LEFT, "left", "", "try to move left");
	c = append_command_list(c, ID_RIGHT, "right", "", "try to move right");
	c = append_command_list(c, ID_FOWARD, "foward", "", "try to move foward");
	c = append_command_list(c, ID_BACK, "back", "", "try to move back");
	c = append_command_list(c, ID_PRINT, "print", "", "Prints the given argument. Eg 'print room'");
}

Dungeon *move(Dungeon *d, int moveId){
	switch(moveId){
		case ID_LEFT :
        		if(d->left){
        			printf("You move into the door on your left\n");
        			d = d->left;
        		}else{
        			printf("There is no door to your left\n");
        		}
        		break;
        	case ID_RIGHT :
        		if(d->right){
        			printf("You move into the door on your right\n");
        			d = d->right;
        		}else{
        			printf("There is no door to your right\n");
        		}
        		break;
        	case ID_FOWARD :
        	        if(d->foward){
        	                printf("You move into the door on your foward\n");
        	        	d = d->foward;
               		}else{
                		printf("There is no door to your foward\n");
                	}
                	break;
		case ID_BACK :
        		if(d->back){
        	        	printf("You move into the door on your back\n");
        			d = d->back;
        		}else{
				printf("There is no door to your back\n");
			}
			break;
	}
	return d;
}

int main(int argc, char **argv){
	printf("starting game\n");
	Commands *c = init_command_list();
	printf("initialised standard commands\n");
	init_c(c);
	printf("initialised custom commands\n");

	Arg *a = malloc(sizeof(Arg));
	assert(a);
	printf("allocated argument memory\n");

	unsigned int seed;
        if(argc >= 2){
                printf("argv[1] = %s\n",argv[1]);
                printf("atoi(argv[1] = %d\n",atoi(argv[1]));
                if(atoi(argv[1]) > 0){
                        seed = (unsigned int) atoi(argv[1]);
                        printf("seed = %d\n",seed);
                        srand(seed);
                }else{
                        srand(time(NULL));
                }
        }else{
                srand(time(NULL));
        }
	
	Dungeon *d = generate_dungeon();
	printf("generated dungeon\n");
	Dungeon *start = d;
	printf("designated starting room\n");

	while(1){
		printf("\n\n");
		print_room(d);
		a = get_input(c,a);
		if(a == NULL) return 1;
		int id = a->id;

		//handle movement commands
		d = move(d, id);

		//handle other commands
		switch(id){
			case ID_PRINT :
				if(a->next){
					if(strcmp(a->next->arg, "world") == 0){
						print_world(start, 0);
					}else if(strcmp(a->next->arg, "room") == 0){
						print_room(d);
					}else{
						//there is no matching argument to be printed
						printf("That cannot be printed\n");
					}
				}else{
					printf("Please specify an argument eg 'print world'\n");
				}

		}	
	}
}
