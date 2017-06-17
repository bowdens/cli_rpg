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

int highest_dungeon_attrib(Dungeon *d){
	if(d->damage >= d->dinge && d->damage >= d->haunt && d->damage >=d->faith){
		return ATTRIB_DAMAGE;
	}else if(d->dinge > d->damage && d->dinge > d->haunt && d->damage > d->faith){
		return ATTRIB_DINGE
	}else if(d->haunt > d->damage && d->haunt > d->dinge && d->haunt > d->faith){
		return ATTRB_HAUNT;
	}else{
		return ATTRIB_FAITH;
	}
}

#define MAX_RM_SUBNAME 64
Clist create_clist(char text[64]){
	Clist *l = malloc(sizeof(Clist));
	assert(l);
	l->next = NULL;
	strcpy(l->text, text);
	return l;
}

void print_clist(Clist *l){
	printf("[");
	while(l){
		printf("%s",l->text);
		if(l->next){
			printf(",");
		}
	}
	printf("]");
}

int *clist_length(Clist *l){
	int c = 0;
	while(l){
		c ++;
		l = l->next;
	}
	return c;
}

Clist *read_subn(char *loc){
	FILE *f = fopen(loc, "r");
	char buffer[MAX_RM_SUBNAME] = {0};
	Clist *l;
	Clist *head;

	if(fgets(buffer, MAX_RM_SUBNAME, f)){
		l = create_clist(buffer);
		head = l;
	}else return NULL;

	while(fgets(buffer, MAX_RM_SUBNAME, f)){
		l->next = create_clist(buffer);
		l = l->next;
	}
	print_clist(head);
	return head;
}

void free_clist(Clist *l){
	Clist *temp;
	while(l){
		temp = l;
		l = l->next;
		free(temp);
	}
}

int *monster_over_level(Monsters *m, int level, char mName[MAX_CHARACTER_NAME-10]){
	//replaces the name with the monster with a max level over a specified 
	//use MAX_CHARACTER_NAME-10 since ", lair of " is 10 chars long
	char maxName = {0};
	int maxLevel = level - 1;
	int replaced = 0;
	while(m && m->monster){
		if(m->monster->level > maxLevel){
			strcpy(maxName, m->monster->name);
			maxLevel = m->monster->level;
			replaced = 1;
		}
	}
	strncpy(mName,maxName, MAX_CHARACTER_NAME-10);
	return replaced;
}

void generate_room_name(Dungeon *d, char name[MAX_ROOM_NAME]){
	int high = highest_dungeon_attrib(d);
	char prefix[MAX_ROOM_SUBNAME] = {0};
	char room[MAX_ROOM_SUBNAME] = {0};
	char suffix[MAX_ROOM_SUBNAME] = {0};
	
	Clist *pNames;
	Clist *rNames;
	Clist *sNames;
	int clistl = 0;
	int r = 0;
	switch(highest_dungeon_attrib(d)){
		case ATTRIB_DAMAGE:
			pNames = read_subn("damage_prefix.subn");
			break;
		case ATTRIB_DINGE:
			pNames = read_subn("dinge_prefix.subn");
			break;
		case ATTIRB_HAUNT:
			pNames = read_subn("haunt_prefix.subn");
			break;
		case ATTRIB_FAITH:
			pNames = read_subn("faith_prefix.subn");
			break;
	}
	//randomise prefix
	clistl = clist_length(pNames);
        r = rand()%clistl + 1;
        for(int i = 0; i < r && pNames; i ++) pNames = pNames->next;
        strcpy(prefix, pNames->text);
	strcat(prefix, " ");
	//room name
	rNames = read_subn("room_names.subn");
	clistl = clist_length(rNames);
	r = rand()%clistl + 1;
	for(int i = 0; i < r && rNames; i ++) rNames = rNames->next;
	strcpy(room, rNames->text);
	
	//suffix
	char mName[MAX_CHARACTER_NAME-10] = {0};
	if(monster_over_level(d->monsters, 30, mName)){
		//if there is a big monster
		strcpy(suffix, ", lair of ");
		strcat(suffix, mName);
	}

	strcpy(name, prefix);
	strcpy(name, room);
	strcpy(name, suffix);

	free_clist(pNames);
	free_clist(rNames);
	free_clist (sNames);
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
