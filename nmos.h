/** @file nmos.h
 * 'Main's function prototypes and definitions
 */

#include <ncurses.h>
#include <panel.h>
#include <stdio.h>

#define CTRL_Q 17
#define CTRL_H 8

#define HELP_WIDTH 54


/// Ncurses initializations routines, including interactive mode and colors
void CursInit ();

/// Draw the hud, with shortcuts, asks for things, that kinda stuff
WINDOW *CreateHud ();

/// Draw the non-interactive help screen
void Help ();

