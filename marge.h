#ifndef _MARGE
#define _MARGE

#include <stdio.h>
#include "stack.h"
#include "date.h"
	
	struct marge {
		float ml, mt;
		int nd, na;
	};
	
/*
	L'include ci-dessous est là pour éviter un warning :
		"its scope is only definition or declaration, wich is probably not what you want"
*/
#include "graphe.h"

#define ERROR_ALLOC_STACK -11

/*
	Fonctions sur les marges
*/

void calculerMarge(struct graphe*, struct date* tot, struct date *tard, struct marge*);
void imprimerMarge (FILE *fs, struct noeuds* nd, struct marge* , int taille);

/*
	Fonctions sur le chemin critique
*/
int initCheminCritique (FILE *fs, struct graphe*, struct marge*, int entree, int sortie);
void rechercheCheminCritique(FILE *fs, struct graphe*, struct stack*, struct marge*, int sortie);

void verificationBackTracking(struct stack*, int *pos,int *i,int *j);
int siArcCritique(struct noeuds*, struct marge*, int born, int depart, int destination);
void imprimerCheminCritique(FILE *fs,struct noeuds*, struct stack*);

#endif