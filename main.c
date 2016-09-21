#include "graphe.h"
#include "fichier.h"

/*
	Programme réalisé par :
		Bianco Andy
		Dervaux Xavier
		
	Le programme a été entiérement réalisé sous linux et compiler sous gcc
	Valgrind à détecter aucune erreur.
*/
int main (int argc, char **argv){
	int erreur = 0;
	
	erreur = traitementGraphe(argv[1],argv[2]);
	
	ecrireErreur(argv[2],erreur);
	
	return 0;
} 
