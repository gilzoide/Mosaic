/** @file wins.h
 * Auxilary windows' definitions: help, menu, new/save/load
 */
 
#ifndef WINS_H
#define WINS_H

#define HELP_WIDTH COLS
#define HELP_HEIGHT (LINES - 1)

#define MENU_HEIGHT 7
#define MENU_WIDTH 20
#define MENU_X_SEPARATOR 9
#define CHKBX_Y 5
#define CHKBX_X 17

#define SAVELOAD_WIDTH 18

#include <curses.h>
#include <panel.h>
#include <menu.h>
#include <form.h>
#include <string.h>
#include <ctype.h>

#include "positioning.h"
#include "keys.h"

/**
 * Initialize the HUD
 */
void InitHud ();

/// Updates the position in the HUD
void UpdateHud (Cursor cur, Direction dir);
/** 
 * Prints a message in the HUD and waits for keystroke
 * 
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

/// Show the About window
void About ();
/// Show the attributes table
int AttrTable (CURS_MOS *current, Cursor cur);

/**
 * Asks the user where to link the new image,
 * and it's size
 *
 * @param[out] new_height The height that the user chose
 * @param[out] new_width The width that the user chose
 * @param[out] duplicate Whether we duplicate the current MOSAIC or not
 * @param[out] new_dir The linking direction that the user chose
 *
 * @return ERR if user don't want to create the image; OK otherwise
 */
char AskCreateNewMOSAIC (int *new_height, int *new_width, char *duplicate, enum direction *new_dir);

/// Input/output, save/load
enum io {save, load};
/**
 * Asks the user for the file name for loading an MOSAIC
 *
 * @return File name
 */
char *AskSaveLoadMOSAIC (enum io io);

/**
 * Ask if the user wants to quit
 *
 * @return Choice: 1 for YES, 0 for NO
 */
char AskQuit ();


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
 * Like the previous, but the WINDOW is centered in the screen
 * @sa CreateBoxedTitledWindow
 *
 * @param nlines 	Number of lines
 * @param ncols 	Number of columns
 * @param title 	The title to be displayed
 *
 * @return The WINDOW created
 */
WINDOW *CreateCenteredBoxedTitledWindow (int nlines, int ncols, const char *title);
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
 * Deletes a previously created MENU and it's WINDOWs, freeing memory
 *
 * @note If the pointer is NULL, this function doesn't do a thing, so it's safer.
 */
void DeleteMenu (MENU *menu);
/**
 * Deletes a previously created FORM and it's WINDOWs, freeing memory
 *
 * @note If the pointer is NULL, this function doesn't do a thing, so it's safer.
 */
void DeleteForm (FORM *form);
/**
 * Deletes our important windows (menu, help, hud...), freeing the memory associated
 */
void DestroyWins ();


#endif
