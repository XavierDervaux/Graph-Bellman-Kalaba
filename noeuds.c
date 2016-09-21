#include "noeuds.h"
#include <stdlib.h>
#include <string.h>

struct noeuds* allocNoeuds(int nbrNoeud, int taille){
	struct noeuds* noeud = (struct noeuds*)malloc(sizeof(struct noeuds));
	int i = -1;
	
	if (noeud){
		noeud->n = nbrNoeud;
		noeud->M = (char**)calloc(nbrNoeud, sizeof(char*));
		noeud->pos = (int*)calloc(nbrNoeud, sizeof(int));
		if (noeud->M != NULL && noeud->pos){
			do{
				i++;
				noeud->M[i] = (char*)calloc(taille,sizeof(char));
				
			} while (noeud->M[i] && i<nbrNoeud-1);

			if (!noeud->M[i])
				libererNoeuds(&noeud);
		}
		else
			libererNoeuds(&noeud);
	}
	return noeud;
}

void libererNoeuds(struct noeuds** noeud){
	int i;
	if (*noeud){
		if ((*noeud)->pos){
			free((*noeud)->pos);
		}
		if ((*noeud)->M){
			for (i = 0; i < (*noeud)->n; i++){
				free((*noeud)->M[i]);
			}
			free((*noeud)->M);
		}
		free(*noeud);
		*noeud = NULL;
	}
}

/*
	Regarde si le nom du noeud existe déja :
		 - si oui il retourne la position
		 - sinon il ajoute le noeud dans la 1er position libre rencontré 
		 et ajoute la position dans un tableau pour retrouver sa position initiale 
		 au moment du tri. 
*/
int isExistName(struct noeuds *noeud, char *name){
	int i = 0, trouve = -1;
	while (trouve < 0 && i < noeud->n){
		if (strcmp(noeud->M[i], "\0") ==  0){
			strcpy(noeud->M[i], name);
			noeud->pos[i] = i;
			trouve = i;
		} 
		else if (strcmp(noeud->M[i], name) == 0)
			trouve = i;
		i++;
	}
	return trouve;
}

/*
	Tri permet d'optimiser le tri des donnèes :
		Si le nombre de données à trié est > 10 => quiSort est utilisé.
		Si le nombre de données à trié est < 10 => trinLin est utilisé.
*/
void tri(struct noeuds* n, int l, int r){
	if (r - l > 10){
		quickSort(n, l, r);
	} else {
		triLin(n, l, r);
	}
}

/*
	L'algorithme de tri linéaire.
*/
void triLin(struct noeuds* n, int l, int r){
	int i, j, tmp;
	char tmp2[9];

	for (i = l+1; i <= r; i++){
		tmp = n->pos[i];
		strcpy(tmp2,n->M[i]);
		j = i;
		while (j > l && strcmp(n->M[j - 1], tmp2) > 0){
			n->pos[j] = n->pos[j-1];
			strcpy(n->M[j],n->M[j - 1]);
			j--;
		}
		n->pos[j] = tmp;
		strcpy(n->M[j], tmp2);
	}
}

/*
	L'algorithme de Quick Sort.
*/
void quickSort(struct noeuds* n, int l, int r){
	int i, j, tmp;
	char p[9], tmp2[9];
	i = l;
	j = r;
	strcpy(p,n->M[(i + j) / 2]);
	do{
		while (strcmp(n->M[i],p) < 0){
			i++;
		}
		while (strcmp(n->M[j],p) > 0){
			j--;
		}

		if (i <= j){
			tmp = n->pos[i];
			strcpy(tmp2,n->M[i]);
			n->pos[i] = n->pos[j];
			strcpy(n->M[i],n->M[j]);
			n->pos[j] = tmp;
			strcpy(n->M[j],tmp2);
			i++;
			j--;
		}
	} while (i <= j);
	if (l < j){
		tri(n, l, j);
	}
	if (r > i){
		tri(n, i, r);
	}
}

/*
	Retourne un nom de neoud trié d'après sa position d'origine
*/
char* getNomNoeudTriee(struct noeuds* nd, int valeur){
	int i = 0, trouver = -1;
	while (i < nd->n && trouver < 0){
		if (nd->pos[i] == valeur)
			trouver = i;
		i++;
	}
	return nd->M[trouver];
}