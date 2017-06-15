#include "game.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


#define ROOM_CO 3

#define C_R "\x1b[31m"
#define C_G   "\x1b[32m"
#define C_Y  "\x1b[33m"
#define C_B    "\x1b[34m"
#define C_M "\x1b[35m"
#define C_C "\x1b[36m"
#define C_W   "\x1b[0m" 

typedef struct nameholder{
	char name[MAX_ROOM_NAME];
} Nameholder;

int count_rooms(Dungeon *d, int c){
	//printf("count rooms called\n");
	if(d == NULL) return 0;
	c = 1;
	c += count_rooms(d->left, c);
	c += count_rooms(d->right, c);
	c += count_rooms(d->foward, c);
	return c;
}

void pr_i(int in){
	for(int i = 0; i < in; i ++){
		if(i%4 == 0){
			printf("|");
		}else{
			printf(" ");
		}
	}
}

void print_world(Dungeon *d, int in){
	if(d == NULL) return;
	//if(in == 0) printf("number of rooms = %d\n",count_rooms(d, 0));
	
	//pr_i(4*in);
	printf(C_G "%s "C_W"\n",d->name);
	
	if(d->left){
		pr_i(4*(in+1));
		printf("L:");
		print_world(d->left, in + 1);
	}

	if(d->right){
		pr_i(4*(in+1));
		printf("R:");
		print_world(d->right, in + 1);
	}

	if(d->foward){
		pr_i(4*(in+1));
		printf("F:");
		print_world(d->foward, in + 1);
	}
	if(in == 0) printf("number of rooms = %d\n", count_rooms(d,0));
}

void print_monsters(Monsters *m){
	while(m){
		if(m->monster){
			printf("%s\n",m->monster->name);
		}
		m = m->next;
	}	
}

void print_room(Dungeon *d){
	printf("You are in "C_G"%s"C_W"\n",d->name);
	if(d->back == NULL){
		printf("To your back there is no exit. ");
	}else{
		printf("Behind you is a door from where you just came. ");
		printf("\n" C_C"Back" C_W":\t" C_G "%s" C_W,d->back->name);
	}	
	if(d->foward){
		printf("\n"C_M"Foward"C_W ":\t"C_G"%s" C_W, d->foward->name);
	}
	
	if(d->left){
		printf("\n"C_Y"Left"C_W ":\t" C_G "%s" C_W, d->left->name);
	}

	if(d->right){
		printf("\n"C_B"Right" C_W ":\t" C_G "%s" C_W, d->right->name);
	}
	
	if(d->monsters == NULL){
		printf("\nThere are no "C_R"monsters"C_W"\n");
	}else{
		printf("\nThere are "C_R"monsters"C_W" in this room, they are:\n");
		print_monsters(d->monsters);
	}
}


Inv *generate_inventory(){
	Inv *i = malloc(sizeof(Inv));
	assert(i);

	strcpy(i->name, "testname");
	strcpy(i->desc, "testdesc");
	i->quantity = 2;
	i->type = TYPE_SWORD;
	i->effect = 1.2;
	i->next = NULL;
	return i;
}

#define ROOM_PREFIX_NUM 5
#define ROOM_NAME_NUM 5
#define ROOM_SUFFIX_NUM 8

char *generate_monster_name(){
	
}

Nameholder *generate_dungeon_name(){
	//printf("called generate dungeon name\n");
	Nameholder *sa = malloc(sizeof(Nameholder));
	//printf("nh = %p\n",sa);
	//printf("name holder malloced, asserting\n");
	
	char rNames[ROOM_NAME_NUM][64] = {{0}};
	strcpy(rNames[0], "dungeon");
	strcpy(rNames[1], "cellar");
	strcpy(rNames[2], "cave");
	strcpy(rNames[3], "room");	
	strcpy(rNames[4], "hallway");
	
	//printf("rNames successfully made\n");

	char dNames[ROOM_SUFFIX_NUM][64] = {{0}};
	//printf("dNames created\n");
	strcpy(dNames[0], " of despair");
	strcpy(dNames[1], " of misery");
	strcpy(dNames[2], " of luck");
	strcpy(dNames[3], " of emptiness");
	strcpy(dNames[4], " of nothing");
	strcpy(dNames[5], " of plenty");
	strcpy(dNames[6], " of monsters");
	strcpy(dNames[7], " of faith");
	//printf("dNames successfully made\n");
	
	char pNames[ROOM_PREFIX_NUM][64] = {{0}};
	//printf("pNames created\n");
	strcpy(pNames[0], "cold ");
	strcpy(pNames[1], "dark ");
	strcpy(pNames[2], "stone ");
	strcpy(pNames[3], "quiet ");
	strcpy(pNames[4], "whispering ");
	//printf("pNames successfully made\n");


	char prefix[64] = {0};
	strcpy(prefix, pNames[rand()%ROOM_PREFIX_NUM]);
	//printf("prefix made\n");

	if(rand()%2 == 0){
		prefix[0] = '\0';
		//printf("prefix set to 0\n");
	}else{
		//printf("Prexif kept\n");
	}

	char room[64] = {0};
	//printf("room created\n");
	strcpy(room, rNames[rand()%ROOM_NAME_NUM]);
	//printf("copied room\n");

	char suffix[64] = {0};
	//printf("suffix init\n");
	strcpy(suffix, dNames[rand()%ROOM_SUFFIX_NUM]);
	
	//printf("suffix made\n");
	if(rand()%2 == 0){
		suffix[0] = '\0';
		//printf("suffix set to 0\n");
	}else{
		//printf("suffix kept\n");
	}

	char *name = strcat(prefix, room);
	//printf("    one succesful strcat");
	name = strcat(name, suffix);
	//printf("    two succesful strcat");
	
	strcpy(sa->name, name);
	return sa;	
}

Dungeon *create_room(){
	//printf("creating a room\n");
	Dungeon *d = malloc(sizeof(Dungeon));
	assert(d);
	//printf("    allocated memory\n");
	
	d->inventory = NULL;
	//printf("    d->inventory set to NULL\n");
	generate_inventory();
	//printf("    generate inventory called experimentally\n");
	d->inventory = generate_inventory();
	//printf("    generated inventory\n");
	
	char s[MAX_ROOM_NAME] = "nothing";
	//printf("    s = %s\n",s);
	strcpy(s,generate_dungeon_name()->name);
	//printf("    generated dungeon name = %s\n",s);
	strcpy(d->name, s);
	//printf("    copied s into d->name\n");
	//printf("    generated name\n");

	d->monsters = NULL;
	//printf("    generated monsters\n");
	
	return d;
}

Dungeon *generate_room(Dungeon *d, Dungeon *back){
	//recursively generates a dungeon
	if(d == NULL) return NULL;
	
	d->back = back;
	
	//create a left door
	int r = rand()%ROOM_CO;	
	if(r == 0){
		//printf("creating door to the left\n");
		d->left = generate_room(create_room(), d);
	}else{
		d->left = NULL;
	}
	
	//create a right door
	r = rand()%ROOM_CO;
        //printf("r = %d\n",r);
        if(r == 0){
		//printf("creating door to the right\n");
		d->right = generate_room(create_room(), d);
	}else{
		d->right = NULL;
	}
	
	//create a foward door
	r = rand()%ROOM_CO;
        //printf("r = %d\n",r);
        if(r == 0){
		//printf("creating door to the front\n");
		d->foward = generate_room(create_room(), d);
	}else{
		d->foward = NULL;
	}
	return d;
}

//initialise the dungeon
Dungeon *generate_dungeon(){
	//unsigned int seed;
	//printf("in generate_dungeon, seed = %d\n",seed);
	//srand(seed);
	Dungeon *d = create_room();
	d->back = NULL;
	d->left = generate_room(create_room(), d);
	d->right = generate_room(create_room(), d);
	d->foward = generate_room(create_room(), d);
	return d;
}

//initiialise the player
Character *generate_player(){
	Character *p = malloc(sizeof(Character));
	assert(p);

	p->inventory = malloc(sizeof(Inv));
	p->lifeTotal = 100;
	p->life = 90;
	p->intelligence = 1;
	p->strength = 10;
	p->charisma = 5;
	p->luck = 9;
	strcpy(p->name, "tom");

	return p;
}
