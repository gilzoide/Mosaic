#include "mosaic.h"


inline int MOSAICSize (MOSAIC img) {
	return img.height * img.width;
}


int NewMOSAIC (MOSAIC *img, int new_height, int new_width) {
	// dimensions still 0
	img->height = img->width = 0;
	// NULL pointers, for realloc to work as a malloc
	img->mosaic = img->attr = NULL;
	
	// alloc the dinamic stuff and fill it: something ResizeMOSAIC already does
	ResizeMOSAIC (img, new_height, new_width);
	
	return 0;
}


int ResizeMOSAIC (MOSAIC *img, int new_height, int new_width) {
	// old dimensions
	int old_height = img->height;
	int old_width = img->width;
	// new dimensions
	img->height = new_height;
	img->width = new_width;
	
	int i;
	// when shrinking the , free the lines we're discarding
	for (i = old_height - 1; i >= new_height; i--) {
		free (img->mosaic[i]);
		free (img->attr[i]);
	}
	
	
	// realloc the dinamic stuff
	// Lines
	// mosaic:
	if ((img->mosaic = (unsigned char**) realloc (
		img->mosaic, new_height * sizeof (unsigned char*))) == NULL)
		return -1;
	for (i = old_height; i < new_height; i++)
		img->mosaic[i] = NULL;
	// attributes:
	if ((img->attr = (Attr**) realloc (
		img->attr, new_height * sizeof (Attr*))) == NULL)
		return -1;
	for (i = old_height; i < new_height; i++)
		img->attr[i] = NULL;

	// Columns
	for (i = 0; i < new_height; i++) {
		if ((img->mosaic[i] = (unsigned char*) realloc (
			img->mosaic[i], new_width * sizeof (unsigned char))) == NULL)
			return -1;
		if ((img->attr[i] = (Attr*) realloc (
			img->attr[i], new_width * sizeof (Attr))) == NULL)
			return -1;
	}
	
	// maybe it grew, so complete with blanks
	int j;
	if (old_height > 0 && old_width > 0) {
		// new lines, until old width
		for (i = old_height; i < new_height; i++) {
			for (j = 0; j < old_width; j++) {
				img->mosaic[i][j] = ' ';
				img->attr[i][j] = 0;
			}
		}
		// new columns, until old height
		for (i = old_width; i < new_width; i++) {
			for (j = 0; j < old_height; j++) {
				img->mosaic[j][i] = ' ';
				img->attr[j][i] = 0;
			}
		}
	}

	// the other square: from old to new height/width (for growing)
	for (i = old_height; i < new_height; i++) {
		for (j = old_width; j < new_width; j++) {
			img->mosaic[i][j] = ' ';
			img->attr[i][j] = 0;
		}
	}

	return 0;
}


int SaveMOSAIC (MOSAIC *image, const char *file_name) {
	FILE *f;
	if ((f = fopen (file_name, "w")) == NULL)
		return -1;

	fprintf (f, "%dx%d\n", image->height, image->width);
	
	// Mosaic
	int i;
	for (i = 0; i < image->height; i++) {
		fprintf (f, "%.*s\n", image->width, (image->mosaic[i]));
	}
	// Attr
	
	fclose (f);

	return 0;
}


int LoadMOSAIC (MOSAIC *image, const char *file_name) {
	FILE *f;
	if ((f = fopen (file_name, "r")) == NULL)
		return errno;
	
	int new_height, new_width;
	if (!fscanf (f, "%3dx%3d", &new_height, &new_width)) {
		fclose (f);
		return 1;
	}
	
	ResizeMOSAIC (image, new_height, new_width);

	int c;
	// there's supposed to have a '\n' to discard after %dx%d;
	// but if there ain't one, we read what's after
	if ((c = fgetc (f)) != '\n')
		ungetc (c, f);
	
	int i, j;
	for (i = 0; i < image->height; i++) {
		// read the line until the end or no more width is available
		for (j = 0; j < image->width; j++) {
			if ((c = fgetc (f)) == EOF)
				// used so won't need a flag or more comparisons
				// to break both the fors
				goto FILL_WITH_BLANK;
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


void FreeMOSAIC (MOSAIC *img) {
	int i;
	for (i = 0; i < img->height; i++) {
		free (img->attr[i]);
		free (img->mosaic[i]);
	}
	free (img->attr);
	free (img->mosaic);
}
