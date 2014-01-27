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

#define KEY_ESC 27
#define KEY_CTRL_Q 17

#define HELP_WIDTH 54

/// UI current cursor position
typedef struct {
	int y;	///< vertical position
	int x;	///< horizontal position
} Cursor;





/// Whole images list and it's size
typedef struct {
	MOSIMG *list;	///< the first image
	int size;	///< the array size
} IMGS;



/// Ncurses initializations routines, including interactive mode and colors
void CursInit ();
/// Draw the hud, with shortcuts, asks for things, that kinda stuff
WINDOW *CreateHud ();
/// Draw the non-interactive help screen
void Help ();
/// Show the options and actions interactive menu
int Menu ();

/// Initializes the IMGS
void InitIMGS (IMGS *everyone);
/// Create a new image and stores it in the images list
void CreateNewImg (IMGS *everyone);
/// Destroy and free memory from the images list
void DestroyIMGS (IMGS *everyone);

#endif
