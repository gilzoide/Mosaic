/** @file img.h
 * .mos format definitions and operations
 */
 
#ifndef IMG_H
#define IMG_H

#include <stdlib.h>

/**
 * "Image" in Nmos mosaic format
 *
 * It's structured as a double linked list, for sequencial matters ["video", 
 * as we like to call it]
 */
typedef struct mosimg_t {
	int height;				///< img height
	int	width;				///< img width
	char *mosaic;			/**< a height * width sized _string_: the drawing itself */
	unsigned char *attr;	/**< a height * width sized array with the attributes for each char.
							 * It's bits are masked, from most to less significative as the color 
							 * (6 bits), bold (1 bit on/off) and underline (1 bit on/off).
							 */
	
	struct mosimg_t *prev;	///< previous image
	struct mosimg_t *next;	///< next image
} MOSIMG;


/**
 * Inline function that measures the total size of an image
 * 
 * @param[in] img Image to be sized
 * 
 * @return image size: height * width
 */
inline int ImgSize (MOSIMG *img);

/** Create a new @ref MOSIMG, allocating the necessary memory
 * 
 * @param new_height New Image height
 * 
 * @return New Image, clean and unlinked
 */
MOSIMG *NewImg (int new_height, int new_width);

/// Destroy an image, deallocating the used memory
void FreeImg (MOSIMG *img);

#endif
