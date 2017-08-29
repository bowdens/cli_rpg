#include <stdio.h>
#include "game.h"
#include <libtalaris.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include "tomlib.h"
#include "glItemList.h"
#include "race.h"

#define ID_LEFT 1
#define ID_RIGHT 2
#define ID_FOWARD 3
#define ID_BACK 4
#define ID_PRINT 6
#define ID_A 7
#define ID_B 8
#define ID_C 9
#define ID_D 10
#define ID_CLEAR 11
#define ID_USE 12

void init_c(Commands *c){
	c = append_command_list(c, ID_LEFT, "left", "", "try to move left", COM_SHOWN);
	c = append_command_list(c, ID_LEFT, "l", "", "shorthand for left", COM_HIDDEN);
	c = append_command_list(c, ID_RIGHT, "right", "", "try to move right", COM_SHOWN);
	c = append_command_list(c, ID_RIGHT, "r", "", "shorthand for right", COM_HIDDEN);
	c = append_command_list(c, ID_FOWARD, "foward", "", "try to move foward", COM_SHOWN);
	c = append_command_list(c, ID_FOWARD, "f", "", "shorthand for foward", COM_HIDDEN);
	c = append_command_list(c, ID_BACK, "back", "", "try to move back", COM_SHOWN);
    c = append_command_list(c, ID_USE, "use", "", "use an item specified", COM_SHOWN);
	c = append_command_list(c, ID_PRINT, "print", "", "Prints the given argument. Eg 'print room'", COM_SHOWN);
	c = append_command_list(c, ID_A, "a", "", "selects the response A for dialogue", COM_HIDDEN);
	c = append_command_list(c, ID_B, "b", "", "selects the response B for dialogue", COM_HIDDEN);
	c = append_command_list(c, ID_C, "c", "", "selects the response C for dialogue", COM_HIDDEN);
	c = append_command_list(c, ID_D, "d", "", "selects the response D for dialogue", COM_HIDDEN);
	c = append_command_list(c, ID_CLEAR, "clear", "", "clears the console", COM_HIDDEN);
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

int any_equals(int c, char *source[c], char *comparison){
    for(int i = 0; i < c; i ++){
        if(!strcmp(source[i],comparison)) return 1;
    }
    return 0;
}

int prefix_equals(char *a, char *b){
    //printf("checking if the prefix of '%s' = '%s\n",a,b);
    if(strlen(a) < strlen(b)) return 0;
    for(unsigned int i= 0; i < strlen(a) && i < strlen(b); i ++){
        if(a[i]!=b[i]) return 0;
    }
    return 1;
}

int return_flag_argument(int argc, char **argv, char *flagPrefix){
    for(int i = 0; i < argc; i ++){
        if(prefix_equals(argv[i], flagPrefix)){
            //printf("prefixes match\n");
            return atoi(argv[i] + strlen(flagPrefix));
        }
    }
    return -1;
}

char *char_return_flag_argument(int argc, char **argv, char *flagPrefix){
    for(int i = 0; i < argc; i ++){
        if(prefix_equals(argv[i], flagPrefix)){
            return argv[i] + strlen(flagPrefix);

        }
    }
    return NULL;
}

int has_flag(int argc, char **argv, char flag){
    for(int i = 0; i < argc; i ++){
        if(argv[i][0] == '-'){
            //this one is flag
            for(int j = 1; j < (int)strlen(argv[i]) && argv[i][j] != '='; j ++){
                if(argv[i][j] == flag) return 1;
            }
        }
    }
    return 0;
}

int main(int argc, char **argv){
    verbose = has_flag(argc, argv, 'v');

    set_lt_verbose(return_flag_argument(argc, argv, "-ltVerbose="));

    if(verbose) printf("starting game\n");

    init_glItemList();
    if(verbose) printf(" - initialised item list\n");

    Commands *c = init_command_list();
    if(verbose) printf(" - initialised standard commands\n");

    init_c(c);
	if(verbose) printf(" - initialised custom commands\n");

	Arg *a = malloc(sizeof(Arg));
	assert(a);
	a->id = 0;
	if(verbose) printf(" - allocated argument memory\n");

	unsigned int seed = time(NULL);
    if(argc >= 2){
        //printf("argv[1] = %s\n",argv[1]);
        //printf("atoi(argv[1] = %d\n",atoi(argv[1]));
        int seed_num = return_flag_argument(argc, argv, "-seed=");
        if(verbose) printf("seed num from flags = %d\n",seed_num);
        if(seed_num > 0) seed = (unsigned int)seed_num;
    }

	printf("%sseed = %d\n",verbose?" - ":"> ",seed);
	srand(seed);
	//printf("srand(%d)\n",seed);

	Dungeon *d = generate_dungeon();
	if(verbose) printf(" - generated dungeon\n");
	Dungeon *start = d;
	if(verbose) printf(" - designated starting room\n");

    char *tempName = char_return_flag_argument(argc, argv, "-name=");
	if(verbose) printf(" - name from commandline read\n");

    if(tempName != NULL && strcmp(tempName, "RANDOMNAME") == 0){
        if(verbose) printf("generating random name\n");
        generate_monster_name(rand()%5 + 5, tempName);
        printf("Your name is '%s'\n",tempName);
    }

    if(verbose) printf(" - Generating player\n");
    Character *p = generate_player(tempName?tempName:NULL);
	//p->luck = SKILL_CAP;
    print_character(p);
    if(verbose) printf("Welcome %s\n",p->name);

    if(verbose) printf(" - Character generated\n");
	if(verbose) printf(" - This world has %d rooms\n", count_rooms(start, 0));
	while(1){
		printf("\n");
		if(a->id == 0 || a->id == ID_LEFT || a->id == ID_RIGHT || a->id == ID_FOWARD || (a->id == ID_BACK && d != start/*TODO change so it prints from depth =2*/)){
			print_room(d);
		}
        //printf("getting input\n");
		a = get_input(c,a);
		if(a == NULL){
            printf("ERROR: exiting due to NULL arguments\n");
            return 1;
        }
        //printf("input got\n");
		int id = a->id;

		//handle movement commands
		d = move(d, id);

		//handle other commands
		switch(id){
			case ID_PRINT :
				if(a->next){
                    Character *temp;
                    Race *tempR;
					if(strcmp(a->next->arg, "world") == 0){
						print_world(start, 0);
					}else if(strcmp(a->next->arg, "room") == 0){
						print_room(d);
					}else if(strcmp(a->next->arg, "inv") == 0){
						print_inventory(p->inventory);
                    }else if(strcmp(a->next->arg, "allitems") == 0){
                        print_glItems();
					}else if((temp = find_character(d->monsters, p, a->next->arg)) != NULL){
						print_character(temp);
					}else if((tempR = find_race(a->next->arg)) != NULL){
                        print_race(tempR);
                    }else{
						//there is no matching argument to be printed
						printf("That cannot be printed\nPlease choose from 'world', 'room', 'allitems', and 'inv', or a character name (such as %s)\n",p->name);
					}
				}else{
					printf("Please specify an argument eg 'print world'\n");
				}
				break;
            case ID_USE:
                //USE ITEM
                if(a->next == NULL || a->next->next == NULL){
                    printf("Usage: %s item (number or name), target (number or name)\n",a->arg);
                    break;
                }
                Inv *i = NULL;
                Character *target;
                if(is_num(a->next->arg)){
                    //printf("entered a number\n");
                    if(verbose) printf("finding item through index\n");
                    i = find_item_index(p->inventory->inv, atoi(a->next->arg) - 1);
                }else{
                    //printf("entered a str\n");
                    if(verbose) printf("finding item through name\n");
                    i = find_item(p->inventory->inv, a->next->arg);
                }
                if(is_num(a->next->next->arg)){
                    if(verbose) printf("finding character using index\n");
                    if(atoi(a->next->arg) == 0) target = p;
                    else target = find_character_index(d->monsters, p, atoi(a->next->next->arg));
                }else{
                    if(verbose) printf("finding character through name\n");
                    target = find_character(d->monsters, p, a->next->next->arg);
                }

                //printf("Target = %p\n",target);
                if(i == NULL){
                   printf("You are not carrying that item\n");//%s\n",is_num(a->next->arg)?"that many items":strcat(is_vowel(a->next->arg[0])?"an":"a",a->next->arg));
                   break;
                  // printf("You should not see this\n");
                }
                //printf("i is not NULL\n");
                if(target == NULL){
                    printf("target character does not exist\n");
                    break;
                    //printf("you should not see this\n");
                }
                //printf("target is not NULL\n");
                if(i != NULL && i->usef == NULL){
                    printf("This item cannot be used\n");
                    break;
                    printf("You should not see this\n");
                }
                //printf("i->usef is not NULL\n");

                if(target != p){
                    //if(verbose) printf("target is not self\n");
                    d->monsters->curr = target;
                    i->usef(i, d->monsters, p);
                }else{
                    //if(verbose) printf("target is self\n");
                    Charlist *pl = create_charlist();
                    pl->first = pl->last = pl->curr = p;
                    i->usef(i, pl, p);
                    free(pl);
                }
                break;
			case ID_A :
				if(d && d->monsters && d->monsters->first && d->monsters->first->dialogue &&  d->monsters->first->dialogue->optionAText[0] != '\0'){
					printf("%s\n",d->monsters->first->dialogue->optionAText);
					d->monsters->first->dialogue = d->monsters->first->dialogue->optionA;
					print_dialogue(d->monsters->first->dialogue, d->monsters->first->name);
				}else{
					printf("there is no option A\n");
				}
				break;
            case ID_B :
                if(d && d->monsters && d->monsters->first && d->monsters->first->dialogue && d->monsters->first->dialogue->optionBText[0] != '\0'){
                    printf("%s\n",d->monsters->first->dialogue->optionBText);
                    d->monsters->first->dialogue = d->monsters->first->dialogue->optionB;
                    print_dialogue(d->monsters->first->dialogue, d->monsters->first->name);
                }else{
                    printf("there is no option B\n");
                }
                break;
            case ID_C :
                if(d && d->monsters && d->monsters->first && d->monsters->first->dialogue && d->monsters->first->dialogue->optionCText[0] != '\0'){
                    printf("%s\n",d->monsters->first->dialogue->optionCText);
                    d->monsters->first->dialogue = d->monsters->first->dialogue->optionC;
                    print_dialogue(d->monsters->first->dialogue, d->monsters->first->name);
                }else{
                    printf("there is no option C\n");
                }
                break;
            case ID_D :
                if(d && d->monsters && d->monsters->first && d->monsters->first->dialogue && d->monsters->first->dialogue->optionDText[0] != '\0'){
                    printf("%s\n",d->monsters->first->dialogue->optionDText);
                    d->monsters->first->dialogue = d->monsters->first->dialogue->optionD;
                    print_dialogue(d->monsters->first->dialogue, d->monsters->first->name);
                }else{
                    printf("there is no option D\n");
                }
                break;
			case ID_CLEAR :
				system("clear");
				break;
		}
	}
}
