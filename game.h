#define MAX_ROOM_NAME 256
#define MAX_CHARACTER_NAME 64
#define MAX_INV_NAME 64
#define MAX_INV_DESC 2048

#define ID_LEFT 1
#define ID_RIGHT 2
#define ID_BACK 3
#define ID_FOWARD 4
#define ID_PRINT 5
#define ID_TEST 6

#define TYPE_SWORD 1
#define TYPE_BOW 2
#define TYPE_RING 3
#define TYPE_POTION 4
#define TYPE_GOLD 5

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

//characters (including the player and monsters
typedef struct character{
        char name[MAX_INV_NAME];
        int level;
	double lifeTotal;
	double life;
	double intelligence;
	double strength;
	double charisma;
	double luck;
	Inv *inventory;
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

//FUNCTIONS
Dungeon *generate_dungeon();

Character *generate_player();

void print_world(Dungeon *d, int in);

void print_room(Dungeon *d);

void print_monsters(Monsters *m);

int count_rooms(Dungeon *d, int c);
