/** @file nmos.h
 * Main's function includes, prototypes and definitions
 */
 
#ifndef NMOS_H
#define NMOS_H

#include <ncurses.h>
#include <panel.h>
#include <stdio.h>
#include "color.h"
#include "img.h"

#define CTRL_Q 17
#define CTRL_INTERROGATION 127

#define HELP_WIDTH 54

/// UI current cursor position
typedef struct {
	int y;	///< vertical position
	int x;	///< horizontal position
} Cursor;


/// Images array
typedef struct {
	MOSIMG *imgs;	///< the images
	int size;	///< the array size
} IMGS;


/// index of the current image we're working on, starting from 0
extern int index;



/// Ncurses initializations routines, including interactive mode and colors
void CursInit ();

/// Draw the hud, with shortcuts, asks for things, that kinda stuff
WINDOW *CreateHud ();

/// Draw the non-interactive help screen
void Help ();

/// Initializes the IMGS
void InitIMGS (IMGS *everyone);

/// Create a new image and stores it in the main array
void CreateNewImg (IMGS *everyone);

#endif
