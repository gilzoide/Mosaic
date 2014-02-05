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
#define KEY_CTRL_O 15
#define KEY_CTRL_Q 17
#define KEY_CTRL_S 19
#define KEY_SUP 337
#define KEY_SDOWN 336

#define HELP_WIDTH COLS
#define HELP_HEIGHT (LINES - 1)

/// UI current cursor position
typedef struct {
	int y;	///< main y coordinate; upper-left corner
	int x;	///< main x coordinate; upper-left corner
	int bot_y;	///< bottom-right corner y coordinate: for copy/paste and attribute setting
	int bot_x;	///< bottom-right corner x coordinate: for copy/paste and attribute setting
} Cursor;



/// Whole images circular double linked list and it's size
typedef struct {
	MOSIMG *list;	///< the first image
	int size;	///< the array size
} IMGS;



/// The movement directions
typedef enum {
	UP,	///< move up!
	DOWN,	///< move down!
	LEFT,	///< move left!
	RIGHT	///< move right, not wrong!
} Direction;



/// Ncurses initializations routines, including interactive mode and colors
void CursInit ();
/// initializes the cursor values
void InitCursor (Cursor *cursor);
/// Draw the HUD, with a few shortcuts and the cursor position
WINDOW *CreateHud ();
/// Updates the position in the HUD
void UpdateHud (WINDOW *hud, Cursor cur, Direction dir);
/** 
 * Prints a message in the HUD and waits for keystroke
 * 
 * @param[in] message Message to be written in the HUD
 */
void PrintHud (WINDOW *hud, const char *message);
/// Draw the non-interactive help screen
void Help ();
/// Show the options and actions interactive menu
int Menu ();
/// Show the attributes table
int AttrTable (MOSIMG *current, Cursor cur);


/** 
 * Move the cursor
 * 
 * @param[in] position Actual working position
 * @param[in] current Current image, for knowing the boundaries
 * @param[in] dir Direction of the movement
 */
inline void Move (Cursor *position, MOSIMG *current, Direction dir);



/// Initializes the IMGS
void InitIMGS (IMGS *everyone);
/// Create a new image and stores it in the images list
void CreateNewImg (IMGS *everyone);
/// Destroy and free memory from the images list
void DestroyIMGS (IMGS *everyone);

#endif
