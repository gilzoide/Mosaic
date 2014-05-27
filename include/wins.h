/** @file wins.h
 * Auxilary windows' definitions: help, menu, new/save/load
 */
 
#ifndef WINS_H
#define WINS_H

#define HELP_WIDTH COLS
#define HELP_HEIGHT (LINES - 1)

#define MENU_WIDTH 27
#define MENU_HEIGHT 30

#include <curses.h>
#include <panel.h>
#include <menu.h>
#include <string.h>
#include "positioning.h"

/// Draw the HUD, with a few shortcuts and the cursor position
WINDOW *CreateHud ();
/// Updates the position in the HUD
void UpdateHud (WINDOW *hud, Cursor cur, Direction dir);
/** 
 * Prints a message in the HUD and waits for keystroke
 * 
 * @param[in] hud the HUD
 * @param[in] message Message to be written in the HUD
 * 
 * @return the key pressed for leaving
 */
int PrintHud (WINDOW *hud, const char *message);
/// Draw the non-interactive help screen
void Help ();
/// Show the options and actions interactive menu
int Menu ();
/// Show the attributes table
int AttrTable (MOSIMG *current, Cursor cur);
/**
 * Creates a PANEL with a boxed WINDOW, with a title in the middle of it's top border.
 * 
 * @note Regarding the nlines and ncols parameters, don't forget that the box border takes one line/column.
 * @note To get the WINDOW inside, use the curses function `panel_window ()`.
 * @warning Always remember to free the memory allocated by the WINDOWs and PANELs. If you want, use our @ref DeletePanel function.
 * 
 * @param nlines 	Number of lines
 * @param ncols 	Number of columns
 * @param begin_y 	The top-left corner Y coordinate
 * @param begin_x 	The top-left corner X coordinate
 * @param title 	The title to be displayed
 * 
 * @return The PANEL created
 */
PANEL *CreateBoxedTitledWindow (int nlines, int ncols, int begin_y, int begin_x, const char *title);
/**
 * Deletes a previously created PANEL and the WINDOW inside, erasing it's contents
 */
void DeletePanel (PANEL *pan);


#endif
