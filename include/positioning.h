/** @file positioning.h
 * Main's function includes, prototypes and definitions
 */
 
#ifndef POSITIONING_H
#define POSITIONING_H

#include "curs_mos.h"
#include "state.h"

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


/// Aux function: returns the greatest
inline int min (int a, int b);
/// Aux function: returns the smallest
inline int max (int a, int b);

/// initializes the cursor values
void InitCursor (Cursor *cursor);

/**
 * Prints in the current window the selection box
 */
void PrintSelection (Cursor *position, CURS_MOS *current);
/**
 * Unprints the selection box 
 */
void UnprintSelection (CURS_MOS *current);
/**
 * Move the cursor one position
 * 
 * @param[in] position Actual working position
 * @param[in] current Current image, for knowing the boundaries
 * @param[in] dir Direction of the movement
 */
void Move (Cursor *position, CURS_MOS *current, Direction dir);
/**
 * Move the cursor to the desired position (for clicking)
 *
 * @param[in] position Actual working position
 * @param[in] current Current image, for knowing the boundaries
 * @param[in] y Y coordinate
 * @param[in] x X coordinate
 */
void MoveTo (Cursor *position, CURS_MOS *current, int y, int x);
/**
 * Move the cursor until the boundary
 * 
 * @param[in] position Actual working position
 * @param[in] current Current image, for knowing the boundaries
 * @param[in] dir Direction of the movement
 */
void MoveAll (Cursor *position, CURS_MOS *current, Direction dir);
/**
 * Changes the default direction for the movement
 * 
 * @param[in] c the key pressed, which we use to know what the desired direction is
 * @param[inout] dir The current direction; might be changed
 */
void ChangeDefaultDirection (int c, Direction *dir);

#endif
