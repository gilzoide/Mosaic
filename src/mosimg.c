#include "mosimg.h"


void InitIMGS (IMGS *everyone) {
	everyone->list = NULL;
	everyone->size = 0;
}


MOSIMG *NewMOSIMG (int new_height, int new_width) {
	// create new MOSIMG
	MOSIMG *new_image;
	if ((new_image = (MOSIMG*) malloc (sizeof (MOSIMG))) == NULL)
		return NULL;
	
	NewImg (&new_image->img, new_height, new_width);

	// clear the mosaic with whitespaces
	int i, j;
	for (i = 0; i < new_height; i++)
		for (j = 0; j < new_width; j++)
			new_image->img.mosaic[i][j] = ' ';

	// create the curses window and panel
	new_image->win = newpad (new_height + 1, new_width + 1);
	dobox (new_image);	// put a border

	new_image->y = new_image->x = 0;
	scrollok (new_image->win, TRUE);
	
	new_image->pan = new_panel (new_image->win);
	DisplayCurrentImg (new_image);

	return new_image;
}


void dobox (MOSIMG *img) {
	int i;
	int y = img->img.height;
	int x = img->img.width;
	for (i = 0; i < x; i++) {
		mvwaddch (img->win, y, i, ACS_HLINE);
	}
	for (i = 0; i < y; i++) {
		mvwaddch (img->win, i, x, ACS_VLINE);
	}
	mvwaddch (img->win, y, x, ACS_LRCORNER);
}


int ResizeMOSIMG (MOSIMG *target, int new_height, int new_width) {
	delwin (target->win);
	target->win = newpad (new_height + 1, new_width + 1);
	int i = ResizeImg (&target->img, new_height, new_width);
	
	if (i == -1) {
		fprintf (stderr, "Resize failed");
		exit (-1);
	}
	
	dobox (target);

	return i;
}


void LinkImg (MOSIMG *dest, MOSIMG *src, enum direction dir) {
	if (dest != NULL) {
		MOSIMG *aux;
		if (dir == before) {
			aux = dest->prev;
			dest->prev = aux->next = src;
			src->next = dest;
			src->prev = aux;			
		}
		else {		// after
			aux = dest->next;
			dest->next = aux->prev = src;
			src->prev = dest;
			src->next = aux;
		}
	}
	else
		fprintf (stderr, "Error: trying to link a MOSIMG to a NULL pointer!!");
}


int mosAddch (MOSIMG *image, int y, int x, int c) {
	if (y >= image->img.height || x >= image->img.width)
		return ERR;

	mvwaddch (image->win, y, x, c);
	image->img.mosaic[y][x] = c;
	return 0;
}


void DisplayCurrentImg (MOSIMG *current) {
	top_panel (current->pan);
	update_panels ();
	doupdate ();
	prefresh (current->win, current->y, current->x, 0, 0, LINES - 2, COLS - 1);
}


void FreeMOSIMG (MOSIMG *image) {
	FreeImg (&image->img);
	del_panel (image->pan);
	delwin (image->win);
}
