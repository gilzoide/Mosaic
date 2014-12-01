/** @file state.h
 * State definition
 */

#ifndef STATE_H
#define STATE_H

/**
 * Nmos State
 * 
 * It's used to store states along Nmos run. They are boolean values,
 * witch may be on or off. Use the defined macros to handle it, as they 
 * are here to simplify the process and make the code more readable.
 * 
 * @sa IS_(a)
 * @sa ENTER_(a)
 * @sa UN_(a)
 * @sa TOGGLE_(a)
 */
typedef short State;
State state;	///< the global Nmos State

/* Some 'boolean' states, might be in or not */
/** Was something changed? To ask if you really want to exit =P */
#define TOUCHED 			0x0001
/** Box/Selection mode! Makes a difference when navigating
 * and editing (copy, paste, attributes)
 */
#define SELECTION 			0x0002
/** 'Paste' is transparent? (ignores blanks) */
#define TRANSPARENT			0x0004
/** Don't move cursor after input
 * A char might have been erased by Backspace or Delete buttons,
 * or selection mode insertion
 */ 
#define NO_MOVING_CURSOR	0x0008
/** There's a message in the hud, but ain't waiting for input
 * @sa UpdateHud
 */
#define HUD_MESSAGE			0x0010
/** Insert mode: insert a new char or write over (IS_(INSERT) => new char) */
#define INSERT				0x0020
/** Paint mode: every move you make, paint is applied */
#define PAINT				0x0040
/** Needs to redraw the whole whing! */
#define REDRAW				0x0080
/** Quit the sw (to break the main loop) */
#define QUIT				0x8000

// states operations
#define IS_(a)		(state & a)		///< Used to check if it's in the 'a' state or not
#define ENTER_(a)	(state |= a)	///< Used to enter the 'a' state
#define UN_(a)		(state &= ~a)	///< Used to exit the 'a' state
#define TOGGLE_(a)	(state ^= a)	///< Used to toggle the 'a' state

#endif
