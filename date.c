#include "date.h"
#include <stdlib.h>

struct date* allocDate(int taille){
	struct date *D = (struct date*)malloc(sizeof(struct date));

	if (D){
		D->n = taille;
		D->V = (float*)calloc(taille, sizeof(float));

		if (!D->V){	// Erreur allocation vecteur.
			libererDate(&D);
		}
	}

	return D;
}

void libererDate(struct date** D){
	if (*D){
		free((*D)->V);
		free(*D);
		*D = NULL;
	}
}

int dateTot(float a, float b){
	return a < b;
}

int dateTard(float a, float b){
	return a > b;
}