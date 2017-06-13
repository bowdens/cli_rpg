#include "game.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

Inv *generate_inventory(){
	Inv *i = malloc(sizeof(Inv));
	strcpy(i->name, "testname");
	strcpy(i->desc, "testdesc");
	i->quantity = 2;
	i->type = TYPE_SWORD;
	i->effect = 1.2;
	i->next = NULL;
}

Dungeon *create_room(){
	Dungeon *d = malloc(sizeof(Dungeon));
	assert(d);
	d->inventory = generate_inventory();
	strcpy(d->name, "room");
	d->monsters = NULL;
}

Dungeon *generate_room(Dungeon *d, Dungeon *back){
	//recursively generates a dungeon
	if(d == NULL) return NULL;
	
	d->back = back;
	
	//create a left door
	if(rand()%4 == 0){
		d->left = generate_room(create_room(), d);
	}else{
		d->left = NULL;
	}
	
	//create a right door
	if(rand()%4 == 0){
		d->right = generate_room(create_room(), d);
	}else{
		d->right == NULL;
	}
	
	//create a foward door
	if(rand()%4 == 0){
		d->foward = generate_room(create_room(), d);
	}
	return d;
}

//initialise the dungeon
Dungeon *generate_dungeon(){
	Dungeon *d = generate_room(create_room(), NULL);
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
