/** @file mosaic.h
 * Main's function includes, prototypes and definitions
 */
 
#ifndef MOSAIC_H
#define MOSAIC_H

#include <curses.h>
#include <panel.h>
#include <stdio.h>

#include "color.h"
#include "mosimg.h"
#include "wins.h"
#include "positioning.h"
#include "state.h"
#include "keys.h"

#define INITIAL_HEIGHT 30
#define INITIAL_WIDTH 40

#define BOUNDARY_Y (LINES)
#define BOUNDARY_X (COLS)


/// Ncurses initializations routines, including interactive mode and colors
void CursInit ();

/**
 * Get a UTF-8 compatible int
 * 
 * We really want to use UTF-8, so we can display nice chars
 */
int toUTF8 ();



/**
 * Asks for the user's choice for which direction he wants
 */
void DefaultDirection (Direction *dir);

/**
 * The copy buffer
 * 
 * CopyBuffer is used at the Copy/Cut and Paste functions.
 * 
 * @warning CopyBuffers must be destroid before the program exits,
 * to avoid memory leaks!
 */
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
 * Reprint the Image from scratch, and refreshes it's WINDOW
 * 
 * @param current Img to be displayed
 */
void RefreshMOSIMG (MOSIMG *current);
/**
 * Destroy and free memory from the images list
 * 
 * @warning This function should be called before exiting the program,
 * unless you know what you're doing...
 */
void DestroyIMGS (IMGS *everyone);

#endif
