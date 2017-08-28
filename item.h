#ifndef _ITEM_H
#define _ITEM_H 1

#include "game.h"

void use_sword(void *inv, void *character, void *player);
void use_potionh(void *inv, void *character, void *player);

ItemType *item_sword(void);
ItemType *item_hPotion(void);
ItemType *item_shield(void);
ItemType *item_melee(void);

#endif
