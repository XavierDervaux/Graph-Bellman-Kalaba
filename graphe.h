#ifndef _GRAPHE
#define _GRAPHE


#include "noeuds.h"
#include "bitmap.h"
#include "date.h"
#include <stdio.h>

	struct graphe {
		float **M;
		int n, m;
		struct noeuds *name;
	};
	
/*
	L'include ci-dessous est là pour éviter ce warning :
		"its scope is only definition or declaration, wich is probably not what you want"
*/	
#include "marge.h"

/*
	Les defines des erreurs potenciels qui peuvent être rencontrées :
*/
#define ERROR_ALLOC_NOEUD -1
#define ERROR_OPEN_FILE -2
#define ERROR_ALLOC_GRAPHE -3
#define ERROR_GRAPH_CYCLIQUE -4
#define ERROR_GRAPH_ENTREE -5
#define ERROR_CYCLE_ALLOC_BITMAP -6
#define ERROR_GRAPH_SORTIE -7
#define ERROR_ALLOC_DATE_TOT -8
#define ERROR_ALLOC_DATE_TARD -9
#define ERROR_ALLOC_MARGE -10

/*
	Fonctions d'allocation, de libération et d'initialisation et d'impression.
*/
struct graphe* allocGraphe (int nbrnoeud);
void libererGraphe (struct graphe**, int nbrnoeud);	
void initGraphe	(struct graphe *g);
void imprimerGraphe (FILE *fs, struct graphe *g);

/*
	Fonctions de vérification cyclique, d'entrée et de sortie.
*/
int verificationCyclique(struct graphe*);
int trouverEntree (struct graphe *g, struct bitmap *bit);

int verificationEntree(struct graphe*);
int verificationSortie(struct graphe*);

/*
	Bellman Kalaba Simplifié
*/
struct date* bellmanKalabaS(struct graphe* g, int input, float valueInput, float flt, int type, 
		int (*rechNoeud)(struct graphe*, struct bitmap*));
int rchNoeudLong(struct graphe* g, struct bitmap *bit);
int rchNoeudCourt(struct graphe* g, struct bitmap *bit);


/*
	Les fonctions principales du programme
*/
int traitementGraphe (char *fichierEntree, char *fichierSortie);
int chargementGraphe(struct graphe**, char *nomFichier);
int verificationGraphe (struct graphe*, int *entree, int *sortie);
int traitementBellmanKalabaS (struct graphe* g,struct date **tot, struct date** tard, int entree, int sortie);
int ecrireSortieGraphe (char *nfs,struct graphe *g, struct date *tot, struct date *tard, struct marge* marg, int entree, int sortie);
	
#endif