#include "game.h"
#include "item.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

GlItemList *bg_glItems = NULL;

Inv *create_inv_element(char *name, char *plName, char *desc, int quantity, ItemType *type, double effect, double value, Usef usef){
    Inv *i = malloc(sizeof(Inv));
    assert(i);
    strncpy(i->name, name, MAX_INV_NAME);
    strncpy(i->plName, plName, MAX_INV_NAME);
    strncpy(i->desc, desc, MAX_INV_DESC);
    i->quantity = quantity;
    i->type = type;
    i->effect = effect;
    i->value = value;
    i->usef = usef;
    i->next = NULL;
    return i;
}

GlItemList *create_glItemList(void){
    GlItemList *il = malloc(sizeof(GlItemList));
    assert(il);
    il->first = NULL;
    il->last = NULL;
    return il;
}

Inv *last_in_list(Inv *i){
    if(i == NULL) return NULL;
    while(i->next) i = i->next;
    return i;
}

int name_exists(Inv *i, char *name){
    for(Inv *temp = i; temp != NULL; temp = temp->next){
        if(strcmp(i->name, name) == 0) return 1;
    }
    return 0;
}

GlItemList *append_glItemList(Inv *toAdd){
    //printf("append to glItemList called with toAdd = %p\n",toAdd);
    if(bg_glItems == NULL){
        bg_glItems = create_glItemList();
        //printf("bg_glItems was NULL, now is %p\n",bg_glItems);
    }
    if(toAdd == NULL){
        //printf("toAdd was null, returning bg_glItems = %p\n", bg_glItems);
        return bg_glItems;
    }
    if(name_exists(bg_glItems->first, toAdd->name)){
        //printf("%s already exists in the list, not appending\n",toAdd->name;
        return bg_glItems;
    }
    if(bg_glItems->first == NULL){
        //printf("bg_glItems was empty, putting %s at start\n",toAdd->name);
        //empty list
        bg_glItems->first = toAdd;
        bg_glItems->last = last_in_list(bg_glItems->first);
        bg_glItems->last->next = NULL;
        return bg_glItems;
    }

    //printf("adding %s to the end of list\n",toAdd->name);
    bg_glItems->last->next = toAdd;
    bg_glItems->last = last_in_list(bg_glItems->first);

    return bg_glItems;
}

GlItemList *get_glItems(void){
    if(bg_glItems == NULL){
        bg_glItems = create_glItemList();
        return bg_glItems;
    }else{
        return bg_glItems;
    }
}

void print_glItems(void){
    printf("[");
    if(bg_glItems == NULL){
        printf("NULL]");
        return;
    }
    for(Inv *temp = bg_glItems->first; temp != NULL; temp = temp->next){
        printf("'%s'",temp->name);
        if(temp->next) printf(", ");
    }
    printf("]");
}

Inv *get_glItem_name(char *name){
    //printf("finding %s in item list\n",name);
    if(bg_glItems == NULL){
        //printf("the item list was null, returning NULL\n");
        return NULL;
    }
    if(name == NULL){
        //printf("name provided was null, returning NULL\n");
        return NULL;
    }
    if(strcmp(name, bg_glItems->last->name) == 0) return bg_glItems->last;

    for(Inv *temp = bg_glItems->first; temp != NULL; temp = temp->next){
        if(strcmp(name,temp->name) == 0){
            //printf("name (%s) matches temp->name(%s)\n",name,temp->name);
            return temp;
        }
    }
    //printf("Could not find %s in list\n",name);
    return NULL;
}
