#include "img.h"


inline int ImgSize (MOSIMG *img) {
	return img->img.height * img->img.width;
}


MOSIMG *NewImg (int new_height, int new_width) {
	// create new image
	MOSIMG *new_image;
	if ((new_image = (MOSIMG*) malloc (sizeof (MOSIMG))) == NULL)
		return NULL;
		
	// fill it's dimensions
	new_image->img.height = new_height;
	new_image->img.width = new_width;
	
	// alloc the dinamic stuff
	if ((new_image->img.mosaic = (char*) malloc (ImgSize (new_image) * sizeof (char))) == NULL)
		return NULL;

	if ((new_image->img.attr = (unsigned char*) calloc (ImgSize (new_image), sizeof (char))) == NULL)
		return NULL;

	// clear the mosaic with whitespaces
	int i;
	for (i = 0; i < ImgSize (new_image); i++)
		new_image->img.mosaic[i] = ' ';

	// create the curses window and panel
	new_image->win = newwin (new_height, new_width, 0, 0);
	new_image->pan = new_panel (new_image->win);
	update_panels ();
	doupdate ();

	return new_image;
}


int SaveImg (MOSIMG *image, const char *file_name) {
	FILE *f;
	if ((f = fopen (file_name, "w")) == NULL)
		return -1;

	fprintf (f, "%dx%d\n", image->img.height, image->img.width);
	
	int i;
	for (i = 0; i < image->img.height; i++) {
		fprintf (f, "%.*s\n", image->img.width, (image->img.mosaic + i * image->img.width));
	}
	
	fclose (f);

	return 0;
}


void FreeImg (MOSIMG *image) {
	free (image->img.attr);
	free (image->img.mosaic);
	del_panel (image->pan);
	delwin (image->win);
	free (image);
}
