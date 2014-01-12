/** @file img.h
 * .mos format definitions and operations
 */

/**
 * "Image" in Nmos mosaic format
 *
 *
 */
typedef struct {
	int height;				//!< img height
	int	width;				//!< img width
	int size;				/*!< total size = @ref height * @ref width */
	char *mosaic;			/*!< a @ref size sized _string_: the drawing itself */
	unsigned char *attr;	/*!< a @ref size sized array with the attributes for each char.
							 * It's bits are masked, from most to less significative as the color 
							 * (6 bits), bold (1 bit on/off) and underline (1 bit on/off).
							 */
} MOSIMG;



