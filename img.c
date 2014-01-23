#include "img.h"


inline int ImgSize (MOSIMG *img) {
	return img->height * img->width;
}


MOSIMG *NewImg (int new_height, int new_width) {
	MOSIMG *img;
	
	if ((img = (MOSIMG*) malloc (sizeof (MOSIMG))) == NULL)
		return NULL;
		
	img->height = new_height;
	img->width = new_width;
	
	if ((img->mosaic = (char*) calloc (ImgSize (img), sizeof (char))) == NULL)
		return NULL;
		
	if ((img->attr = (unsigned char*) calloc (ImgSize (img), sizeof (char))) == NULL)
		return NULL;
	
	return img;
}


void DestroyImg (MOSIMG *img) {
	free (img->attr);
	free (img->mosaic);
	free (img);
}
