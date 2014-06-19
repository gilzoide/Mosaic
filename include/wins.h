/** @file wins.h
 * Auxilary windows' definitions: help, menu, new/save/load
 */
 
#ifndef WINS_H
#define WINS_H

#define HELP_WIDTH COLS
#define HELP_HEIGHT (LINES - 1)

#define MENU_HEIGHT 7
#define MENU_WIDTH 19
#define MENU_X_SEPARATOR 9

#include <curses.h>
#include <panel.h>
#include <menu.h>
#include <string.h>

#include "positioning.h"
#include "keys.h"

/**
 * Initialize all the important windows (and menus, panels, forms...)
 * 
 * As these are used frequently, we create them only once, and then just
 * show on the fly
 */
void InitWins ();

/// Updates the position in the HUD
void UpdateHud (Cursor cur, Direction dir);
/** 
 * Prints a message in the HUD and waits for keystroke
 * 
 * @param[in] hud the HUD
 * @param[in] message Message to be written in the HUD
 * 
 * @return the key pressed for leaving
 */
int PrintHud (const char *message);

/// Draw the non-interactive help screen
void Help ();
/// Show the options and actions interactive menu
int Menu ();
/// Auxilary function for the Menu: menu driver
int GetChosenOption (MENU *menu);

/// Show the attributes table
int AttrTable (MOSIMG *current, Cursor cur);
/**
 * Creates a PANEL with a boxed WINDOW, with a title in the middle of it's top border.
 * 
 * @note Regarding the nlines and ncols parameters, don't forget that the box border takes one line/column.
 * @warning Always remember to free the memory allocated by the curses WINDOWs. If you want, use our @ref DeleteWindow function.
 * 
 * @param nlines 	Number of lines
 * @param ncols 	Number of columns
 * @param begin_y 	The top-left corner Y coordinate
 * @param begin_x 	The top-left corner X coordinate
 * @param title 	The title to be displayed
 * 
 * @return The WINDOW created
 */
WINDOW *CreateBoxedTitledWindow (int nlines, int ncols, int begin_y, int begin_x, const char *title);
/**
 * Deletes a previously created PANEL and the WINDOW inside, erasing it's contents and freeing memory
 *
 * @note If the pointer is NULL, this function doesn't do a thing, so it's safer.
 */
void DeletePanel (PANEL *pan);
/**
 * Deletes a previously created WINDOW, erasing it's contents and freeing memory.
 *
 * @note If the pointer is NULL, this function doesn't do a thing, so it's safer.
 */
void DeleteWindow (WINDOW *win);
/**
 * Deletes our important windows (menu, help, hud), freeing the memory associated
 */
void DestroyWins ();


#endif
