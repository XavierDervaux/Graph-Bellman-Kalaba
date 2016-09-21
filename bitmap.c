#include "bitmap.h"
#include <stdlib.h>

struct bitmap* allocBitmap(int taille){
	struct bitmap* bit = (struct bitmap*)malloc(sizeof(struct bitmap));
	if (bit){
		bit->V = (int*)calloc(taille, sizeof(int));
		bit->n = taille;
		
		if (!bit->V){	// Erreur allocation du vecteur de bitmap.
			libererBitmap(&bit);
		}
	}
	else{	//Erreur allocation structure
		libererBitmap(&bit);
	}

	return bit;
}

void libererBitmap(struct bitmap** bm){
	if (*bm){
		free((*bm)->V); //Pas besoin de vérification, free gére le NULL
		free(*bm);
		*bm = NULL;
	}
}