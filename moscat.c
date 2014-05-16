/** @file cat-nmos.c
 * A 'cat' application for nmos images
 */



#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

//~ #define RESET   "\033[0m"
//~ #define BLACK   "\033[30m"      /* Black */
//~ #define RED     "\033[31m"      /* Red */
//~ #define GREEN   "\033[32m"      /* Green */
//~ #define YELLOW  "\033[33m"      /* Yellow */
//~ #define BLUE    "\033[34m"      /* Blue */
//~ #define MAGENTA "\033[35m"      /* Magenta */
//~ #define CYAN    "\033[36m"      /* Cyan */
//~ #define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#include <stdio.h>
#include <string.h>
#include "img.h"
#include "state.h"

/**
 * Prints the image at stdout, using 'puts's
 *
 * @param[in] img The image to be displayed
 */
void printImg (MOSIMG *img) {
	int i, j;
	for (i = 0; i < img->img.height; i++) {
		for (j = 0; j < img->img.width; j++)
			putc (img->img.mosaic[i][j], stdout);

		putc ('\n', stdout);
	}
}


int main (int argc, char *argv[]) {
	// we need the input file's name
	if (argc < 2) {
		fprintf (stderr, "Usage: cat-nmos [options] <filename>\n");
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
