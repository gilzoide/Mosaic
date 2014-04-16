/** @file state.h
 * State definition
 */

#ifndef STATE_H
#define STATE_H

/**
 * Nmos State
 * 
 * It's used to store states along Nmos run. They are boolean values, witch may be on or off.
 * Use the defined macros to handle it, as they are here to simplify the process and make the code more readable.
 * 
 * @sa IS_(a)
 * @sa ENTER_(a)
 * @sa UN_(a)
 * @sa TOGGLE_(a)
 */
typedef char State;
State state;	///< the global Nmos State

// Some 'boolean' states, might be in or not
#define TOUCHED 	0b00000001	///< Was something changed? To ask if you really want to exit =P
#define SELECTION 	0b00000010	///< Box/Selection mode! Makes a difference when navigating and editing (copy, paste, attributes)
#define ERASED		0b00000100	///< A char was erased by Backspace or Delete buttons
// states operations
#define IS_(a)		(state & a)		///< Used to check if it's in the 'a' state or not
#define ENTER_(a)	(state |= a)	///< Used to enter the 'a' state
#define UN_(a)		(state &= ~a)	///< Used to exit the 'a' state
#define TOGGLE_(a)	(state ^= a)	///< Used to toggle the 'a' state

#endif
