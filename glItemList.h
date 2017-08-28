#ifndef _ITEMLIST_H
#define _ITEMLIST_H 1

#include "game.h"
#include "item.h"

Inv *create_inv_element(char *name, char *plName, char *desc, int quantity, ItemType *type, double effect, double value, Usef usef);

GlItemList *create_glItemList(void);

GlItemList *append_glItemList(Inv *toAdd);

GlItemList *get_glItems(void);

Inv *get_glItem_name(char *name);

void print_glItems(void);

#endif
