#include "game.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "tomlib.h"
#include <ctype.h>

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

int count_monsters(Monsters *m){
	int c = 0;
	while(m){
		c ++;
		m = m->next;
	}
	return c;
}

void print_monsters(Monsters *m){
	while(m){
		if(m->monster){
			printf("%s\t(level %d)\n\t%.1lf/%.1lf\n",m->monster->name, m->monster->level,m->monster->life, m->monster->lifeTotal);
		}
		m = m->next;
	}	
}

void print_room(Dungeon *d){
	printf("You are in a "C_G"%s"C_W"\n",d->name);
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
		printf("\nThere are %d "C_R"monsters"C_W" in this room, they are:\n", count_monsters(d->monsters));
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

char ret_c(){
	char a = rand()%25 + 'A';
	if(is_vowel(a)){
		return ret_c();
	}else{
		//printf("%c",a);
		return a;
	}
}

char ret_v(){
	int a = rand()%5;
	switch(a){
		case 0:
			return 'A';
		case 1:
			return 'E';
		case 2:
			return 'I';
		case 3:
			return 'O';
		case 4:
			return 'U';
	}
	return 'A';
}

void generate_monster_name(int length, char str[length]){
	for(int i = 0; i < length; i ++){
		if(i%2){
			str[i] = tolower(ret_c());
		}else{
			if( i == 0){
				str[i] = ret_v();
			}else{
				str[i] = tolower(ret_v());
			}
		}
		//printf("i = %d, str[%d] = %c = %d\n",i, i, str[i], str[i]);
	}
	str[length] = '\0';
	//return str;
}

Character *generate_monster(){
	Character *m = malloc(sizeof(Character));
	assert(m);

	//name
	generate_monster_name(rand()%5 + 3, m->name);
	//printf("Monster name generated: %s\n",m->name);
	int r1 = rand()%8 + 1;
	int r2 = rand()%8 + 1;
	int r3 = rand()%7;
	m->level = r1 * r2 + r3; //generates a random number between 0 and 70, with a bias to lower numbers
	//printf("Monster %s's level = %d\n",m->name, m->level);
	m->lifeTotal = m->level * 10 + rand()%10;
	m->life = m->lifeTotal;
	m->inventory = NULL;
	return m;
}

char generate_room_name(Dungeon *d, char name[MAX_ROOM_NAME]){
	
}

#define ROOM_PREFIX_NUM 49
#define ROOM_NAME_NUM 19
#define ROOM_SUFFIX_NUM 15

Nameholder *generate_dungeon_name(){
	Nameholder *sa = malloc(sizeof(Nameholder));
		
	char rNames[ROOM_NAME_NUM][64] = {{0}};
	strcpy(rNames[0], "dungeon");
	strcpy(rNames[1], "cellar");
	strcpy(rNames[2], "cave");
	strcpy(rNames[3], "room");
	strcpy(rNames[4], "hallway");
	strcpy(rNames[5], "throne room");
	strcpy(rNames[6], "tunnel");
	strcpy(rNames[7], "chapel");
	strcpy(rNames[8], "basement");
	strcpy(rNames[9], "cavern");
	strcpy(rNames[10], "mine shaft");
	strcpy(rNames[11], "bridge");
	strcpy(rNames[12], "burrow");
	strcpy(rNames[13], "chamber");
	strcpy(rNames[14], "crypt");
	strcpy(rNames[15], "vault");
	strcpy(rNames[16], "grotto");
	strcpy(rNames[17], "den");
	strcpy(rNames[18], "passage");
		
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
	strcpy(dNames[8], " of contempt");
	strcpy(dNames[9], " of cruelty");
	strcpy(dNames[10], " of wrath");
	strcpy(dNames[11], " of death");
	strcpy(dNames[12], " of life");
	strcpy(dNames[13], " of horror");
	//monster suffix
	int length = 3 + rand()%5;
	char str[length];
	for(int i = 0; i < length; i ++) str[i] = '\0';
	generate_monster_name(length, str);
	char str2[64] = " of \0";
	strcat(str2, str);
	strcpy(dNames[ROOM_SUFFIX_NUM - 1], str2);
	
	char pNames[ROOM_PREFIX_NUM][64] = {{0}};
	strcpy(pNames[0], "cold ");
	strcpy(pNames[1], "dark ");
	strcpy(pNames[2], "stone ");
	strcpy(pNames[3], "quiet ");
	strcpy(pNames[4], "whispering ");
	strcpy(pNames[5], "shabby ");
	strcpy(pNames[6], "grimy ");
	strcpy(pNames[7], "seedy ");
	strcpy(pNames[8], "dim ");
	strcpy(pNames[9], "wailing ");
	strcpy(pNames[10], "muted ");
	strcpy(pNames[11], "brown ");
	strcpy(pNames[12], "white ");
	strcpy(pNames[13], "vile ");
	strcpy(pNames[14], "loathsome ");
	strcpy(pNames[15], "acrid ");
	strcpy(pNames[16], "pungent ");
	strcpy(pNames[17], "burning ");
	strcpy(pNames[18], "frozen ");
	strcpy(pNames[19], "icy ");
	strcpy(pNames[20], "frigid ");
	strcpy(pNames[21], "hellish ");
	strcpy(pNames[22], "sweltering ");
	strcpy(pNames[23], "blazing ");
	strcpy(pNames[24], "fiery ");
	strcpy(pNames[25], "heated ");
	strcpy(pNames[26], "rustling ");
	strcpy(pNames[27], "howling ");
	strcpy(pNames[28], "creaking ");
	strcpy(pNames[29], "damaged ");
	strcpy(pNames[30], "fractured ");
	strcpy(pNames[31], "evil ");
	strcpy(pNames[32], "ravaged ");
	strcpy(pNames[33], "narrow ");
	strcpy(pNames[34], "expansive ");
	strcpy(pNames[35], "echoing ");	
	strcpy(pNames[36], "large ");
	strcpy(pNames[37], "prison ");
	strcpy(pNames[38], "torture ");
	strcpy(pNames[39], "abandoned ");
	strcpy(pNames[40], "claustrophobic ");
	strcpy(pNames[41], "slimy ");
	strcpy(pNames[42], "damp ");
	strcpy(pNames[43], "unused ");
	strcpy(pNames[44], "defunct ");
	strcpy(pNames[45], "lifeless ");
	strcpy(pNames[46], "breathless ");
	strcpy(pNames[47], "drab ");
	strcpy(pNames[48], "faded ");

	char prefix[64] = {0};
	strcpy(prefix, pNames[rand()%ROOM_PREFIX_NUM]);
	if(rand()%4 == 0){
		prefix[0] = '\0';
	}

	char room[64] = {0};
	strcpy(room, rNames[rand()%ROOM_NAME_NUM]);
	char suffix[64] = {0};
	strcpy(suffix, dNames[rand()%ROOM_SUFFIX_NUM]);
	if(rand()%2 == 0){
		suffix[0] = '\0';
	}
	char *name = strcat(prefix, room);
	name = strcat(name, suffix);
	strcpy(sa->name, name);
	return sa;	
}

Monsters *generate_monster_list(){
	Monsters *ml = malloc(sizeof(Monsters));
	ml->monster = generate_monster();
	ml->next = NULL;
	return ml;
}

Monsters *generate_many_monsters(int num){
	if(num < 1) return NULL;
	Monsters *ml = generate_monster_list();
	Monsters *head = ml;
	for(int i = 1; i < num; i ++){
		ml->next = generate_monster_list();
		ml = ml->next;
	}
	ml->next = NULL;
	return head;
}

Dungeon *create_room(){
	//printf("creating a room\n");
	Dungeon *d = malloc(sizeof(Dungeon));
	assert(d);
	
	d->damage = (rand()%10000)/100.0;
	d->dinge = (rand()%10000)/100.0;
	d->haunt = (rand()%10000)/100.0;
	d->faith = (rand()%10000)/100.0;

	d->inventory = NULL;
	//generate_inventory();
	d->inventory = generate_inventory();
	char s[MAX_ROOM_NAME] = "nothing";
	strcpy(s,generate_dungeon_name()->name);
	strcpy(d->name, s);
	d->monsters = generate_many_monsters(rand()%4);
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
