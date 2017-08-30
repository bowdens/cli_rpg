#include "game.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "tomlib.h"
#include <ctype.h>
#include "item.h"
#include "die.h"
#include "race.h"
#include "glItemList.h"

// 1/ROOM_CO chance of generating a room to the left/right/foward
#define ROOM_CO 3

//global itemlist
void init_glItemList(void){
    append_glItemList(create_inv_element("Rusty Sword", "Rusty Swords", "At least its better than your hands", 1, item_sword(), 4.8, 5,  use_sword));
    append_glItemList(create_inv_element("Potion of Minor Healing", "Potions of Minor Healing", "A faintly glowing blue liquid", 1, item_hPotion(), 12, 20, use_potionh));
    append_glItemList(create_inv_element("Claw","Claws","They look like they could do some damage",1,item_melee(), 5, 3, use_sword));
    append_glItemList(create_inv_element("Steel Sword", "Steel Swords", "Its sharp and in pretty good condition",1, item_sword(), 9.2, 45, use_sword));
    append_glItemList(create_inv_element("Potion of Major Healing", "Potions of Major Healing", "A glowing blue liquid", 1, item_hPotion(), 40, 50, use_potionh));
    append_glItemList(create_inv_element("Bronze Knife","Bronze Knives","A small but sharp weapon",1,item_sword(),5,10,use_sword));
    if(verbose){
        printf("item list is: ");
        print_glItems();
        printf("\n");
    }
}

typedef struct nameholder{
	char name[MAX_ROOM_NAME];
} Nameholder;

//prototype for highest_room_attrib()
int highest_dungeon_attrib(Dungeon *d, double minAttrib);

void free_Character(Character *c){
    while(c){
        Character *temp = c;
        free(temp);
        c = c->next;
    }
}

void free_Charlist(Charlist *l){
    free(l->first);
    free(l);
}

void safe_exit(int exit_status){
    printf("freeing all memory locations and exiting");
    exit(exit_status);
}

int count_rooms(Dungeon *d, int c){
	//printf("count rooms called\n");
	if(d == NULL) return 0;
	c = 1;
	c += count_rooms(d->left, c);
	c += count_rooms(d->right, c);
	c += count_rooms(d->foward, c);
	return c;
}

int room_depth(Dungeon *d){
    if(d == NULL) return 0;
    int count = 1;
    while(d->back != NULL){
        d = d->back;
        count ++;
    }
    return count;
}

void pr_i(int in){
	for(int i = 0; i < in; i ++){
		if(i%2 == 0){
			printf("|");
		}else{
			printf(" ");
		}
	}
}

void print_character(Character *p){
	printf(C_Y"%s"C_W" (%s) %.1lf/%.1lf\n",p->name,p->race->name, p->life, p->lifeTotal);
	printf("\t"C_G"Intelligence"C_W": "C_Y"%.1lf\n"C_W,p->intelligence);
	printf("\t"C_G"Strength"C_W": "C_Y"%.1lf\n"C_W,p->strength);
	printf("\t"C_G"Speed"C_W": "C_Y"%.1lf\n"C_W,p->speed);
	printf("\t"C_G"Charisma"C_W": "C_Y"%.1lf\n"C_W,p->charisma);
	printf("\t"C_G"Luck"C_W": "C_Y"%.1lf\n"C_W,p->luck);
	printf("%s is carrying:\n", p->name);
	print_inventory(p->inventory);
}

int item_exists(Inv *i, char *name){
    if(name == NULL) return 0;
    for(Inv *t = i; t != NULL; t = t->next){
        if(strcmp(name, t->name) == 0) return 1;
    }
    return 0;
}

Character *find_character_index(Charlist *ml, Character *p, int x){
    if(ml == NULL && p == NULL) return NULL;
    if(x == 0) return p;
    if(ml == NULL) return NULL;

    Character *t = ml->first;
    for(int i = 1; i < x; i ++){
        if(t == NULL){
            if(verbose) printf("Could not find character with index %d\n",x);
            return NULL;
        }
        t = t->next;
    }
    if(t == NULL){
        if(verbose) printf("Could not find character with index %d\n",x);
        return NULL;
    }
    //printf("returning %s\n",t?t->name:"NULL");
    return t;
}

Character *find_character(Charlist *ml, Character *p, char *name){
    if(name == NULL) return NULL;
    if(p != NULL && strcmp(p->name, name) == 0) return p;
    if(ml == NULL) return NULL;
    Character *t = ml->first;
    while(t){
        //printf("t = %p\n, t->name = %s, name = %s\n",t,t->name,name);
        if(strcmp(t->name,name) == 0){
            //printf("returning %s\n",t->name);
            return t;
        }
        t = t->next;
    }
    if(verbose) printf("Could not find character with name == '%s'\n",name);
    return NULL;
}

Inv *find_item_index(Inv *i, int x){
    Inv *t = i;
    for(int a = 0; a < x; a ++){
        if(t == NULL){
            if(verbose) printf("Could not find item with index %d\n",x);
            return NULL;
        }
        t = t->next;
    }
    if(t == NULL){
        if(verbose) printf("Could not find item with index %d\n",x);
        return NULL;
    }
    //printf("returning %s\n",t?t->name:"NULL");
    return t;
}

Inv *find_item(Inv *i, char *name){
    if(name == NULL) return NULL;
    for(Inv *t = i; t != NULL; t = t->next){
        if(strcmp(name, t->name) == 0){
            //printf("returning '%s'\n",t->name);
            return t;
        }
    }
    if(verbose) printf("could not find item with name == '%s'\n",name);
    return NULL;
}

Inv *add_to_inv(Inv *toAdd, Inv *i){
    if(toAdd == NULL) return i;
    if(i == NULL) return toAdd;
    if(find_item(i, toAdd->name)){
        find_item(i, toAdd->name)->quantity += toAdd->quantity;
        if(toAdd != find_item(i,toAdd->name)) free(toAdd);
        return i;
    }
    Inv *temp;
    for(temp = i; temp->next!= NULL; temp = temp->next);
    temp->next = toAdd;
    return i;
}

int is_num(char *str);

void print_inv(Inv *i){
    if(i == NULL){
        printf("(nothing)\n");
        return;
    }
    int c = 1;
	while(i){
		printf("%d: "C_Y"%d"C_C" %s"C_W"\tTYPE: "C_B"%s"C_W"\n",c,i->quantity,i->quantity==1?i->name:i->plName, i->type->name);
		printf("\t\"%s\"\n",i->desc);
		printf("\t%s: "C_Y"%.1lf"C_W"\n",i->type->effectDesc,i->effect);
        i = i->next;
        c++;
	}
}

void print_equipped(Equipped *e){
    printf(C_B"Right hand:"C_W"\n");
    e==NULL?printf("nothing\n"):print_inv(e->rHand);

    printf(C_B"Left hand:"C_W"\n");
    e==NULL?printf("nothing\n"):print_inv(e->lHand);

    printf(C_B"Head:\n"C_W);
    e==NULL?printf("nothing\n"):print_inv(e->head);

    printf(C_B"Torso:\n"C_W);
    e==NULL?printf("nothing\n"):print_inv(e->torso);

    printf(C_B"Arms:\n"C_W);
    e==NULL?printf("nothing\n"):print_inv(e->arms);

    printf(C_B"Legs:\n"C_W);
    e==NULL?printf("nothing\n"):print_inv(e->legs);

    printf(C_B"Feet:\n"C_W);
    e==NULL?printf("nothing\n"):print_inv(e->feet);
}

void print_inventory(Items *i){
    if(i == NULL) {
        printf("Nothing\n");
        return;
    }
    //printf("Items Equipped:\n");
    print_equipped(i->equip);
    printf("Items in Inventory:\n");
    print_inv(i->inv);
}

void print_race(Race *r){
    if(r == NULL){
        printf("Error: Race entered was NULL\n");
        return;
    }
    printf("%s: %s\n",r->name,r->desc);
}

void print_world(Dungeon *d, int in){
	if(d == NULL) return;
	//if(in == 0) printf("number of rooms = %d\n",count_rooms(d, 0));

    char c = 'n';
    switch(highest_dungeon_attrib(d,60)){
        case ATTRIB_FAITH:
            c = 'f';
            break;
        case ATTRIB_DINGE:
            c = 'd';
            break;
        case ATTRIB_HAUNT:
            c = 'h';
            break;
        case ATTRIB_DAMAGE:
            c = 'D';
            break;
    }
	//pr_i(4*in);
	printf(C_G "%s"C_W" (%c)\n",d->name, c);

	if(d->left){
		pr_i(2*(in+1));
		printf("%d:",room_depth(d));
		print_world(d->left, in + 1);
	}

	if(d->right){
		pr_i(2*(in+1));
		printf("%d:",room_depth(d));
		print_world(d->right, in + 1);
	}

	if(d->foward){
		pr_i(2*(in+1));
		printf("%d:",room_depth(d));
		print_world(d->foward, in + 1);
	}
	if(in == 0) printf("number of rooms = %d\n", count_rooms(d,0));
}

int count_monsters(Charlist *cl){
	int i = 0;
	for(Character *c = cl->first; c != NULL; c = c->next){
		i ++;
	}
	return i;
}

void print_monsters(Charlist *ml){
    int i = 1;
	for(Character *m = ml->first; m != NULL; m = m->next, i++){
		printf("%d: "C_R"%s"C_W"\t(level %d)\n\t%.1lf/%.1lf\n",i, m->name, m->level,m->life, m->lifeTotal);
	}
}

void print_dialogue(Dialogue *d, char *speakerName){
	//printf("printing dialogue\n");
	if(d == NULL) return;

	printf(C_R"%s"C_W": \"%s\"\n",speakerName, d->text);
	if(d->optionAText[0] != '\0'){
		printf("\tA: %s\n",d->optionAText);
	}
	if(d->optionBText[0] != '\0'){
		printf("\tB: %s\n",d->optionBText);
	}
	if(d->optionCText[0] != '\0'){
		printf("\tC: %s\n",d->optionCText);
	}
	if(d->optionDText[0] != '\0'){
		printf("\tD: %s\n",d->optionDText);
	}
}

void print_room(Dungeon *d){
	printf("You are in ");
	if(is_vowel(d->name[0])){
		printf("an ");
	}else{
		printf("a ");
	}
	printf(C_G"%s"C_W".\t\tDepth = %d\n",d->name, room_depth(d));

	/*printf("Its attributes include:\n");
	printf("damage:\t%.2lf\n",d->damage);
	printf("dinge:\t%.2lf\n",d->dinge);
	printf("haunt:\t%.2lf\n",d->haunt);
	printf("faith:\t%.2lf\n",d->faith);
	*/

	//print_dialogue(d->monsters->monster->dialogue);

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

	if(d->monsters == NULL || count_monsters(d->monsters) == 0){
		printf("\n\nThere are no "C_R"monsters"C_W"\n");
	}else{
		printf("\n\nThere are %d "C_R"monsters"C_W" in this room, they are:\n", count_monsters(d->monsters));
		print_monsters(d->monsters);
	}
	if(d->monsters && d->monsters->first && d->monsters->first->dialogue){
		print_dialogue(d->monsters->first->dialogue, d->monsters->first->name);
	}
}

Inv *create_inv(){
	Inv *i = malloc(sizeof(Inv));
	assert(i);
	i->next = NULL;
    strcpy(i->name, "UNNAMED");
    strcpy(i->plName, "UNNAMEDS");
    i->quantity = 0;
    i->type = NULL;
    i->effect = 0;
    i->usef = NULL;
	return i;
}

Inv *generate_inventory(){
	Inv *i = create_inv();
    //printf("glItems: ");
    //print_glItems();
    //printf("\n");
    i = get_glItem_name("Rusty Sword");
    assert(i);
    //printf("added rusty sword\n");

    i->next = get_glItem_name("Potion of Minor Healing");
    assert(i->next);
    i->next->quantity = 3;
    //printf("added potion\n");

    i->next->next = get_glItem_name("Steel Sword");
    assert(i->next->next);
    //printf("added steel sword\n");

    i->next->next->next = NULL;

	return i;
}

Inv *generate_inventory_dungeon(void){
    //Inv *i = create_inv();
    Inv *i = NULL;
    return i;
}

/*void use_item(Inv *i, Character *c, Character *p){
    i->usef(i, c, p);
}*/

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
		if(i%2 == 0){
			if(i == 0){
				str[i] = ret_c();
			}else{
				str[i] = tolower(ret_c());
			}
		}else{
			if(i == 0){
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

Dialogue *create_dialogue(){
	Dialogue *d = malloc(sizeof(Dialogue));
	assert(d);
	d->optionA = NULL;
	d->optionB = NULL;
	d->optionC = NULL;
	d->optionD = NULL;
	return d;
}

Dialogue *generate_dialogue(char *name){
	Dialogue *d = create_dialogue();

	strcpy(d->text, "I am ");
	strcat(d->text, name);
	strcat(d->text, ", fear me!");

	strcpy(d->optionAText, "Please. Let me live!");

	strcpy(d->optionBText, "No, ");
	strcat(d->optionBText, name);
	strcat(d->optionBText, ", fear me!");

	strcpy(d->optionCText, "I am unimpressed");


	strcpy(d->optionDText, "I will fight you to the death!");

	d->optionD = create_dialogue();
	strcpy(d->optionD->text, "So be it!");

	d->optionA = create_dialogue();
	strcpy(d->optionA->text, "Hahaha, not likely, fool!");

	d->optionB = create_dialogue();
	strcpy(d->optionB->text, "Arrogant creature, prepare yourself for battle!");

	d->optionC = create_dialogue();
	strcpy(d->optionC->text, "And I am unimpressed by you. Now face me in combat!");
	return d;
}

/*Character *generate_player(){
	Character *p = malloc(sizeof(Character));
	assert(p);

	char name[MAX_CHARACTER_NAME] = {0};
	printf("Enter you character's name: ");
	fgets(name, MAX_CHARACTER_NAME, stdin);

	int i = 0;
	while(i < MAX_CHARACTER_NAME && name[i] != '\0'){
		if(name[i] == '\n') name[i] = '\0';
	}

	strcpy(p->name, name);

	return p;
}*/

void split_num(int *loc1, int *loc2, int total){
    if(total < 0){
        *loc1 = total/2;
        *loc2 = total/2;
        return;
    }

    int r = rand()%(total+1);
    *loc1 = r;
    *loc2 = total - r;
}

void generate_stats(Character *c, int total){
    if(c == NULL) return;
    if(total < 0) return;

    int initArr[2] = {0};
    int midArr[4] = {0};
    int statArr[5] = {0};

    split_num(&initArr[0], &initArr[1], total);
    split_num(&midArr[0], &midArr[1], initArr[0]);
    split_num(&midArr[2], &midArr[3], initArr[1]);
    for(int i = 0; i < 3; i ++) statArr[i] = midArr[i];
    split_num(&statArr[3], &statArr[4], midArr[3]);

    c->intelligence = statArr[0] + 1;
    c->strength = statArr[1] + 1;
    c->speed = statArr[2] + 1;
    c->charisma = statArr[3] + 1;
    c->luck = statArr[4] + 1;
}

//add items function here

Items *create_items(void){
    Items *it = malloc(sizeof(Items));
    assert(it);
    it->inv = NULL;
    it->equip = NULL;
    return it;
}

Character *generate_monster(int depth){
	Character *m = malloc(sizeof(Character));
	assert(m);
    depth = depth/2;
    if(depth < 1) depth = 1;

    //name
	generate_monster_name(rand()%5 + 3, m->name);
	//printf("Monster name generated: %s\n",m->name);
	int r1 = rand()%depth + 1;
	int r2 = rand()%depth + 1;
	int r3 = rand()%depth;//rand()%7;
	m->level = r1 * r2 + r3; //generates a random number between 0 and 70, with a bias to lower numbers
	//printf("Monster %s's level = %d\n",m->name, m->level);
	m->lifeTotal = m->level * 10 + rand()%10;
	m->life = m->lifeTotal;

    m->inventory = create_items();

    //printf("Getting claw for monster inv\n");
    //printf("List is: ");
    //print_glItems();
    //printf("\n");

    m->inventory->inv = get_glItem_name("Claw");
    assert(m->inventory->inv);


    //printf("getting potion for monster inv\n");
    //printf("list is: ");
    //print_glItems();
    //printf("\n");
    m->inventory->inv->next = get_glItem_name("Potion of Minor Healing");
    assert(m->inventory->inv->next);

    generate_stats(m, depth * 5);

    m->race = rand()%3?race_kaskeer():race_voidwretch();

	m->dialogue = generate_dialogue(m->name);

    m->dief = monster_die;

    m->next = NULL;
    m->prev = NULL;
	return m;
}

int highest_dungeon_attrib(Dungeon *d, double minAttrib){
	if(d->damage >= d->dinge && d->damage >= d->haunt && d->damage >=d->faith && d->damage > minAttrib){
		return ATTRIB_DAMAGE;
	}else if(d->dinge > d->damage && d->dinge > d->haunt && d->dinge > d->faith && d->dinge > minAttrib){
		return ATTRIB_DINGE;
	}else if(d->haunt > d->damage && d->haunt > d->dinge && d->haunt > d->faith && d->haunt > minAttrib){
		return ATTRIB_HAUNT;
	}else if(d->faith > d->damage && d->faith > d->dinge && d->faith > d->haunt && d->faith > minAttrib){
		return ATTRIB_FAITH;
	}else{
		return ATTRIB_NONE;
	}
}


Clist *create_clist(char text[64]){
	if(text[0] == '\0') return NULL;
	Clist *l = malloc(sizeof(Clist));
	assert(l);
	l->next = NULL;
	strcpy(l->text, text);
	//printf("created clist node at %p with text %s\n",l, l->text);
	return l;
}

void print_clist(Clist *l){
	printf("[");
	while(l){
		printf("%s",l->text);
		if(l->next){
			printf(",");
		}
		l = l->next;
	}
	printf("]");
}

int clist_length(Clist *l){
	int c = 0;
	while(l){
		c ++;
		l = l->next;
	}
	return c;
}

Clist *read_subn(char *loc){
	//printf("\topening file %s\n",loc);
	FILE *f = fopen(loc, "r");
	if(f == NULL){
        printf("FILE \"%s\" does not exist\n",loc);
    }
    char buffer[MAX_ROOM_SUBNAME] = {0};

	Clist *l;
	Clist *head;

	//first read
	if(f != NULL && fgets(buffer, MAX_ROOM_SUBNAME, f)){
		buffer[strcspn(buffer, "\n")] = 0;
		l = create_clist(buffer);
		head = l;
		//printf("read %s\n",buffer);
	}else{
		//printf("\tnothing was read from %s\n",loc);
        strcpy(buffer, "UNNAMED");
		l = create_clist(buffer);
        head = l;
	}
	//looping read
	while(l!=NULL && f != NULL && fgets(buffer, MAX_ROOM_SUBNAME, f)!=NULL){
		if(buffer[0] == '\n' || buffer[0] == '\0') return head;
		//printf("    buffer had text\n");
		buffer[strcspn(buffer, "\n")] = 0;
		//printf("    removed trailing newline\n");
		l->next = create_clist(buffer);
		//printf("    set l->next to %p\n",l->next);
		l = l->next;
		//printf("    moving to %p\n",l);
		//printf("     read %s\n",buffer);
		//print_clist(head);
	}
	if(f!= NULL) fclose(f);
	//print_clist(head);
	//printf("\n\thead = %p\n",head);
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

int monster_over_level(Charlist *m, int level, char mName[MAX_CHARACTER_NAME-10]){
	//replaces the name with the monster with a max level over a specified
	//use MAX_CHARACTER_NAME-10 since ", lair of " is 10 chars long
	//printf("monster_over_level called\n");
	if(m == NULL) return 0;
	//printf("\tm is not null\n");
	//printf("\tm = %p",m);
	//printf(", m->monster = %p\n",m->monster);
	if(m->first == NULL) return 0;
	//printf("\tm->monster is not null\n");
	//printf("\tentering monster over level\n");

	char maxName[MAX_CHARACTER_NAME-10] = {0};
	int maxLevel = level - 1;
	int replaced = 0;
	//printf("\tvariavles init\n");

	for(Character *curr = m->first; curr != NULL; curr = curr->next){
		//printf("\tlooping through monster\n");
		if(curr->level > maxLevel){
			//printf("monster %s has a higher level than the previous max %d, at %d\n",m->monster->name,maxLevel, m->monster->level);
			strcpy(maxName, curr->name);
			maxLevel = curr->level;
			replaced = 1;
		}
	}
	strncpy(mName,maxName, MAX_CHARACTER_NAME-10);
	//printf("Monster over level returned, value %d\n", replaced);
	return replaced;
}

void generate_room_name(Dungeon *d, char name[MAX_ROOM_NAME]){
	//if(verbose) printf("\t\t\tgenerate_room_name called\n");
	int high = highest_dungeon_attrib(d, 60);
	char prefix[MAX_ROOM_SUBNAME] = {0};
	char room[MAX_ROOM_SUBNAME] = {0};
	char suffix[MAX_ROOM_SUBNAME] = {0};
	//printf("\tvariables initialised\n");
	Clist *pNames;
	Clist *rNames;
	//printf("\tclists named\n");

	int clistl = 0;
	int r = 0;
	//printf("\tmore variables init\n");

	switch(high){
		case ATTRIB_DAMAGE:
			pNames = read_subn("damage_prefix.subn");
			//if(verbose) printf("\t\t\troom type: damaged\n");
			break;
		case ATTRIB_DINGE:
			pNames = read_subn("dinge_prefix.subn");
            //if(verbose) printf("\t\t\troom type: dinge\n");
			break;
		case ATTRIB_HAUNT:
			pNames = read_subn("haunt_prefix.subn");
            //if(verbose) printf("\t\t\troom type: haunt\n");
			break;
		case ATTRIB_FAITH:
			pNames = read_subn("faith_prefix.subn");
            //if(verbose) printf("\t\t\troom type: faith\n");
			break;
		default:
			pNames = read_subn("none_prefix.subn");
            //if(verbose) printf("\t\t\tno room type!\n");
			break;
	}
	//if(verbose) printf("\t\t\tread subn\n");
	//randomise prefix
    //if(verbose) printf("\t\t\tFinding length of char list\n");
	clistl = clist_length(pNames);
    //if(verbose) printf("\t\t\tLength found, = %d\n", clistl);
	//printf("\tpNames = ");
	//print_clist(pNames);
	//printf("\t\tlength of clist = %d\n",clistl);
    if(clistl == 0){
        if(verbose) printf("CLISTL was 0, setting clistl to 1\n");
        clistl = 1;
    }
	r = rand()%clistl;
    //if(verbose) printf("\t\t\tr = %d\n",r);
    for(int i = 0; i < r && pNames->next; i ++) pNames = pNames->next;
    //printf("\t\t\tlooped to random pNames. pNames = %p\n", pNames);
	strcpy(prefix, pNames->text);
	strcat(prefix, " ");
    //if(verbose) printf("\t\t\tprefix: '%s'\n",prefix);

	//room name
	rNames = read_subn("room_names.subn");
    //printf("read rNames, rNames = ");
    //print_clist(rNames);
    //printf("\n");
	clistl = clist_length(rNames);
    if(clistl == 0) clistl = 1;
	r = rand()%clistl;
	for(int i = 0; i < r && rNames != NULL && rNames->next != NULL; i ++) rNames = rNames->next;
	//printf("\t\t\tLooped to random rName, rName = %p\n", rNames);
	strcpy(room, rNames->text);
	//if(verbose) printf("\t\t\troom: '%s'\n",room);

	//suffix
	char mName[MAX_CHARACTER_NAME-10] = {0};
	if(monster_over_level(d->monsters, 30, mName)){
		//if there is a big monster
		//printf("creating a suffix\n");
		strcpy(suffix, ", lair of ");
		strcat(suffix, mName);
	}
	//if(verbose) printf("\t\t\tsuffix: '%s'\n",suffix);

	strcpy(name, prefix);
	strcat(name, room);
	strcat(name, suffix);
	//printf("name: %s\n",name);

	free_clist(pNames);
	free_clist(rNames);
}

void append_monster_list(Charlist *ml, int depth){
    Character *m = generate_monster(depth);
    assert(m);
    if(ml->last == NULL){
        ml->first = m;
        ml->last = m;
        ml->curr = m;
        return;
    }
    ml->last->next = m;
    ml->last->next->prev = ml->last;
    ml->last = m;
}

Charlist *create_charlist(void){
    Charlist *cl = malloc(sizeof(Charlist));
    cl->first = NULL;
    cl->curr = NULL;
    cl->last = NULL;
    return cl;
}

Charlist *generate_many_monsters(int num, int depth){
	if(num < 1) return NULL;
	Charlist *ml = create_charlist();
	Charlist *head = ml;
	for(int i = 0; i < num; i ++){
		append_monster_list(ml, depth);
	}
	return head;
}

Dungeon *create_room(int depth){
	if(verbose) printf("\tcreating a room\n");
	Dungeon *d = malloc(sizeof(Dungeon));
	if(d == NULL){
        printf("There was no more memory when trying to create a room\n");
        exit(EXIT_FAILURE);
    }

    if(depth > DEPTH_CAP){
        if(verbose) printf("Too deep. Will not generate this\n");
        return NULL;
    }

	//if(verbose) printf("dungeon created at %p\n",d);

	d->damage = (rand()%10000)/100.0;
	d->dinge = (rand()%10000)/100.0;
	d->haunt = (rand()%10000)/100.0;
	d->faith = (rand()%10000)/100.0;
	//if(verbose) printf("\t\tattributes created\n");

	d->inventory = NULL;
	//generate_inventory();
    //if(verbose) printf("\t\tgenerating inventory\n");
	//d->inventory = generate_inventory_dungeon();
	//if(verbose) printf("\t\tgenerated inv\n\t\tgenerating monsters");
	d->monsters = generate_many_monsters(rand()%4, depth);
	//if(verbose) printf("\t\tgenerated monster\n");

	//room name must be last because it is dependant on the above features
    //if(verbose)printf("\t\tgenerating room name\n");
    generate_room_name(d, d->name);
	//if(verbose) printf("\troom \"%s\" created\n",d->name);
	return d;
}

Dungeon *generate_room(Dungeon *d, Dungeon *back){
	//if(verbose) printf("\tgenerating room\n");
	if(d == NULL){
        if(verbose) printf("\t\ttried to generate room, but the room given was NULL\n");
        return NULL;
    }

    d->back = back;
    int depth = room_depth(d);
	//create a left door
	int r = rand()%ROOM_CO;
	//printf("\tr = %d\n",r);
	if(r == 0){
		//printf("creating door to the left\n");
		d->left = generate_room(create_room(depth), d);
	}else{
		d->left = NULL;
	}

	//create a right door
	r = rand()%ROOM_CO;
        //printf("\tr = %d\n",r);
        if(r == 0){
		//printf("creating door to the right\n");
		d->right = generate_room(create_room(depth), d);
	}else{
		d->right = NULL;
	}

	//create a foward door
	r = rand()%ROOM_CO;
        //printf("\tr = %d\n",r);
        if(r == 0){
		//printf("creating door to the front\n");
		d->foward = generate_room(create_room(depth), d);
	}else{
		d->foward = NULL;
	}
	return d;
}

//initialise the dungeon
Dungeon *generate_dungeon(){
	//unsigned int seed;
	//printf("in generate_dungeon, seed = %d\n",seed);
	//printf("generating dungeon\n");
	Dungeon *d = create_room(1);
	//printf("first dungeon created\n");
	d->back = NULL;
	d->left = generate_room(create_room(2), d);
	//printf("left wing created\n");
	d->right = generate_room(create_room(2), d);
	//printf("right wing created\n");
	d->foward = generate_room(create_room(2), d);
	//printf("foward wing created\n");
	return d;
}

int is_valid_name(char *name){
    //checks whether a name is valid
    if(name == NULL) return 0;
    if(strcmp(name, "") == 0 || strcmp(name, "room")==0 || strcmp(name, "inv") == 0 || strcmp(name, "world") == 0) return 0;
    for(unsigned int i = 0; i < strlen(name); i ++){
        if(isspace(name[i])) return 0;
    }
    if(strcmp(name, "NAMEHELP") == 0){
        printf("\nYour name must:\n - Not be 'room', 'inv', 'world', or 'NAMEHELP',\n - Not be empty (ie no letters),\n - Not have any whitespace.\n");
        return 0;
    }
    return 1;
}

int is_num(char *str){
    //checks through a string to ensure all characters are digits (allows a trailing \n character)
    for(unsigned int i = 0; str[i] != '\0'; i ++){
        //printf("i = %d, str[%d] = %c\n",i,i,str[i]);
        if(str[i] == '\n' && str[i+1] == '\0') break;
        if(isdigit(str[i]) == 0){
            //printf("returning 0\n");
            return 0;
        }
    }
    //printf("returning 1\n");
    return 1;
}

int get_player_stat(char *stat, int *totalPoints, int *remainingPoints){
    //returns a value entered by the user which is lower than the remaining points. reduces the remaining points by the value entered
    //only accepts integers >= 0
    if(*remainingPoints <= 0){
        printf("\nYou have 0 points left to spend. Setting "C_G"%s"C_W" to 0.\n",stat);
        return 0;
    }

    printf("\nYou have %d/%d points to spend.\nEnter your "C_G"%s"C_W": ",*remainingPoints, *totalPoints, stat);

    int point = *totalPoints;
    char inputPoint[100] = {0};
    do{
        fgets(inputPoint, 100, stdin);
        //printf("You entered %s = %d\n",inputPoint, atoi(inputPoint));
        if(!is_num(inputPoint) || atoi(inputPoint) < 0 || atoi(inputPoint) > *remainingPoints){
            printf("\nError: You entered an invalid value for "C_G"%s"C_W". You have %d/%d points left to spend.\nEnter your "C_C"%s"C_W": ",stat, *remainingPoints, *totalPoints, stat);
        }else{
            point = atoi(inputPoint);
            break;
        }
    } while(1);
    *remainingPoints -= point;
    return point;
}

void generate_player_stats(Character *p, int totalPoints){
    //creates the players statistics based on what they enter
    printf("generating player statistics with %d total points\n",totalPoints);
    int remainingPoints = totalPoints;
    p->intelligence = get_player_stat("intelligence", &totalPoints, &remainingPoints);
    p->strength = get_player_stat("strength", &totalPoints, &remainingPoints);
    p->speed = get_player_stat("speed", &totalPoints, &remainingPoints);
    p->charisma = get_player_stat("charisma", &totalPoints, &remainingPoints);
    p->luck = get_player_stat("luck", &totalPoints, &remainingPoints);
}

Items *generate_player_items(void){
    Items *i = malloc(sizeof(Items));
    assert(i);
    i->inv = NULL;
    i->equip = NULL;
    return i;
}

Inv *remove_from_inv(int index, Inv *inv){
    if(inv == NULL) return NULL;
    Inv *temp = inv;
    if(index == 0){
        inv = inv->next;
        return temp;
    }
    int i = 0;
    Inv *prev = inv;
    for(temp = inv, i = 0; temp != NULL; temp = temp->next, i++){
        if(i == index){
            prev->next = temp->next;
            return temp;
        }
        prev = temp;
    }
    return NULL;
}

//initiialise the player
Character *generate_player(char *parName){
	//printf("Generate player called\n");
    Character *p = malloc(sizeof(Character));
	assert(p);

    char name[MAX_CHARACTER_NAME] = {0};
    if(parName == NULL || (strcmp(parName, "") == 0 || is_valid_name(parName) == 0)){
        if(parName != NULL && !is_valid_name(parName)) printf("Warning: \"%s\" is an invalid name. ", parName);
        printf("Enter your character's name: ");
        int validName = 0;
        while(validName == 0){
            fgets(name, MAX_CHARACTER_NAME, stdin);
            int i = 0;
            while(i < MAX_CHARACTER_NAME && name[i] != '\0'){
                if(name[i] == '\n'){
            	    name[i] = '\0';
	    		    break;
	    	    }
	    	    i++;
	        }
            if(is_valid_name(name)){
                validName = 1;
            }else{
                if(strcmp(name, "NAMEHELP")) printf("%sThe name \"%s\" is not allowed. Enter 'NAMEHELP' for information about which names are valid\n",name[0] == '\0' ? "\n":"", name);
                printf("Enter your character's name: ");
            }
        }
    }else{
        strncpy(name, parName, MAX_CHARACTER_NAME);
    }
    strcpy(p->name, name);

    p->inventory = generate_player_items();
    p->inventory->inv = generate_inventory();
    p->lifeTotal = 100;
    p->life = p->lifeTotal;
    generate_player_stats(p,25);
    p->race = race_human();
	return p;
}
