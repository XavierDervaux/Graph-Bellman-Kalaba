#ifndef _BITMAP
#define _BITMAP
	struct bitmap{
		int *V;
		int n; 
	};
	
	struct bitmap* allocBitmap(int taille);
	void libererBitmap(struct bitmap** bm);
#endif