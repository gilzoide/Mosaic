/** @file cat-nmos.c
 * A 'cat' application for nmos images
 */

#include <stdio.h>
#include <string.h>
#include "img.h"
#include "state.h"
#include "color.h"

/**
 * Prints the image at stdout, using 'puts's
 *
 * @param[in] img The image to be displayed
 */
void printImg (MOSIMG *img) {
	int i, j;
	for (i = 0; i < img->img.height; i++) {
		for (j = 0; j < img->img.width; j++) {
			Tcolor (img->img.attr[i][j]);
			printf ("%c", img->img.mosaic[i][j]);
		}

		putchar ('\n');
	}
}


int main (int argc, char *argv[]) {
	// we need the input file's name
	if (argc < 2) {
		fprintf (stderr, "Usage: cat-nmos [options -dc] <filename>\n");
		return 0;
	}
	
	char *arg_to_open;
	
	// options!
	if (argv[1][0] == '-') {
		arg_to_open = argv[2];
		if (strchr (argv[1], 'c'))
			ENTER_(COLORED);
		if (strchr (argv[1], 'd'))
			ENTER_(DIMMENSIONS);
	}
	else
		arg_to_open = argv[1];

	// image to be loaded
	MOSIMG img;
	NewImg (&img.img, 1, 1);
	switch (LoadImg (&img, arg_to_open)) {
		case -1:
			fprintf (stderr, "Couldn't open the file, i'm sorry...\n");
			return 0;

		case 1:
			fprintf (stderr, "There are no dimensions in this file...\n");
			return 0;
	}
	
	if (IS_(DIMMENSIONS))
		printf ("%dx%d\n", img.img.height, img.img.width);

	// print the image at stdout
	printImg (&img);

	FreeImg (&img.img);

	return 0;
}
