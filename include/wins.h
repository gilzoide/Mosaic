/** @file wins.h
 * Auxilary windows' definitions: help, menu, new/save/load, about, attr_table
 */
 
#ifndef WINS_H
#define WINS_H

/* Windows' sizes or important points */
#define HELP_WIDTH COLS
#define HELP_HEIGHT (LINES - 1)

#define MENU_HEIGHT 9
#define MENU_WIDTH 21
#define MENU_X_SEPARATOR 9
#define CHKBX_Y 5
#define CHKBX_X 14

#define ABOUT_HEIGHT 20
#define ABOUT_WIDTH 50

#define HUD_MSG_X 28
#define HUD_CURSOR_X 15

#define MODES 3

#define SAVELOAD_WIDTH 18

#include <curses.h>
#include <panel.h>
#include <menu.h>
#include <form.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "positioning.h"
#include "keys.h"

#define INITIAL_HEIGHT 30
#define INITIAL_WIDTH 40

/**
 * Initialize the HUD
 */
void InitHud ();
/**
 * Redraw the HUD
 */
void ReHud ();

/// Updates the position in the HUD
void UpdateHud (Cursor cur, Direction dir);

/// For PrintHud execute a `scanw` instead of a getch
#define SCAN 2
/** 
 * Prints a message in the HUD and waits for keystroke
 * 
 * @param[in] wait_for_input Do we have to wait for input?
 * Possible values:
 * - FALSE: don't wait (and return 0)
 * - TRUE: wait for a char from getch
 * - SCAN: wait for a number from scanw
 * @param[in] message Message to be written in the HUD
 * 
 * @return the key pressed for leaving
 */
int PrintHud (char wait_for_input, const char *message);
/**
 * A @ref PrintHud with variable argument, just like printf
 *
 * @param[in] wait_for_input Same as @ref PrintHud
 * @param[in] message Formatted message to be written in the HUD
 * @param[in] ... Variable arguments to make the string to be
 *  passed to @ref PrintHud
 */
int VPrintHud (char wait_for_input, const char *message, ...);

/// Draw the non-interactive help screen
void Help ();
/// Show the options and actions interactive menu
int Menu ();

/// Show the About window
void About ();
/// Show the attributes table
mos_attr AttrTable (CURS_MOS *current, Cursor cur);

/**
 * Asks the user where to link the new image, and it's size
 *
 * @param[out] new_height The height that the user chose
 * @param[out] new_width The width that the user chose
 * @param[out] duplicate Whether we duplicate the current MOSAIC or not
 * @param[out] new_dir The linking direction that the user chose
 *
 * @return ERR if user don't want to create the image; OK otherwise
 */
char AskCreateNewMOSAIC (int *new_height, int *new_width, char *duplicate, enum direction *new_dir);
/**
 * Asks the user for the new image sizes, for resizing
 *
 * @param[out] new_height The new height
 * @param[out] new_width The new width
 *
 * @return ERR if user don't want to resize the image; OK otherwise
 */
char AskResizeMOSAIC (int *new_height, int *new_width);

/// Input/output, save/load
enum io {
	save,
	load
};
/**
 * Asks the user for the file name for loading an MOSAIC
 *
 * @return File name
 */
char *AskSaveLoadMOSAIC (enum io io);
/**
 * Inits the Save/Load form, with the possibility to specify it's starting value
 *
 * @param[in] file_name The initial file name. If NULL, nothing goes to the buffer
 */
void InitSaveLoadMOSAIC (const char *file_name);

/**
 * Ask if the user wants to quit
 *
 * @return Choice: 1 for YES, 0 for NO
 */
char AskQuit ();

/**
 * Ask something and wait for input: [Y]es or [N]o.
 * @note The default choice is always No.
 *
 * @param[in] msg The message to be shown, along with asking for the 
 * choice ("[y/N]")
 *
 * @return Choice: 1 for YES, 0 for NO
 */
char AskMessage (const char *msg);

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
 * @note The pointer is changed to NULL after the operation
 */
void DeletePanel (PANEL **pan);
/**
 * Deletes a previously created WINDOW, erasing it's contents and freeing memory.
 *
 * @note If the pointer is NULL, this function doesn't do a thing, so it's safer.
 * @note The pointer is changed to NULL after the operation
 */
void DeleteWindow (WINDOW **win);
/**
 * Deletes a previously created MENU and it's WINDOWs, freeing memory
 *
 * @note If the pointer is NULL, this function doesn't do a thing, so it's safer.
 * @note The pointer is changed to NULL after the operation
 */
void DeleteMenu (MENU **menu);
/**
 * Deletes a previously created FORM and it's WINDOWs, freeing memory
 *
 * @note If the pointer is NULL, this function doesn't do a thing, so it's safer.
 * @note The pointer is changed to NULL after the operation
 */
void DeleteForm (FORM **form);
/**
 * Deletes our important windows (menu, help, hud...), freeing the memory associated
 *
 * @warning This function calls `endwin`, from curses, so this 
 * should be the last command in the curses mode!
 */
void DestroyWins ();


#endif
