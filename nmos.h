/** @file nmos.h
 * Main's function includes, prototypes and definitions
 */
 
#ifndef NMOS_H
#define NMOS_H

#include <curses.h>
#include <panel.h>
#include <stdio.h>
#include "color.h"
#include "img.h"
#include "wins.h"
#include "positioning.h"
#include "state.h"

#define KEY_ESC 27
#define KEY_CTRL_B 2
#define KEY_CTRL_C 3
#define KEY_CTRL_D 4
#define KEY_CTRL_O 15
#define KEY_CTRL_Q 17
#define KEY_CTRL_R 18
#define KEY_CTRL_S 19
#define KEY_CTRL_U 21
#define KEY_CTRL_V 22
#define KEY_CTRL_X 24

#define INITIAL_WIDTH 100
#define INITIAL_HEIGHT 30

#define BOUNDARY_Y (LINES)
#define BOUNDARY_X (COLS)


/// Ncurses initializations routines, including interactive mode and colors
void CursInit ();

/**
 * Asks for the user's choice for which direction he wants
 */
void DefaultDirection (WINDOW *hud, Direction *dir);


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
