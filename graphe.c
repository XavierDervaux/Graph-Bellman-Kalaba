#include "graphe.h"
#include <float.h>
#include <stdlib.h>

struct graphe* allocGraphe (int nbrnoeud){
	int i = -1;
	struct graphe* g= NULL;
	
	g = (struct graphe*)malloc(sizeof(struct graphe));
	if (g){
		g->M = (float**)calloc(nbrnoeud,sizeof(float*));
		
		if (g->M){
			do{
				i++;
				g->M[i] = (float*)malloc(sizeof(float)*nbrnoeud);
			} while (g->M[i] != NULL && i<nbrnoeud-1);
			if (!g->M[i]){
				libererGraphe(&g, nbrnoeud);
			}
			else{// Le graphe est correctement alloué
				g->name = allocNoeuds(nbrnoeud, 9);
				if (!g->name){
					libererGraphe(&g, nbrnoeud);
				}	
			}
		}
	}
	return g;
}

void libererGraphe(struct graphe** g, int nbrnoeud){
	int i;
	if (*g){
		if ((*g)->M){
			for (i = 0; i < nbrnoeud; i++)
				free((*g)->M[i]);
			free((*g)->M);
		}
		free(*g);
		*g = NULL;
	}
}

/*
	Initialise l'intégralité du graphe
	en FLT_MAX
*/
void initGraphe(struct graphe *g){
	int i, j;
	for (i = 0; i < g->n; i++){
		for (j = 0; j < g->n; j++){
			g->M[i][j] = FLT_MAX;
		}
	}
}

/*
	Imprime un graphe dans un fichier.
*/
void imprimerGraphe (FILE *fs, struct graphe *g){
	int i, j;
	
	fprintf(fs,"          |");
	for (i = 0; i < g->n; i++){
		fprintf(fs, "%9s |", g->name->M[i]);
	}
	fprintf(fs,"\n");
	for (i = 0; i < g->n; i++)
	{
		fprintf(fs, "%9s |", g->name->M[i]);
		for (j = 0; j < g->n; j++){
			if (g->M[i][j] == FLT_MAX)
				fprintf(fs, "      .   |");
			else
				fprintf(fs, "%9.2f |", g->M[i][j]);
		}
		fprintf(fs, "\n");
	}
}


/****************************************************************/
/*						Fonctions de vérificaton				*/
/****************************************************************/

/*
	Vérifie que le graphe n'est pas cyclique, il vérifie par les entrées : 
		sortie :
			0 : non cyclique 
			1 : cyclique		
			ERROR_CYCLE_ALLOC_BITMAP : erreur allocation de la bitmap
*/
int verificationCyclique(struct graphe *g){
	int i = 0, trouver, nbrBit = 1;
	struct bitmap* bit = allocBitmap(g->n);
	
	if (bit){
		while((trouver = trouverEntree(g, bit)) > -1 && i < g->n){	
				bit->V[trouver] = 1;
				i++;
		}
		if(i == g->n)
			nbrBit = 0;
		libererBitmap(&bit);
	}
	else
		nbrBit = ERROR_CYCLE_ALLOC_BITMAP;
	return nbrBit;
}

/*
	Trouve une entrée en ignorant certaines lignes et colonnes grâce à une bitmap:
		En sortie :
			-1	 : pas d'entrée
			> -1 : position de l'entrée  
*/
int trouverEntree(struct graphe *g, struct bitmap *bit){
	int i, j = 0;
	int trouver = -1, nbrNoeud = 0, arc=0;
	while (!nbrNoeud && j < g->n){
		if (!bit->V[j]){
			arc = 0;
			i = 0;
			while (i < g->n  && arc < 1){
				if (g->M[i][j] != FLT_MAX && !bit->V[i]){
					arc++;
				}
				i++;
			}
			if (!arc){
				nbrNoeud++;
				trouver = j;
			}
		}
		j++;
	}
	return trouver;
}

/*
	Vérifie que le graphe possède qu'une seule et unique entrée :
		En sortie :
			-1	 : 0 ou plusieurs entrées
			> -1 : position de l'entrée  
*/
int verificationEntree(struct graphe *g){
	int i, j=0;
	int nbrEntree = 0, next, res;
	
	/*
		nbrEntree < 2 car une fois qu'il a trouvé une entrée, 
		il vérifie qu'il n'en existe pas au moins une supplémentaire !
	*/
	while (nbrEntree < 2 && j < g->n){
		next = 0;
		i = 0;
		while (!next && i < g->n){
			if (g->M[i][j] != FLT_MAX){
				next++;
			}
			i++;
		}
		if (!next){
			nbrEntree++; 
			res = j;
		}
		j++;
	}
	
	if(nbrEntree != 1){
		res = -1;
	}
	return res;
}

/*
	Vérifie que le graphe posséde qu'une seule et unique sortie :
		En sortie :
			-1	 : 0 ou plusieurs entrées
			> -1 : position de l'entrée  
*/
int verificationSortie(struct graphe *g){
	int i, j = 0;
	int nbrSortie = 0, next, res;
	
	/*
		nbrSortie < 2 car une fois qu'il a trouvé une entrée, 
		il vérifie qu'il n'en existe pas au moins une supplémentaire !
	*/
	while (nbrSortie < 2 && j < g->n){
		next = 0;
		i = 0;
		while (!next && i < g->n){
			if (g->M[j][i] != FLT_MAX){
				next++;
			}
			i++;
		}
		if (!next){
			nbrSortie++;
			res = j;
		}
		j++;
	}
	
	if(nbrSortie != 1){
		res = -1;
	}
 	return res;
}


/****************************************************************/
/*						Bellman Kalaba Simplifié				*/
/****************************************************************/

/*
	bellmanKallabaS 
	
	Entrée :
		- [struct graphe*] graphe : Structure du graphe
		- [int] input : La position de l'entrée ou de la sortie
		- [float] valueInput : la valeur de l'entrée ou de la sortie
		- [float] flt : la valeur maximum (FLT_MAX) ou minimum (-FLT_MAX)
		- [int (struct graphe*, struct bitmap*)] rechNoeud : fonction rchNoeudCourt ou rchNoeudLong
		- [int] type : 0 défini les dates au plus tôt, 1 défini les dates au plus tard.
	Sortie :
		- Retourne une Structure date qui sera null en cas d'erreur d'allocation
*/
struct date* bellmanKalabaS(struct graphe* g, int input, float valueInput, float flt, int type, 
	int (*rechNoeud)(struct graphe*, struct bitmap*)){
		
	int i = 0, j=0;
	float temps;
	struct bitmap *bit = allocBitmap(g->n);
	struct date *dat = allocDate(g->n);

	if (bit && dat){
		bit->V[input] = 1;
		dat->V[input] = valueInput;
		
		while ((i = rechNoeud(g, bit)) != -1){
			temps = flt;
			for (j = 0; j < g->n; j++){
				if(type){
					if (g->M[i][j] != FLT_MAX && dateTot(dat->V[j] - g->M[i][j], temps))
						temps = dat->V[j] - g->M[i][j];
				}
				else{
					if (g->M[j][i] != FLT_MAX && dateTard(dat->V[j] + g->M[j][i], temps))
						temps = dat->V[j] + g->M[j][i];
				}
			}

			dat->V[i] = temps;
			bit->V[i] = 1;
		}
		libererBitmap(&bit);
	}

	return dat;
}

/*
	L'algorithme parcourt les lignes puis les colonnes.
	Il recherche un noeud qui n'est pas marqué dont tous ces prédécesseurs sont marqués.

	Entrée :
		- [struct graphe*] graphe : Structure du graphe
		- [struct bitmap*] bit : Structure de bitmap
	Sortie :
		- [int] retourne -1 s'il ne trouve pas de noeud à optimiser
		- [int] retoune la position du noeud à optimiser 
*/
int rchNoeudLong(struct graphe* g, struct bitmap *bit){
	int i, j = 0, trouver = -1, mark;
	while (trouver < 0 && j < g->n){
		if (!bit->V[j]){
			i = 0;
			mark = 0;
			while (i < g->n && !mark){
				if (g->M[i][j] != FLT_MAX){
					if (!bit->V[i])
						mark = 1;
				}
				i++;
			}
			if (!mark)
				trouver = j;
		}
		j++;
	}
	return trouver;
}

/*
	L'algorithme parcourt les colonnes puis les lignes pour éviter l'inversion d'une bitmap.
	Il recherche un noeud qui n'est pas marqué dont tous ces prédécesseurs sont marqués.

	Entrée :
		- [struct graphe*] graphe : Structure du graphe
		- [struct bitmap*] bit : Structure de bitmap
	Sortie :
		- [int] retourne -1 s'il ne trouve pas de noeud à optimiser
		- [int] retoune la position du noeud à optimiser 
*/
int rchNoeudCourt(struct graphe* g, struct bitmap *bit){
	int i=0, j, trouver = -1, mark;
	while (trouver < 0 && i < g->n){
		if (!bit->V[i]){
			j = 0;
			mark = 0;
			while (j < g->n && !mark){
				if (g->M[i][j] != FLT_MAX){
					if (!bit->V[j])
						mark = 1;
				}
				j++;
			}
			if (!mark)
				trouver = i;
		}
		i++;
	}
	return trouver;
}


/****************************************************************/
/*			  Les fonctions principales du programme			*/
/****************************************************************/

int traitementGraphe (char *fichierEntree, char *fichierSortie){
	int erreur = 0;
	int entree = -1, sortie = -1;
	
	struct graphe *g = NULL;
	struct date *tot = NULL, *tard = NULL;
	struct marge* marg = NULL;

	if(!(erreur = chargementGraphe(&g, fichierEntree))){
		if(!(erreur = verificationGraphe(g,&entree,&sortie))){
			if(!(erreur = traitementBellmanKalabaS (g,&tot,&tard,entree, sortie))){
				if((marg = (struct marge*)malloc(sizeof(struct marge)*g->m))){
					
					erreur = ecrireSortieGraphe(fichierSortie, g, tot, tard, marg, entree, sortie);
				}
				else{
					erreur = ERROR_ALLOC_MARGE;
				}
			}
		}
		/*
			Liberation des ressources, même s'il y a eu des erreurs avant l'allocation de toutes les ressources, 
			les fonctions gérent les ensembles NULL,ce qui permet de ne pas gérer chaque cas de figure dans des if / else.
		*/
		free(marg);
		libererDate(&tot);
		libererDate(&tard);
		libererNoeuds(&g->name);
		libererGraphe(&g, g->n);
	}
	return erreur;
}

int chargementGraphe(struct graphe **g, char *nomFichier){
	int i, n, m, posI, posJ, erreur = 0;
	float l;
	char *noeud;
	FILE *fd;
	
	fd = fopen(nomFichier, "r");

	if (!fd){
		erreur = ERROR_OPEN_FILE; 
	}
	else {
		noeud = (char*)malloc(sizeof(char) * 9);	
		if (!noeud){
			erreur = ERROR_ALLOC_NOEUD;
		}
		else{
			fscanf(fd, "%d %d", &n, &m);
			*g = allocGraphe(n);
			
			if (!(*g)){
				erreur = ERROR_ALLOC_GRAPHE;
			}
			else {
				(*g)->n = n;
				(*g)->m = m;
				initGraphe(*g);
				for (i = 0; i < m; i++){
					fscanf(fd, "%s",noeud);
					posI = isExistName((*g)->name, noeud);
					fscanf(fd, "%s %f", noeud, &l);
					posJ = isExistName((*g)->name, noeud);
	
					if (posI != -1 && posJ != -1)
						(*g)->M[posI][posJ] = l;
				}
				free(noeud);
			}
			fclose(fd);	
		} 
	}
	return erreur;
}

int verificationGraphe (struct graphe *g, int *entree, int *sortie){
	int erreur = 0;

	if ((erreur = verificationCyclique(g)) == 1){
		erreur = ERROR_GRAPH_CYCLIQUE;
	}
	else if (!erreur && (*entree = verificationEntree(g)) < 0){
		erreur = ERROR_GRAPH_ENTREE;
	}
	else if (!erreur && (*sortie = verificationSortie(g)) < 0){
		erreur = ERROR_GRAPH_SORTIE;
	}
	return erreur;
}

int traitementBellmanKalabaS (struct graphe* g,struct date **tot, struct date** tard, int entree, int sortie){
	int erreur = 0;
	if(*tot = bellmanKalabaS(g, entree,0, -FLT_MAX, 0,rchNoeudLong)){
		if(!(*tard = bellmanKalabaS(g, sortie, (*tot)->V[sortie], FLT_MAX, 1,rchNoeudCourt))){
			erreur = ERROR_ALLOC_DATE_TARD;
		}
	}
	else{
		erreur = ERROR_ALLOC_DATE_TOT;
	}
	return erreur;
}

int ecrireSortieGraphe (char *nfs,struct graphe *g, struct date *tot, struct date *tard, struct marge* marg, int entree, int sortie){
	int i, erreur=0;

	FILE *fs = fopen(nfs, "w");

	if (fs){
		imprimerGraphe(fs,g);
		
		//Trie les noeuds par ordre alphabétique
		tri(g->name, 0, g->n-1);
		
		/*
			Imprime les dates : 
				A cause de la limitation du procédurale, on a décidé de mettre l'impression des dates ici pour éviter les
				multiples appels dans date.h
		*/
		fprintf(fs, "\n\nNoeuds    | Dates au plus tot | Date au plus tard\n");
		fprintf(fs, "----------------------------------------------\n");
		for (i = 0; i < g->name->n; i++){
			fprintf(fs, "%9s | %9.2f         | %9.2f \n", g->name->M[i], tot->V[g->name->pos[i]], tard->V[g->name->pos[i]]);
		}
		
		calculerMarge(g, tot, tard, marg);
		imprimerMarge (fs,g->name,marg, g->m);
		
		fprintf(fs, "\n\nchemin(s) critique(s):\n");
		fprintf(fs, "----------------------\n");
		erreur = initCheminCritique(fs, g, marg, entree, sortie);
		
		fclose(fs);
	}
	return erreur;
}