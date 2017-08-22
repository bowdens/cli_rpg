#include "game.h"
#include "tomlib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void use_sword(void *inv, void *character, void *player){
    //printf("Using sword, inv = %p, character = %p\n", inv, character);
    Inv *i = (Inv *)inv;
    //printf("downcast inv\n");
    Charlist *target = (Charlist *)character;
    //printf("downcast character\n");
    Character *p = (Character *) player;
    if(i == NULL){
        printf("You try to swing your sword before realising that you do not have one\n");
        return;
    }
    if(target == NULL || target->curr == NULL){
        printf("You do not hit anything\n");
        return;
    }
    double damage = i->effect;
    int crt = rand()%(int)round((SKILL_CAP - round(p->luck))/4 + 1);
    printf("crt = %d\n",crt);
    if(crt == 0){
        damage = damage * 2 + (double)p->luck/SKILL_CAP;
        printf(C_Y"Critical!"C_W"\n");
    }
    target->curr->life -= i->effect;
    printf("%.1lf damage was dealt to "C_R"%s"C_W" using a%s "C_B"%s"C_W". "C_R"%s"C_W" now has %.1lf/%.1lf health remaining.\n",
            damage, target->curr->name, is_vowel(i->name[0])?"n":"", i->name, target->curr->name, target->curr->life >= 0?target->curr->life:0,
            target->curr->lifeTotal);
    if(target->curr->life < 0.1) target->curr->dief(target);
}

void use_potionh(void *inv, void *character, void *player){
    //TODO
}
