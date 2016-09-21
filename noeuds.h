#ifndef _NOEUDS
#define _NOEUDS
	struct noeuds{
		char **M;

		int *pos; //Permet de stocker les positions initiales du noeud avant le tri. 
		int n;
	};
	
	struct noeuds* allocNoeuds(int nbrNoeud, int taille);
	void libererNoeuds(struct noeuds** noeud);
	int isExistName(struct noeuds *noeud, char *name);
	
	void tri(struct noeuds* n, int l, int r);
	void triLin(struct noeuds* n, int l, int r);
	void quickSort(struct noeuds* n, int l, int r);
	
	char* getNomNoeudTriee(struct noeuds* nd, int valeur);
#endif