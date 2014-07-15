#include "img.h"


inline int ImgSize (Image img) {
	return img.height * img.width;
}


int NewImg (Image *img, int new_height, int new_width) {
	// fill it's dimensions
	img->height = new_height;
	img->width = new_width;
	
	// alloc the dinamic stuff
	// mosaic:
	if ((img->mosaic = (unsigned char**) malloc (new_height * sizeof (unsigned char*))) == NULL)
		return -1;
	// attributes:
	if ((img->attr = (Attr**) malloc (new_height * sizeof (Attr*))) == NULL)
		return -1;

	int i;
	for (i = 0; i < new_height; i++) {
		if ((img->mosaic[i] = (unsigned char*) malloc (new_width * sizeof (unsigned char))) == NULL)
			return -1;
		if ((img->attr[i] = (Attr*) malloc (new_width * sizeof (Attr))) == NULL)
			return -1;
	}
	
	return 0;
}


int ResizeImg (Image *img, int new_height, int new_width) {
	// old dimensions
	int old_height = img->height;
	int old_width = img->width;
	// new dimensions
	img->height = new_height;
	img->width = new_width;
	
	int i;
	// if it's smaller, free the lines we're discarding
	for (i = old_height - 1; i >= new_height; i--) {
		free (img->mosaic[i]);
		free (img->attr[i]);
	}
	
	
	// realloc the dinamic stuff
	// Lines
	// mosaic:
	if ((img->mosaic = (unsigned char**) realloc (img->mosaic, new_height * sizeof (unsigned char*))) == NULL)
		return -1;
	for (i = old_height; i < new_height; i++)
		img->mosaic[i] = NULL;
	// attributes:
	if ((img->attr = (Attr**) realloc (img->attr, new_height * sizeof (Attr*))) == NULL)
		return -1;
	for (i = old_height; i < new_height; i++)
		img->attr[i] = NULL;

	// Columns
	for (i = 0; i < new_height; i++) {
		if ((img->mosaic[i] = (unsigned char*) realloc (img->mosaic[i], new_width * sizeof (unsigned char))) == NULL)
			return -1;
		if ((img->attr[i] = (Attr*) realloc (img->attr[i], new_width * sizeof (Attr))) == NULL)
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


int SaveImg (Image *image, const char *file_name) {
	FILE *f;
	if ((f = fopen (file_name, "w")) == NULL)
		return -1;

	fprintf (f, "%dx%d\n", image->height, image->width);
	
	int i;
	for (i = 0; i < image->height; i++) {
		fprintf (f, "%.*s\n", image->width, (image->mosaic[i]));
	}
	
	fclose (f);

	return 0;
}


int LoadImg (Image *image, const char *file_name) {
	FILE *f;
	if ((f = fopen (file_name, "r")) == NULL)
		return -1;
	
	int new_height, new_width;
	if (!fscanf (f, "%dx%d", &new_height, &new_width))
		return 1;
	
	ResizeImg (image, new_height, new_width);

	char c;
	// there's supposed to have a '\n' to discard after %dx%d; but if there ain't one, we read what's after
	if ((c = fgetc (f)) != '\n')
		ungetc (c, f);
	
	int i, j;
	for (i = 0; i < image->height; i++) {
		// read the line until the end or no more width is available
		for (j = 0; j < image->width; j++) {
			if ((c = fgetc (f)) == EOF)
				goto FILL_WITH_BLANK;	// used so won't need a flag or more comparisons to break both the fors
			// if it reached newline before width...
			else if (c == '\n')
				break;
			image->mosaic[i][j] = c;
		}
		// ...complete with whitespaces
		for ( ;  j < image->width; j++) {
			image->mosaic[i][j] = ' ';
		}
		
		// we read the whole line, but the tailing '\n', we need to discard it
		if (c != '\n')
			// may happen it's not a newline yet, so let's reread it =P
			if ((c = fgetc (f)) != '\n')
				ungetc (c, f);
	}
	
FILL_WITH_BLANK:
	// well, maybe we reached OEF, so everything else is a blank...
	for ( ; i < image->height; i++) {
		for (j = 0;  j < image->width; j++) {
			image->mosaic[i][j] = ' ';
		}
	}
	
	fclose (f);

	return 0;
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
