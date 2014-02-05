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
	// mosaic:
	if ((new_image->img.mosaic = (char**) malloc (new_height * sizeof (char*))) == NULL)
		return NULL;
	// attributes:
	if ((new_image->img.attr = (unsigned char**) malloc (new_height * sizeof (char*))) == NULL)
		return NULL;

	int i;
	for (i = 0; i < new_height; i++) {
		if ((new_image->img.mosaic[i] = (char*) malloc (new_width * sizeof (char))) == NULL)
			return NULL;
		if ((new_image->img.attr[i] = (unsigned char*) malloc (new_width * sizeof (char))) == NULL)
			return NULL;
	}

	// clear the mosaic with whitespaces
	int j;
	for (i = 0; i < new_height; i++)
		for (j = 0; j < new_width; j++)
			new_image->img.mosaic[i][j] = ' ';

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
		fprintf (f, "%.*s\n", image->img.width, (image->img.mosaic[i]));
	}
	
	fclose (f);

	return 0;
}


void FreeImg (MOSIMG *image) {
	int i;
	for (i = 0; i < image->img.height; i++) {
		free (image->img.attr[i]);
		free (image->img.mosaic[i]);
	}
	free (image->img.attr);
	free (image->img.mosaic);
	del_panel (image->pan);
	delwin (image->win);
	free (image);
}
