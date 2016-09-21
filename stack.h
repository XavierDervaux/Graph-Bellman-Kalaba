#ifndef _STACK
#define _STACK

//Initialiser la structure avec la fonction stackInit(NombreArc);
struct stack{
	/*
		A chaque emplacement de la matrice est stocké la ligne et la colonne 
		d'arrêt au moment du push.
	*/
	int **M;	
	int n, i;
};

struct stack* allocStack(int taille);
void libererStack (struct stack** s, int taille);

int emptyS (struct stack *s);

void pushS (struct stack *s, int ligne , int pos);
void popS (struct stack *s);

int getS (struct stack *s, int pos);


#endif