#include "game.h"
#include "tomlib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

ItemType *make_item(char *name, char *plName, char *desc, int bloc){
    ItemType *it = malloc(sizeof(ItemType));
    assert(it);
    strncpy(it->name, name, MAX_INV_NAME);
    strncpy(it->plName, plName, MAX_INV_NAME);
    strncpy(it->effectDesc, desc, MAX_INV_DESC);
    it->bodyLocation = bloc;
    return it;
}

ItemType *gl_item_sword = NULL;

ItemType *item_sword(void){
    if(gl_item_sword){
        return gl_item_sword;
    }else{
        gl_item_sword = make_item(
                "sword", "swords",
                "damage", BLOC_RHAND);
        return gl_item_sword;
    }
}

ItemType *gl_item_hPotion = NULL;

ItemType *item_hPotion(void){
    if(gl_item_hPotion){
        return gl_item_hPotion;
    }else{
        gl_item_hPotion = make_item(
                "healing potion", "healing potions",
                "health points restored", BLOC_LHAND);
        return gl_item_hPotion;
    }
}

ItemType *gl_item_shield = NULL;

ItemType *item_shield(void){
    if(gl_item_shield){
        return gl_item_shield;
    }else{
        gl_item_shield = make_item(
                "shield", "shields",
                "damage reduction", BLOC_LHAND);
        return gl_item_shield;
    }
}

ItemType *gl_item_melee = NULL;

ItemType *item_melee(void){
    if(gl_item_melee){
        return gl_item_melee;
    }else{
        gl_item_melee = make_item(
                "melee", "melee",
                "damage", BLOC_RHAND);
        return gl_item_melee;
    }
}

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
    double added = (rand()%(int)round(p->strength*5))/10.0;
    double damage = i->effect;
    if(verbose) printf("damage = %.1lf + %.1lf + %.1lf = %.1lf\n",damage, added, p->strength*0.5, added + p->strength*0.5 + damage);
    damage += added + p->strength * 0.5;
    int crt = rand()%(int)round((SKILL_CAP - round(p->luck))/4 + 1);
    //printf("crt = %d\n",crt);
    if(crt == 0){
        damage = damage * 2 + (double)p->luck/SKILL_CAP;
        printf(C_Y"Critical!"C_W"\n");
    }
    target->curr->life -= damage;
    printf(C_Y"%s"C_W" dealt "C_R"%.1lf"C_W" damage to "C_R"%s"C_W" using a%s "C_C"%s"C_W". "C_R"%s"C_W" now has %.1lf/%.1lf health remaining.\n",
            p->name, damage, target->curr->name, is_vowel(i->name[0])?"n":"", i->name, target->curr->name, target->curr->life >= 0?target->curr->life:0,
            target->curr->lifeTotal);
    if(target->curr->life < 0.1) target->curr->dief(target, p);
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
        printf("ERROR: you do not exist (this should not happen)\n");
        return;
    }
    if(i->quantity < 1){
        printf("You have no more "C_C"%s"C_W " remaining\n",i->plName);
        return;
    }
    i->quantity -= 1;
    double heal = cap(i->effect, target->curr->lifeTotal - target->curr->life);
    //printf("healing %p: %s\n",target->curr, target->curr->name);
    target->curr->life += heal;
    printf(C_Y"%s"C_W" healed "C_R"%s"C_W" for "C_Y"%.1lf"C_W" health. "C_R"%s"C_W" %s %.1lf/%.1lf health remaining.\n",
            p->name,
            target->curr->name,
            heal,
            target->curr == p ? "You":target->curr->name,
            target->curr == p ? "now have":"now has",
            target->curr->life, target->curr->lifeTotal);

    printf("You have "C_Y"%d"C_W" remaining "C_C"%s"C_W".\n",
            i->quantity,
            i->quantity == 1?i->name:i->plName);
}
