
#include "marge.h"
#include <float.h>

/*
	Calcul des marges libres et des marges totales en tenant  compte que les noms des noeuds sont déja triés
	par ordre alphabétique.
*/
void calculerMarge(struct graphe *g, struct date *tot, struct date *tard, struct marge* marg){
	int i, j, coli, colj, k = 0;
	for (i = 0; i < g->n; i++){
		coli = g->name->pos[i];
		for (j = 0; j < g->n; j++){
			colj = g->name->pos[j];
			if (g->M[coli][colj] != FLT_MAX){
				marg[k].ml = tot->V[colj] - g->M[coli][colj] - tot->V[coli];
				marg[k].mt = tard->V[colj] - g->M[coli][colj] - tot->V[coli];
				marg[k].nd = i;
				marg[k].na = j;
				k++;
			}
		}
	}
}

/*
	Imprime les marges dans un fichier passé en paramètre.
	Entrée :
		[FILE*] fs : Le fichier de sortie
		[struct noeud*] nd : Les noeuds du graphe
		[struc marge*] marge : Les marges à afficher
		[int] taille : Le nombre d'arc (g->m)
*/
void imprimerMarge (FILE *fs, struct noeuds* nd, struct marge* marg, int taille){
	int i;
	fprintf(fs, "\n\ndepart    | arrivee  | marge libre | marge total\n");
	fprintf(fs, "--------------------------------------------\n");
	for (i = 0; i < taille; i++){
		fprintf(fs,"%9s |%9s |%9.2f    |%9.2f\n", nd->M[marg[i].nd], nd->M[marg[i].na], marg[i].ml, marg[i].mt);
	}
}

/****************************************************************************************************************/
/*                                           Chemin(s) Critique(s)                                              */
/****************************************************************************************************************/

/*
	Pourquoi les fonctions du chemin critique dans marge.h ?
		En théorie le chemin critique utilise les marges totales
		En plus de la limitation du procédurale, nous avons donc décidé de le mettre dans le fichier marge.
*/



/*
	Alloue le stack et push l'entrée.
*/
int initCheminCritique (FILE *fs, struct graphe *g, struct marge *marg, int entree, int sortie){
	int erreur = 0;
	struct stack* s = allocStack(g->m);

	if (s){
		pushS(s, entree,0);
		rechercheCheminCritique(fs, g, s, marg, sortie);
		libererStack(&s, g->m);
	}
	else
		erreur = ERROR_ALLOC_STACK;

	return erreur;
}

void rechercheCheminCritique(FILE *fs, struct graphe *g, struct stack *s, struct marge *marg, int sortie){
	int i = -1,j, trouver;
	/*
	 	tmp est incrémenté à chaque fois qu'on push. Cela permet de détecter si on fait
		 du backtracking ou non. Si la taille du stack (s->i) < tmp alors on retourne en arriére.
	*/
	int tmp = 0; 
	
	//tant que le stack n'est pas vide
	while(s->i != 0 || i == -1){//s->i != 0 || i == -1
		do{
			trouver = 0;
			//Verifie si on a depop à l'itération précédente. 
			verificationBackTracking(s,&tmp,&i,&j);		
			//tant qu'on a pas trouvé d'arc critique ou qu'on n'est pas sorti des bornes
			while( j < g->n && !trouver){
				//Si on trouve un arc
				if (g->M[i][j] != FLT_MAX){
					//s'il est critique alors j'indique qu'il est trouvé et je le push
					if (siArcCritique(g->name,marg, g->m, i, j)){
						/*
							Le if permet de supprimer les doublons quand il trouve plusieurs marges critiques sur
							un même noeud.  
						*/
						if(i == s->M[s->i][0] && s->i != 0){ 
							tmp--;
							popS(s);
						}
							
						pushS(s, i, j);
						trouver = 1;
						tmp++;
					}
				}
				j++;
			}
					
		}while(j < g->n || trouver); //Permet d'éviter de sortir de la boucle quand on push à la position n.
		if(s->M[s->i][1] == sortie){
			imprimerCheminCritique(fs, g->name, s);
		}
		popS(s);
	}
}

/*
	Regarde si on est en back tracking ou non:
		-Si s->i < pos: On est en backtracking et on a besoin de la position de la ligne
		-Sinon : On doit parcourir toute la ligne, on retourne donc 0.
*/
void verificationBackTracking(struct stack*s, int *pos,int *i,int *j){
	//Si s->i < pos alors on retourne en arriére
	if(s->i < *pos){
		//On récupére la ligne à parcourir et la position à laquelle on s'était arrêté 
		*i = getS (s,0);
		*j = getS (s,1)+1; //+1 car sinon on retrouvera l'arc critique qui à pop juste avant.
		*pos = *pos -1;
	}
	else{
		//On initialise les variables pour parcourir une ligne
		if(*i == -1)		//Si on rentre pour la 1er fois, on get la ligne
			*i = getS(s,0);
		else				//sinon on récupére la position du noeud d'arrivée de l'arc critique
			*i = getS(s,1);
		
		*j = 0;
	}
}

/*
	Recherche le noeud de départ de l'arc et celui d'arrivée.
	Ensuite on vérifie que l'arc est critique. 
*/
int siArcCritique(struct noeuds* nd, struct marge *marg, int born, int depart, int destination){
	int i = 0, trouver = 0;
	while (i < born && !trouver){
		if (nd->pos[marg[i].nd] == depart && nd->pos[marg[i].na] == destination){
			if (!(int)marg[i].mt){
				trouver = 1;
			}
		}
		i++;
	}
	return trouver;
}

/*
	Imprime le chemin critique dans le fichier de sortie
*/
void imprimerCheminCritique(FILE *fs,struct noeuds *nd, struct stack *s){
	int i;
	for (i = 1; i <= s->i; i++){
		fprintf(fs,"-> %s", getNomNoeudTriee(nd, s->M[i][0]));
	}
	fprintf(fs,"-> %s", getNomNoeudTriee(nd, s->M[s->i][1]));
	fprintf(fs,"\n");
}