#include "img.h"


inline int ImgSize (MOSIMG *img) {
	return img->img.height * img->img.width;
}


void InitIMGS (IMGS *everyone) {
	everyone->list = NULL;
	everyone->size = 0;
}


int NewImg (Image *img, int new_height, int new_width) {
	// fill it's dimensions
	img->height = new_height;
	img->width = new_width;
	
	// alloc the dinamic stuff
	// mosaic:
	if ((img->mosaic = (char**) malloc (new_height * sizeof (char*))) == NULL)
		return -1;
	// attributes:
	if ((img->attr = (unsigned char**) malloc (new_height * sizeof (char*))) == NULL)
		return -1;

	int i;
	for (i = 0; i < new_height; i++) {
		if ((img->mosaic[i] = (char*) malloc (new_width * sizeof (char))) == NULL)
			return -1;
		if ((img->attr[i] = (unsigned char*) malloc (new_width * sizeof (char))) == NULL)
			return -1;
	}
	
	return 0;
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


int ResizeImg (Image *img, int new_height, int new_width) {
	// old dimensions
	int old_height = img->height;
	int old_width = img->width;
	// new dimensions
	img->height = new_height;
	img->width = new_width;
	
	int i;
	for (i = old_height; i > new_height; i--) {
		free (img->mosaic[i]);
		free (img->attr[i]);
	}
	
	
	// realloc the dinamic stuff
	// mosaic:
	if ((img->mosaic = (char**) realloc (img->mosaic, new_height * sizeof (char*))) == NULL)
		return -1;
	// attributes:
	if ((img->attr = (unsigned char**) realloc (img->attr, new_height * sizeof (char*))) == NULL)
		return -1;
		
	for (i = 0; i < new_height; i++) {
		if ((img->mosaic[i] = (char*) realloc (img->mosaic[i], new_width * sizeof (char))) == NULL)
			return -1;
		if ((img->attr[i] = (unsigned char*) realloc (img->attr[i], new_width * sizeof (char))) == NULL)
			return -1;
	}
	
	// maybe it grew, so complete with blanks
	int j;
	for (i = old_height; i < new_height; i++) {
		for (j = 0; j < new_width; j++)
			img->mosaic[i][j] = ' ';
	}
	
	for (i = old_width; i < new_width; i++) {
		for (j = 0; j < new_height; j++)
			img->mosaic[j][i] = ' ';
	}

	return 0;
}


int ResizeMOSIMG (MOSIMG *target, int new_height, int new_width) {
	delwin (target->win);
	target->win = newpad (new_height + 1, new_width + 1);
	int i = ResizeImg (&target->img, new_height, new_width);
	dobox (target);
	
	if (i == -1) {
		fprintf (stderr, "Resize failed");
		exit (-1);
	}
	
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
	
	ResizeMOSIMG (image, new_height, new_width);

	char c;
	// there's supposed to have a '\n' to discard after %dx%d; but if there ain't one, we read what's after
	if ((c = fgetc (f)) != '\n')
		ungetc (c, f);
	
	int i, j;
	for (i = 0; i < image->img.height; i++) {
		// read the line until the end or no more width is available
		for (j = 0; j < image->img.width; j++) {
			if ((c = fgetc (f)) == EOF)
				goto FILL_WITH_BLANK;	// yes, that's right, a f**king goto! blablabla good programming, blablabla; F**K OFF! (actually, used so won't need a flag or more comparisons to break both the fors)
			// if it reached newline before width...
			else if (c == '\n')
				break;
			image->img.mosaic[i][j] = c;
		}
		// ...complete with whitespaces
		for ( ;  j < image->img.width; j++) {
			image->img.mosaic[i][j] = ' ';
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
		for (j = 0;  j < image->img.width; j++) {
			image->img.mosaic[i][j] = ' ';
		}
	}
	
	fclose (f);

	return 0;
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


void FreeImg (Image *img) {
	int i;
	for (i = 0; i < img->height; i++) {
		free (img->attr[i]);
		free (img->mosaic[i]);
	}
	free (img->attr);
	free (img->mosaic);
}


void FreeMOSIMG (MOSIMG *image) {
	FreeImg (&image->img);
	del_panel (image->pan);
	delwin (image->win);
}
