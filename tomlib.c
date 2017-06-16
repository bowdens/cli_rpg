#include "tomlib.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
/*----------------------
  Mathematical functions
  ----------------------*/
double sigmoid(double x){
	return (double) 1/(exp(-x)+1);
}

int power_of_two(int x){
	return (x & (x-1)) == 0;
}

/*----------------
  String Functions
  ----------------*/

//input string, returns string with rANdOmUPPErcAse
void random_case_string(char *c){
	if(c == NULL) return;
	for(int i = 0; i < strlen(c) + 1; i ++){
		c[i] = random_case(c[i]);
	}
}

//input a character, return a character with RaNDOMuppERCaSe
char random_case(char c){
	//srand(time(NULL));
	if(rand()%2){
		if(c >= 'a' && c<= 'z'){
			return c - 32;
		}else if(c >= 'A' && c <= 'Z'){
			return c + 32;
		}
	}
	return c;
}

int is_vowel(char c){
	c = tolower(c);
	return(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}


