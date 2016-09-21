#include "stack.h"
#include <stdlib.h>	

struct stack* allocStack(int taille){
	int i = -1;
	
	struct stack* s = (struct stack*)malloc(sizeof(struct stack));
	if (s){
		s->M = (int**)calloc(taille, sizeof(int*));
		if(s->M){
			do{
				i++;
				s->M[i] = (int*)malloc(sizeof(int)*2);
			} while (s->M[i] != NULL && i<taille-1);
			
			if (!s->M[i])
				libererStack(&s,taille);
			else{
				s->n = taille;
				s->i = -1;
			}
		}
		else
			libererStack(&s,taille);
	} 

	return s;
}

void libererStack (struct stack** s, int taille){
	int i;
	if (*s){
		if ((*s)->M){
			for (i = 0; i < taille; i++)
				free((*s)->M[i]);
			free((*s)->M);
		}
		free(*s);
		*s = NULL;
	}
}

int emptyS (struct stack *s){
	return s->i<0;
}

void pushS (struct stack *s, int ligne , int pos){
	s->i++;
	s->M[s->i][0] = ligne;
	s->M[s->i][1] = pos;	
}

void popS (struct stack *s){
	if(!emptyS(s)){
		s->i--;
	} 	
}

int getS (struct stack *s, int pos){
	return s->M[s->i][pos];
}