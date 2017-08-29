#include "game.h"
#include "item.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void print_glItems(void);

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
    Inv *temp = i;
    while(temp->next) temp = temp->next;
    return temp;
}

int name_exists(Inv *i, char *name){
    for(Inv *temp = i; temp != NULL; temp = temp->next){
        if(strcmp(temp->name, name) == 0) return 1;
    }
    return 0;
}

void append_glItemList(Inv *toAdd){
    //printf("append to glItemList called with toAdd = %p\n",toAdd);
    if(bg_glItems == NULL){
        bg_glItems = create_glItemList();
        //printf("\tbg_glItems was NULL, now is %p\n",bg_glItems);
    }
    if(toAdd == NULL){
        //printf("\ttoAdd was null, returning bg_glItems = %p\n", bg_glItems);
        return;
    }
    if(name_exists(bg_glItems->first, toAdd->name)){
        //printf("\t%s already exists in the list, not appending\n",toAdd->name);
        return;
    }
    if(bg_glItems->first == NULL){
        //printf("\tbg_glItems was empty, putting %s at start\n",toAdd->name);
        //empty list
        bg_glItems->first = toAdd;
        bg_glItems->last = last_in_list(bg_glItems->first);
        bg_glItems->last->next = NULL;
        return;
    }
    if(bg_glItems->last == bg_glItems->first){
        //one element
        //printf("\tThere was one element\n");
        bg_glItems->last = toAdd;
        bg_glItems->first->next = toAdd;
        bg_glItems->last->next = NULL;
        return;
    }

    //printf("\tadding %s to the end of list\n",toAdd->name);
    toAdd->next = NULL;
    //printf("\tbg_glItems->last = %p\n",bg_glItems->last);
    bg_glItems->last->next = toAdd;
    bg_glItems->last = toAdd;
    //printf("\tbg_glItems->last = %p\n",bg_glItems->last);
    bg_glItems->last->next = NULL;
    //printf("\tlist is: ");
    //fflush(stdout);
    //print_glItems();
    //printf("\n");
    return;
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
    int i = 0;
    for(Inv *temp = bg_glItems->first; temp != NULL && i < 10; temp = temp->next, i++){
        printf("'%s'",temp->name);
        if(temp->next) printf(", ");
    }
    printf("]");
}

Inv *create_copy(Inv *i){
    if(i == NULL) return NULL;
    Inv *new = create_inv_element(i->name, i->plName, i->desc, i->quantity, i->type, i->effect, i->value, i->usef);
    return new;
}

Inv *get_glItem_name(char *name){
    //printf("finding %s in item list: ",name);
    //print_glItems();
    //printf("\n");
    if(bg_glItems == NULL){
        //printf("\tthe item list was null, returning NULL\n");
        return NULL;
    }
    if(name == NULL){
        //printf("\tname provided was null, returning NULL\n");
        return NULL;
    }
    if(strcmp(name, bg_glItems->last->name) == 0){
        //printf("\tName was last element\n");
        return create_copy(bg_glItems->last);
    }
    Inv *temp = bg_glItems->first;
    while(temp != NULL){
        //printf("\ttesting %s (%p) against %s. temp->next = %s (%p)\n",temp->name,temp, name, temp->next?temp->next->name:"NULL", temp->next);
        if(strcmp(name,temp->name) == 0){
            //printf("\tname (%s) matches temp->name(%s)\n",name,temp->name);
            //printf("\tafter finding, list = ");
            //print_glItems();
            //printf("\n");
            return create_copy(temp);
        }
        temp = temp->next;
    }
    //printf("\tCould not find %s in list\n",name);
    return NULL;
}
