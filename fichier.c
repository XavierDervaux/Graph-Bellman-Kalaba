#include "fichier.h"

#include "graphe.h"
#include "marge.h" 

void ecrireErreur (char *nfs,int erreur){
	switch (erreur){
	case ERROR_ALLOC_NOEUD:
		fichierSortie(nfs, "[Erreur] Echec d'allocation de char* dans chargementFichier();.\n");
		break;
	case ERROR_OPEN_FILE:
		fichierSortie(nfs, "[Erreur] Echec d'ouverture du fichier d'entrée dans chargementFichier();.\n");
		break;
	case ERROR_ALLOC_GRAPHE:
		fichierSortie(nfs, "[Erreur] Echec de l'allocation du graphe dans allocGraphe();.\n");
		break;
	case ERROR_GRAPH_CYCLIQUE:
		fichierSortie(nfs, "[Erreur] Le graphe est cyclique\n");
		break;
	case ERROR_GRAPH_ENTREE:
		fichierSortie(nfs, "[Erreur] Le graphe à plusieurs entrées\n");
		break;
	case ERROR_CYCLE_ALLOC_BITMAP:
		fichierSortie(nfs, "[Erreur] Echec allocation bitmap* dans verificationCyclique();.\n");		
		break;
	case ERROR_GRAPH_SORTIE:
		fichierSortie(nfs, "[Erreur] Le graphe à plusieurs sortie\n");
		break;
	case ERROR_ALLOC_DATE_TOT:
		fichierSortie(nfs, "[Erreur] Echec d'allocation des dates au plus tot dans bellmanKalabaS();\n");
		break;
	case ERROR_ALLOC_DATE_TARD:
		fichierSortie(nfs, "[Erreur] Echec d'allocation des dates au plus tard dans bellmanKalabaS();\n");
		break;
	case ERROR_ALLOC_MARGE:
		fichierSortie(nfs, "[Erreur] Echec d'allocation pour les marges dans traitementGraphe();\n");
		break;
	case ERROR_ALLOC_STACK:
		fichierSortie(nfs, "[Erreur] Echec d'allocation du stack dans allocStack();\n");
		break;
	}
}

void fichierSortie(char *nfs, char *message){
	FILE *fs = fopen(nfs, "w");

	if (fs){
		fprintf(fs,"%s", message);
		fclose(fs);
	}
}