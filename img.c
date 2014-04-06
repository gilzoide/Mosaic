#include "img.h"


inline int ImgSize (MOSIMG *img) {
	return img->img.height * img->img.width;
}


void InitIMGS (IMGS *everyone) {
	everyone->list = NULL;
	everyone->size = 0;
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
	new_image->win = newpad (new_height + 2, new_width + 2);
	new_image->pan = new_panel (new_image->win);
	update_panels ();
	doupdate ();
	
	new_image->y = new_image->x = 0;
	
	box (new_image->win, 0, 0);
	prefresh (new_image->win, 1, 1, new_image->y, new_image->x, new_height, new_width);

	return new_image;
}


int ResizeImg (MOSIMG *target, int new_height, int new_width) {
	// new dimensions
	target->img.height = new_height;
	target->img.width = new_width;
	
	// realloc the dinamic stuff
	// mosaic:
	if ((target->img.mosaic = (char**) realloc (target->img.mosaic, new_height * sizeof (char*))) == NULL)
		return -1;
	// attributes:
	if ((target->img.attr = (unsigned char**) realloc (target->img.attr, new_height * sizeof (char*))) == NULL)
		return -1;
		
	int i;
	for (i = 0; i < new_height; i++) {
		if ((target->img.mosaic[i] = (char*) realloc (target->img.mosaic[i], new_width * sizeof (char))) == NULL)
			return -1;
		if ((target->img.attr[i] = (unsigned char*) realloc (target->img.attr[i], new_width * sizeof (char))) == NULL)
			return -1;
	}

	return 0;
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


int LoadImg (MOSIMG *image, const char *file_name) {
	FILE *f;
	if ((f = fopen (file_name, "r")) == NULL)
		return -1;
	
	int new_height, new_width;
	if (!fscanf (f, "%dx%d", &new_height, &new_width))
		return 1;
	
	ResizeImg (image, new_height, new_width);

	char c;
	// there's supposed to have a '\n' to discard after %dx%d, but if don't, we read what's after
	if ((c = fgetc (f)) != '\n')
		ungetc (c, f);
	
	int i, j;
	wmove (image->win, 0, 0);
	for (i = 0; i < image->img.height; i++) {
		// read the line until the end or no more width is available
		for (j = 0; j < image->img.width; j++) {
			if ((c = fgetc (f)) == EOF)
				goto FILL_WITH_BLANK;	// yes, that's right, a f**king goto! blablabla good programming, blablabla; F**K OFF! (actually, used so won't need a flag or more comparisons to break both the fors)
			// if it reached newline before width...
			else if (c == '\n')
				break;
			image->img.mosaic[i][j] = c;
			wechochar (image->win, c);
		}
		// ...complete with whitespaces
		for ( ;  j < image->img.width; j++) {
			image->img.mosaic[i][j] = ' ';
			wechochar (image->win, ' ');
		}
		
		// we read the whole line, but the tailing '\n', we need to discard it
		if (c != '\n')
			// may happen it's not a newline yet, so let's reread it =P
			if ((c = fgetc (f)) != '\n')
				ungetc (c, f);
	}
	
FILL_WITH_BLANK:
	// well, maybe we reached OEF, so everything else is a blank...
	for ( ; i < image->img.height; i++) {
		for ( ;  j < image->img.width; j++) {
			image->img.mosaic[i][j] = ' ';
			wechochar (image->win, ' ');
		}
		
		j = 0;
	}

	return 0;
}


int mos_addch (MOSIMG *image, int y, int x, int c) {
	if (mvwaddch (image->win, y + 1, x + 1, c) == ERR)
		return ERR;
	image->img.mosaic[y][x] = c;
	return 0;
}


void DisplayCurrentImg (MOSIMG *current) {
	top_panel (current->pan);
	update_panels ();
	doupdate ();
	prefresh (current->win, 1, 1, 0, 0, current->img.height, current->img.width);
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
