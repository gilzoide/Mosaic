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
typedef char State;
State state;	///< the global Nmos State

/* Some 'boolean' states, might be in or not */
/** Was something changed? To ask if you really want to exit =P */
#define TOUCHED 			0b00000001
/** Box/Selection mode! Makes a difference when navigating
 * and editing (copy, paste, attributes)
 */
#define SELECTION 			0b00000010
/** 'Paste' is transparent? (ignores blanks) */
#define TRANSPARENT			0b00000100
/** Don't move cursor after input
 * A char might have been erased by Backspace or Delete buttons,
 * or selection mode insertion
 */ 
#define NO_MOVING_CURSOR	0b00001000
/** There's a message in the hud, but ain't waiting for input
 * @sa UpdateHud
 */
#define HUD_MESSAGE			0b00010000
/** Insert mode: insert a new char or write over (IS_(INSERT) -> new char) */
#define INSERT				0b00100000
/** Quit the sw (to break the main loop) */
#define QUIT				0b10000000

// states operations
#define IS_(a)		(state & a)		///< Used to check if it's in the 'a' state or not
#define ENTER_(a)	(state |= a)	///< Used to enter the 'a' state
#define UN_(a)		(state &= ~a)	///< Used to exit the 'a' state
#define TOGGLE_(a)	(state ^= a)	///< Used to toggle the 'a' state

#endif
