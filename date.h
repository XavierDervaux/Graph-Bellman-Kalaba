#ifndef _DATE
#define _DATE

struct date{
	float *V;
	int n;
};

struct date* allocDate(int taille);
void libererDate (struct date**);

int dateTot(float a, float b);
int dateTard(float a, float b);

#endif