#include "game.h"
#include "tomlib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


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
    if(p == NULL){
        printf("ERROR: You cannot attack with your sword because you do not exist\n");
        return;
    }
    double damage = i->effect;
    int crt = rand()%(int)round((SKILL_CAP - round(p->luck))/4 + 1);
    //printf("crt = %d\n",crt);
    if(crt == 0){
        damage = damage * 2 + (double)p->luck/SKILL_CAP;
        printf(C_Y"Critical!"C_W"\n");
    }
    target->curr->life -= i->effect;
    printf(C_Y"%s"C_W" dealt %.1lf damage to "C_R"%s"C_W" using a%s "C_C"%s"C_W". "C_R"%s"C_W" now has %.1lf/%.1lf health remaining.\n",
            p->name, damage, target->curr->name, is_vowel(i->name[0])?"n":"", i->name, target->curr->name, target->curr->life >= 0?target->curr->life:0,
            target->curr->lifeTotal);
    if(target->curr->life < 0.1) target->curr->dief(target);
}

double cap(double amount, double max){
    return amount > max ? max : amount;
}

void use_potionh(void *inv, void *character, void *player){
    Inv *i = (Inv*) inv;
    if(i == NULL){
        printf("You do not have this item\n");
        return;
    }
    Charlist *target = (Charlist*) character;
    if(target == NULL){
        printf("Your target does not exist\n");
        return;
    }
    Character *p = (Character*) player;
    if(p == NULL){
        printf("ERROR: you do not exist\n");
        return;
    }
    if(i->quantity < 1){
        printf("You have no more "C_B"%s"C_W"\n",i->name);
        return;
    }
    i->quantity -= 1;
    double heal = cap(i->effect, target->curr->lifeTotal - target->curr->life);
    //printf("healing %p: %s\n",target->curr, target->curr->name);
    target->curr->life += heal;
    printf("You healed %s for "C_Y"%.1lf"C_W" health. %s %.1lf/%.1lf health remaining.\n",
            target->curr == p ? "yourself":target->curr->name, heal, target->curr == p ? "You now have":strcat(target->curr->name, "now has"),
            target->curr->life, target->curr->lifeTotal);
    printf("You have "C_Y"%d"C_W" remaining %s\n",i->quantity,i->quantity == 1?i->name:i->plName);
}
