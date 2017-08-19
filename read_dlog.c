#include <stdio.h>
#include "game.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_SIZE 10000

char *shift(char *str, int dist){
    dist = (dist<strlen(str)) ? dist : strlen(str);
    if(shift <= 0) return str;
    for(int l = 0; l < dist; l ++){
        for(int i = 0; i < strlen(str); i ++){
            str[i] = str[i+1];
        }
    }
    return str;
}

int line_num(char *str){
    //printf("line_num called for \"%s\"\n",str);
    char local[MAX_LINE_SIZE] ={0};
    strcpy(local,str);
   // printf("local = %s\n",local);

    while(isblank(local[0])){
        //printf("first of %s is blank\n'%c' is a blan char\n",local, local[0]);
        shift(local, 1);
        //printf("after shifting, local = %s\n",local);
    }
    char digit[MAX_LINE_SIZE] = {0};
    for(int i = 0; i < strlen(local) && isdigit(local[i]); i ++){
        digit[i] = local[i];
    }
    if(digit[0] == '\0') return -1;
    return atoi(digit);
}

int is_comment(char *buffer){
    int i = 0;
    for(; i < strlen(buffer) && buffer[i] != '#'; i ++);
    return i != strlen(buffer);
}

void read_dlog(char *filename){
    char buffer[MAX_LINE_SIZE];
    FILE *f = fopen(filename, "r");
    assert(f);
    while(fgets(buffer, MAX_LINE_SIZE,f)){
        if(buffer == NULL) break;
        if(!is_comment(buffer)){
            printf("%s",buffer);
            printf("above line id = %d\n",line_num(buffer));
        }
    }
}
