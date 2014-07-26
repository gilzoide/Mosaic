/** @file img.h
 * .mosi format definitions and operations
 */
 
#ifndef IMG_H
#define IMG_H

#include <stdlib.h>
#include <stdio.h>
#include "color.h"

/**
 * "Image" in Nmos mosaic format
 */
typedef struct {
	int height;	///< img height
	int	width;	///< img width
	unsigned char **mosaic;			/**< a height * width sized string: the drawing itself */
	Attr **attr;	/**< a height * width sized array with the attributes for each char. */
} Image;



/**
 * Inline function that measures the total size of an image
 * 
 * @param[in] img Image to be sized
 * 
 * @return image size: height * width
 */
inline int ImgSize (Image img);


/** 
 * Create a new @ref Image, allocating the necessary memory
 * 
 * @param[in] img The Image to be initialized
 * @param[in] new_height New Image's height
 * @param[in] new_width New Image's width
 * 
 * @return 1 on success
 * @return -1 if allocation failed
 */
int NewImg (Image *img, int new_height, int new_width);
/**
 * Resize a @ref Image, reallocating the necessary memory
 * 
 * @param[in] img The target Image
 * @param[in] new_height Image's new height
 * @param[in] new_width Image's new width
 * 
 * @return 0 if successfully resized @ref MOSIMG
 * @return -1 if allocation failed
 */
int ResizeImg (Image *img, int new_height, int new_width);

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
int SaveImg (Image *image, const char *file_name);
/**
 * Loads the image from a file
 * 
 * It's the same scheme from the @ref SaveImg function
 * 
 * @param[out] image The image to be loaded onto
 * @param[in] file_name The new file name
 * 
 * @return 0 on success, -1 on failure, 1 on no dimensions present in the file
 */
int LoadImg (Image *image, const char *file_name);

/// Destroy an image, deallocating the used memory
void FreeImg (Image *img);

#endif
