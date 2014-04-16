/** @file img.h
 * .mos format definitions and operations
 */
 
#ifndef IMG_H
#define IMG_H

#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include <stdio.h>
#include "color.h"

/**
 * "Image" in Nmos mosaic format
 */
typedef struct {
	int height;	///< img height
	int	width;	///< img width
	char **mosaic;			/**< a height * width sized string: the drawing itself */
	unsigned char **attr;	/**< a height * width sized array with the attributes for each char.
							 * It's bits are masked, from most to less significative as the color 
							 * (6 bits), bold (1 bit on/off) and underline (1 bit on/off).
							 */
} Image;

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



/**
 * Inline function that measures the total size of an image
 * 
 * @param[in] img Image to be sized
 * 
 * @return image size: height * width
 */
inline int ImgSize (MOSIMG *img);

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
MOSIMG *NewImg (int new_height, int new_width);
/** aux for the NewImg: creates the border */
void dobox (WINDOW *win);
/**
 * Resize a @ref MOSIMG, reallocating the necessary memory
 * 
 * @param[in] new_height Image's new height
 * @param[in] new_width Image's new width
 * 
 * @return Resized @ref MOSIMG, clean and unlinked
 * @return __NULL__ if allocation failed
 */
int ResizeImg (MOSIMG *target, int new_height, int new_width);

/**
 * Only to say if you want to link the img before or after the other in LinkImg
 */
enum direction {before, after};
/**
 * Link an image to another, before or after it
 * 
 * @param[in] dest image linked to
 * @parem[in] src image to link to dest
 * @param[in] direction link src before or after dest
 * 
 * @return pointer to the linked MOSIMG, to be stored in the 'current'
 */
void LinkImg (MOSIMG *dest, MOSIMG *src, enum direction dir);
/**
 * Saves the image in a file
 * 
 * The file has a header with the mosaic dimensions, the asc art itself and it's attributes.
 * 
 * @note A .mosi file is hybrid.
 * The dimensions and @ref Image::mosaic are text, so they can be viewed in any text editor. 
 * The @ref Image::attr part is binary, so expect some weird stuff while oppening it as text.
 * 
 * @param[in] image The image to be saved
 * @param[in] file_name The new file name
 * 
 * @return 0 on success, -1 on failure
 */
int SaveImg (MOSIMG *image, const char *file_name);
/**
 * Loads the image from a file
 * 
 * It's the same scheme from the @ref SaveImg function
 * 
 * @param[in] image The image to be loaded onto
 * @param[in] file_name The new file name
 * 
 * @return 0 on success, -1 on failure
 */
int LoadImg (MOSIMG *image, const char *file_name);
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
int mos_addch (MOSIMG *image, int y, int x, int c);

/**
 * Displays current MOSIMG in the stdscr
 * 
 * @param[in] current MOSIMG
 */
void DisplayCurrentImg (MOSIMG *current);

/// Destroy an image, deallocating the used memory
void FreeImg (MOSIMG *img);

#endif
