#define MAX_ROOM_NAME 256
#define MAX_CHARACTER_NAME 64
#define MAX_INV_NAME 64
#define MAX_INV_DESC 2048
#define MAX_DIALOGUE 2048
#define MAX_DIALOGUE_RESPONSE 512

#define ID_LEFT 1
#define ID_RIGHT 2
#define ID_BACK 3
#define ID_FOWARD 4
#define ID_PRINT 5
#define ID_TEST 6

#define WEAPON_SWORD 1
#define WEAPON_BOW 2
#define WEAPON_RING 3
#define WEAPON_POTION 4
#define WEAPON_GOLD 5

#define ATTRIB_DAMAGE 1
#define ATTRIB_DINGE 2
#define ATTRIB_HAUNT 3
#define ATTRIB_FAITH 4

//GLOBAL VARIABLES
extern unsigned int seed;

//STRUCTS

//inventory
typedef struct inv{
        char name[MAX_INV_NAME];
	char desc[MAX_INV_DESC];
	int quantity;
	int type;
	double effect;
	struct inv *next;
} Inv;

typedef struct dialogue{
	char dialogue[MAX_DIALOGUE];
	
	char optionAText[MAX_DIALOGUE_TEXT];
	char optionBText[MAX_DIALOGUE_TEXT];
	char optionCText[MAX_DIALOGUE_TEXT];
	char optionDText[MAX_DIALOGUE_TEXT];
	
	struct dialogue *optionA;
	struct dialogue *optionB;
	struct dialogue *optionC;
	struct dialogue *optionD;
} Dialogue;

//characters (including the player and monsters
typedef struct character{
        char name[MAX_INV_NAME];
        int level;
	
	double lifeTotal;
	double life;
	
	double intelligence;
	double strength;
	double speed;
	double charisma;
	double luck;
	
	Inv *inventory;
	Dialogue *dialogue;
} Character;

//a list of monsters
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

	Monsters *monsters;
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

Character *generate_player();

void print_world(Dungeon *d, int in);

void print_room(Dungeon *d);

void print_monsters(Monsters *m);

int count_rooms(Dungeon *d, int c);
