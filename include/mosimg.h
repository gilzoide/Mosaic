/** @file mosimg.h
 * @ref img wrapper for curses
 */

#ifndef MOSIMG_H
#define MOSIMG_H

#include <curses.h>
#include <panel.h>
#include "img.h"

/**
 * Wraps the image, list and curses interface
 * 
 * The Image structured as a double linked list, for sequencial matters ["video",
 * as we like to call it] and curses interface.
 */
typedef struct mosimg_t {
	Image img;	///< the asc art itself
	int y,		///< the Y coordinate of the piece of @ref img showed at the pad
		x;		///< the Y coordinate of the piece of @ref img showed at the pad
	struct mosimg_t *prev,	///< previous MOSIMG
					*next;	///< next MOSIMG
	WINDOW *win;	///< window to show the Image
	PANEL *pan;	///< panel for showing the window
} MOSIMG;


/// Whole images circular double linked list and it's size
typedef struct {
	MOSIMG *list;	///< the first image
	int size;	///< the array size
} IMGS;


/// Initializes the IMGS
void InitIMGS (IMGS *everyone);

/** 
 * Create a new @ref MOSIMG, allocating the necessary memory
 * 
 * @param[in] new_height New Image's height
 * @param[in] new_width New Image's width
 * 
 * @return New @ref MOSIMG, clean and unlinked
 * @return __NULL__ if allocation failed
 */
MOSIMG *NewMOSIMG (int new_height, int new_width);
/** aux for the NewMOSIMG: creates the right and bottom border */
void dobox (MOSIMG *img);

/**
 * Resize a @ref MOSIMG, reallocating the necessary memory and resizing it's WINDOW
 * @note This function just resizes the WINDOW, without caring about erasing or printing it on the screen
 * 
 * @param[in] target the target Image
 * @param[in] new_height Image's new height
 * @param[in] new_width Image's new width
 * 
 * @return 0 if successfully resized @ref MOSIMG
 * @return -1 if allocation failed
 */
int ResizeMOSIMG (MOSIMG *target, int new_height, int new_width);

/**
 * Only to say if you want to link the img before or after the other in LinkImg
 */
enum direction {before, after};
/**
 * Link an image to another, before or after it
 * 
 * @param[in] dest image linked to
 * @param[in] src image to link to dest
 * @param[in] dir link src before or after dest
 * 
 * @return pointer to the linked MOSIMG, to be stored in the 'current'
 */
void LinkImg (MOSIMG *dest, MOSIMG *src, enum direction dir);

/**
 * Add a char to the MOSIMG at position y/x and print it in it's WINDOW.
 * 
 * @note It doesn't refresh the WINDOW, so do it yourself whenever you feel ready
 * 
 * If position is outside image boundaries, returns ERR, as it calls mvwaddch, which calls wmove internally
 * 
 * @return 0 on success
 * @return ERR on error
 */
int mosAddch (MOSIMG *image, int y, int x, int c);

/**
 * Displays current MOSIMG in the stdscr
 * 
 * @param[in] current MOSIMG
 */
void DisplayCurrentImg (MOSIMG *current);

/// Destroy a MOSMIG, deallocating the used memory
void FreeMOSIMG (MOSIMG *image);

#endif
