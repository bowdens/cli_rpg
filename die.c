#include "game.h"
#include <stdio.h>
#include <stdlib.h>

void player_die(void *ptr){
    Character *p = (Character*) ptr;
    if(p == NULL){
        printf("Tried to kill player but player does not exist\n");
        exit(EXIT_FAILURE);
    }
    printf("%s died\n", p->name);
    exit(EXIT_SUCCESS);
}

void monster_die(void *ptr){
    //printf("killing %p\n",ptr);
    Charlist *ml = (Charlist *)ptr;
    printf(C_R"%s"C_W" died\n",ml->curr->name);
    if(ml == NULL) return;
    if(ml->curr == NULL) return;
    if(ml->curr == ml->first && ml->curr == ml->last){
        //one element
        free(ml->curr);
        ml->curr = NULL;
        ml->first = NULL;
        ml->last = NULL;
    }else if(ml->curr == ml->first){
        //first element
        ml->curr = ml->curr->next;
        free(ml->first);
        ml->first = ml->curr;
        ml->curr->prev = NULL;
    }else if(ml->curr == ml->last){
        //last element
        ml->curr = ml->curr->prev;
        ml->curr->next = NULL;
        free(ml->last);
        ml->last = ml->curr;
    }else{
        //regular case
        Character *temp = ml->curr;
        ml->curr = ml->curr->next;
        ml->curr->prev = temp->prev;
        temp->prev->next = ml->curr;
        free(temp);
    }
    //TODO: use encapsulating character list or similar to be able to remove specific monsters from the list when they die
}
