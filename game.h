#ifndef __GAME_H
#define __GAME_H 1

//Maximum string lengths
#define MAX_ROOM_NAME 256
#define MAX_CHARACTER_NAME 64
#define MAX_INV_NAME 64
#define MAX_INV_DESC 2048
#define MAX_DIALOGUE 2048
#define MAX_DIALOGUE_TEXT 512
#define MAX_ROOM_SUBNAME 64
#define MAX_ROOM_SUBNAME 64
#define MAX_ROOM_SUBNAME 64

//Command IDs handled int the game.c file

//Item IDs
#define ITEM_SWORD 1
#define ITEM_BOW 2
#define ITEM_RING 3
#define ITEM_POTION 4
#define ITEM_GOLD 5

//Room Attributes
#define ATTRIB_NONE 0
#define ATTRIB_DAMAGE 1
#define ATTRIB_DINGE 2
#define ATTRIB_HAUNT 3
#define ATTRIB_FAITH 4

//Dialogue
#define DLOG_MONSTER 0
#define DLOG_PLAYER 1

#define DLOG_TALK 0
#define DLOG_FIGHT 1
#define DLOG_LOOT 2

#define SKILL_CAP 100
#define DEPTH_CAP 100

#define BLOC_HEAD 1
#define BLOC_TORSO 2
#define BLOC_LEGS 3
#define BLOC_ARMS 4
#define BLOC_LHAND 5
#define BLOC_RHAND 6
#define BLOC_FEET 7

#define MONST_AGGRO 1
#define MONST_PASSIVE 0
#define MONST_DEFENSIVE 2

//colour definitions
#define C_R "\x1b[31m"
#define C_G "\x1b[32m"
#define C_Y "\x1b[33m"
#define C_B "\x1b[34m"
#define C_M "\x1b[35m"
#define C_C "\x1b[36m"
#define C_W "\x1b[0m"

//GLOBAL VARIABLES

int verbose;

//STRUCTS

//inventory

typedef struct inv Inv;
typedef struct character Character;

typedef void (*Usef)(void*, void*, void*);
typedef void (*Dief)(void*, Character*);

typedef struct itemType{
    char name[MAX_INV_NAME]; //name of the type (eg sword)
    char plName[MAX_INV_NAME]; //here swords
    char effectDesc[MAX_INV_DESC]; //eg for a sword damage, for a potion healing ability
    int bodyLocation; //eg BLOC_HEAD, BLOC_HAND1
} ItemType;

struct inv{
    char name[MAX_INV_NAME];
	char plName[MAX_INV_NAME];
    char desc[MAX_INV_DESC];
	int quantity;
	ItemType *type;
	double effect;
    double value;
    Usef usef;
	struct inv *next;
};

typedef struct glItemList{
    Inv *first;
    Inv *last;
} GlItemList;

typedef struct equipped{
    Inv *head;
    Inv *torso;
    Inv *arms;
    Inv *legs;
    Inv *feet;
    Inv *lHand;
    Inv *rHand;
} Equipped;

typedef struct items{
    Inv *inv;
    Equipped *equip;
} Items;


//the dlist is the overarching dialogue structure. contains the dlog and the next dlist (each stage of interaction is considered a dlist,
//with dialogue stored in the dlog
typedef struct dlist{
    struct dlog *this;
    struct dlist *next;
} Dlist;

typedef struct dlog{
    char text[MAX_DIALOGUE];
    int speaker;
    int action;
    int id;
    Dlist *responses;
} Dlog;


//legacy dialogue
typedef struct dialogue{
	char text[MAX_DIALOGUE];

	char optionAText[MAX_DIALOGUE_TEXT];
	char optionBText[MAX_DIALOGUE_TEXT];
	char optionCText[MAX_DIALOGUE_TEXT];
	char optionDText[MAX_DIALOGUE_TEXT];

	struct dialogue *optionA;
	struct dialogue *optionB;
	struct dialogue *optionC;
	struct dialogue *optionD;
} Dialogue;

typedef struct race{
    char name[MAX_INV_NAME];
    char plName[MAX_INV_NAME];
    char desc[MAX_INV_DESC];
} Race;

//characters (including the player and monsters
struct character{
    char name[MAX_INV_NAME];
    int level;

	double lifeTotal;
	double life;

	double intelligence;
	double strength;
	double speed;
	double charisma;
	double luck;

    int isAgressive;

    Race *race;

	Items *inventory;
	Dialogue *dialogue;

    Dief dief;
    struct character *next;
    struct character *prev;
};

//list of characters
typedef struct character_list{
    Character *curr;
    Character *first;
    Character *last;
} Charlist;

//a list of monsters (LEGACY)
typedef struct monsters{
	Character *monster;
	struct monsters *next;
} Monsters;

//the list of dungeon rooms
typedef struct dungeon{
	char name[MAX_ROOM_NAME];

	double damage;
	double dinge;
	double haunt;
	double faith;

	Charlist *monsters;
	Inv *inventory;

	struct dungeon *left;
	struct dungeon *right;
	struct dungeon *foward;
	struct dungeon *back;
} Dungeon;

//for generating dungeon names
typedef struct char_list{
	char text[64];
	struct char_list *next;
} Clist;

//FUNCTIONS
Dungeon *generate_dungeon();

Character *generate_player(char *parName);

void print_world(Dungeon *d, int in);

void print_room(Dungeon *d);

void print_monsters(Charlist *cl);

void print_dialogue(Dialogue *d, char *speakerName);

void print_inv(Inv *i);

void print_equipped(Equipped *e);

void print_inventory(Items *i);

void print_race(Race *r);

Character *find_character_index(Charlist *ml, Character *p, int x);

Character *find_character(Charlist *ml, Character *p, char *name);

Charlist *create_charlist(void);

Inv *find_item(Inv *i, char *name);

Inv *find_item_index(Inv *it, int x);

Inv *add_to_inv(Inv *toAdd, Inv *i);

int item_exists(Inv *i, char *name);

void print_character(Character *p);

void generate_monster_name(int length, char str[length]);

int count_rooms(Dungeon *d, int c);

int is_num(char *str);

void init_glItemList(void);

//DIALOGUE
void read_dlog(char *filename);

#endif
