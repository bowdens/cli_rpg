#include "game.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


Race *make_race(char *name, char *plName, char *desc){
    Race *r = malloc(sizeof(Race));
    assert(r);
    strncpy(r->name, name, MAX_INV_NAME);
    strncpy(r->plName, plName, MAX_INV_NAME);
    strncpy(r->desc, desc, MAX_INV_DESC);
    return r;
}

int same_race(Race *race, char *name){
    //printf("testing %s against %s\n", race->name, name);
    return strcmp(name,race->name) == 0;
}

Race *find_race(char *name){
    if(name == NULL) return NULL;
    if(same_race(gl_race_human, name)) return gl_race_human;
    if(same_race(gl_race_voidwretch, name)) return gl_race_voidwretch;
    if(same_race(gl_race_kaskeer, name)) return gl_race_kaskeer;
    return NULL;
}

Race *gl_race_human = NULL;

Race *race_human(void){
    if(gl_race_human){
        return gl_race_human;
    }else{
        gl_race_human = make_race("human", "humans",
                "Mostly consisting of workers and farmers, humans are commonly found throughout the land");
        return gl_race_human;
    }
}

Race *gl_race_voidwretch = NULL;

Race *race_voidwretch(void){
    if(gl_race_voidwretch){
        return gl_race_voidwretch;
    }else{
        gl_race_voidwretch = make_race("void wretch", "void wretches",
                "A horrific underground creature that has mastered human speech. Known for forcing those unlucky enough to encounter one to solve a riddle to pass");
        return gl_race_voidwretch;
    }
}

Race *gl_race_kaskeer = NULL;

Race *race_kaskeer(void){
    if(gl_race_kaskeer){
        return gl_race_kaskeer;
    }else{
        gl_race_kaskeer = make_race("kaskeer", "kaskeers",
                "A large rat like creature with no eyes but an acute sense of smell.");
        return gl_race_kaskeer;
    }
}

Race *gl_race_cavegolem = NULL;

Race *race_cavegolem(void){
    if(gl_race_cavegolem){
        return gl_race_cavegolem;
    }else{
        gl_race_cavegolem = make_race("cave golem", "cave golems",
                "First raised by dark magicians eons ago, cave golems can now be found in some of the deeper cave systems");
        return gl_race_cavegolem;
    }
}
