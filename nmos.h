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
#define KEY_CTRL_B 2
#define KEY_CTRL_C 3
#define KEY_CTRL_D 4
#define KEY_CTRL_O 15
#define KEY_CTRL_Q 17
#define KEY_CTRL_R 18
#define KEY_CTRL_S 19
#define KEY_CTRL_V 22
#define KEY_CTRL_X 24

#define HELP_WIDTH COLS
#define HELP_HEIGHT (LINES - 1)
#define INITIAL_WIDTH 40
#define INITIAL_HEIGHT 30

/**
 * Nmos State
 * 
 * It's used to store states along Nmos run. They are boolean values, witch may be on or off.
 * Use the defined macros to handle it, as they are here to simplify the process and make the code more readable.
 * 
 * @sa IS_(a)
 * @sa ENTER_(a)
 * @sa UN_(a)
 * @sa TOGGLE_(a)
 */
typedef char State;
State state;	///< the global Nmos State

// Some 'boolean' states, might be in or not
#define TOUCHED 	0b00000001	///< Was something changed? To ask if you really want to exit =P
#define SELECTION 	0b00000010	///< Box/Selection mode! Makes a difference when navigating and editing (copy, paste, attributes)
#define ERASED		0b00000100	///< A char was erased by Backspace or Delete buttons
// states operations
#define IS_(a)		(state & a)		///< Used to check if it's in the 'a' state or not
#define ENTER_(a)	(state |= a)	///< Used to enter the 'a' state
#define UN_(a)		(state &= ~a)	///< Used to exit the 'a' state
#define TOGGLE_(a)	(state ^= a)	///< Used to toggle the 'a' state


/// UI current cursor position
typedef struct {
	int y;	///< main y coordinate; upper-left corner
	int x;	///< main x coordinate; upper-left corner
	int origin_y;	///< origin of where box/selection started (y coordinate)
	int origin_x;	///< origin of where box/selection started (x coordinate)
} Cursor;



/**
 * The movement directions
 * 
 * @warning Keep the declaration order, for @ref REVERSE(dir) to work
 */
typedef enum {
	UP,		///< move up!
	LEFT,	///< move left!
	RIGHT,	///< move right, not wrong!
	DOWN	///< move down!
} Direction;
/**
 * Reverse a @ref Direction
 * 
 * It relies on the Direction declaration order, so that
 * >	3 - UP == DOWN
 * 
 * >	3 - LEFT == RIGHT 
 * and vice-versa
 */
#define REVERSE(dir) (3 - dir)


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
 * Move the cursor one position
 * 
 * @param[in] position Actual working position
 * @param[in] current Current image, for knowing the boundaries
 * @param[in] dir Direction of the movement
 */
void Move (Cursor *position, MOSIMG *current, Direction dir);
/**
 * Move the cursor until the boundary
 * 
 * @param[in] position Actual working position
 * @param[in] current Current image, for knowing the boundaries
 * @param[in] dir Direction of the movement
 */
void MoveAll (Cursor *position, MOSIMG *current, Direction dir);
/**
 * Changes the default direction for the movement
 * 
 * @param[in] hud we need the hud for the message
 * @param[inout] dir The current direction; might be changed
 */
void ChangeDefaultDirection (WINDOW *hud, Direction *dir);
/**
 * 
 */
void PrintSelection (Cursor *position, MOSIMG *current, Direction dir);
/**
 * 
 */
void UnprintSelection (MOSIMG *current);


typedef struct {
	WINDOW *buff;		///< WINDOW that stores the mosaic
	Cursor coordinates;	///< copy coordinates: origin_y/x for the upper-left corner, y/x for the size (how many chars in each direction)
} CopyBuffer;
/**
 * Initializes the copy buffer with default empty falues
 */
void InitCopyBuffer (CopyBuffer *buffer);
/**
 * Destroy the copy buffer, deallocating memory and erasing the buffered copy
 * 
 * @warning This function should be called before exiting the program,
 * unless you know what you're doing...
 */
inline void DestroyCopyBuffer (CopyBuffer *buffer);
/**
 * Copies the current selection (may be only one char, whatever) into the buffer
 * 
 * @param[in] current The current mosaic, to be copied from
 * @param[in] selection The selection to be copied
 * @param[out] buffer The copy buffer, to be stored the copied data
 */
void Copy (CopyBuffer *buffer, MOSIMG *current, Cursor selection);
/**
 * Copies the current selection (may be only one char, whatever) into the buffer and erase the selection
 * 
 * @param[in] current The current mosaic, to be copied from
 * @param[in] selection The selection to be copied
 * @param[out] buffer The copy buffer, to be stored the copied data
 */
void Cut (CopyBuffer *buffer, MOSIMG *current, Cursor selection);
/**
 * Copies the current selection (may be only one char, whatever) into the buffer
 * 
 * @param[in] current The current mosaic, to be pasted to
 * @param[in] cursor The position to paste from
 * @param[out] buffer The copy buffer, where the data is stored
 */
int Paste (CopyBuffer *buffer, MOSIMG *current, Cursor cursor);

/**
 * Create a new image and store it in the images list
 * 
 * @param[in] everyone the mosaics list
 * @param[in] current the current mosaic, a reference for the new one coming
 * 
 * @return pointer to the created MOSIMG, to be stored in the 'current'
 */
MOSIMG *CreateNewImg (IMGS *everyone, MOSIMG *current);
/**
 * Destroy and free memory from the images list
 * 
 * @warning This function should be called before exiting the program,
 * unless you know what you're doing...
 */
void DestroyIMGS (IMGS *everyone);

#endif
