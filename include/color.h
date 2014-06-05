/** @file color.h
 * Color definitions and initialization
 */

#ifndef COLOR_H
#define COLOR_H

#include <curses.h>

/// Initialize all color combinations in Curses
void InitColors ();
/// A simple test of the color definitions, with Curses as output
void TestColors_Curses ();
/// A simple test of the color definitions, with stdout as output
void TestColors_Stdout ();

/**
 * Color definitions
 * 
 * The colors used in Mosaic (and in terminals, in overall), defined
 * in an enum. These are already the COLOR_PAIRs at curses.
 * 
 * @note Colors are defined in the format: _foreground-background_
 * @note They're agrouped by foreground color.
 */
enum colors {
	Normal = 0,	///< Terminal's default colors
// black
	BkBk, 	///< Black-Black
	BkR, 	///< Black-Red
	BkG, 	///< Black-Green
	BkY, 	///< Black-Yellow
	BkBl, 	///< Black-Blue
	BkM, 	///< Black-Magenta
	BkC, 	///< Black-Cyan
	BkW, 	///< Black-White
// red
	RBk, 	///< Red-Black
	RR, 	///< Red-Red
	RG, 	///< Red-Green
	RY, 	///< Red-Yellow
	RBl, 	///< Red-Blue
	RM, 	///< Red-Magenta
	RC, 	///< Red-Cyan
	RW, 	///< Red-White
// green
	GBk, 	///< Green-Black
	GR, 	///< Green-Red
	GG, 	///< Green-Green
	GY, 	///< Green-Yellow
	GBl, 	///< Green-Blue
	GM, 	///< Green-Magenta
	GC, 	///< Green-Cyan
	GW, 	///< Green-White
// yellow
	YBk, 	///< Yellow-Black
	YR, 	///< Yellow-Red
	YG, 	///< Yellow-Green
	YY, 	///< Yellow-Yellow
	YBl, 	///< Yellow-Blue
	YM, 	///< Yellow-Magenta
	YC, 	///< Yellow-Cyan
	YW, 	///< Yellow-White
// blue
	BlBk, 	///< Blue-Black
	BlR, 	///< Blue-Red
	BlG, 	///< Blue-Green
	BlY, 	///< Blue-Yellow
	BlBl, 	///< Blue-Blue
	BlM, 	///< Blue-Magenta
	BlC, 	///< Blue-Cyan
	BlW, 	///< Blue-White
// magenta
	MBk,	///< Magenta-Black
	MR, 	 ///< Magenta-Red
	MG,  	///< Magenta-Green
	MY,  	///< Magenta-Yellow
	MBl,	///< Magenta-Blue
	MM, 	 ///< Magenta-Magenta
	MC, 	///< Magenta-Cyan
	MW, 	///< Magenta-White
// cyan
	CBk, 	///< Cyan-Black
	CR, 	///< Cyan-Red
	CG, 	///< Cyan-Green
	CY, 	///< Cyan-Yellow
	CBl, 	///< Cyan-Blue
	CM, 	///< Cyan-Magenta
	CC, 	///< Cyan-Cyan
	CW, 	///< Cyan-White
// white
	WBk, 	///< White-Black
	WR, 	///< White-Red
	WG, 	///< White-Green
	WY, 	///< White-Yellow
	WBl, 	///< White-Blue
	WM,		///< White-Magenta
	WC, 	///< White-Cyan
	WW 	///< White-White
};

/**
 * The codes for printing colored output at stdout
 */
char *Tcolor (enum colors color);

#endif
