/** @file color.h
 * Color defines and initialization
 * 
 * @note Colors are defined in format: _foreground-background_
 */

#include <ncurses.h>

/// Initialize all color combinations
void InitColors ();
/// A simple test of the color definitions
void TestColors ();

// By Foregrounds:
// black
#define BkBk 1	///< Black-Black
#define BkR 2	///< Black-Red
#define BkG 3	///< Black-Green
#define BkY 4	///< Black-Yellow
#define BkBl 5	///< Black-Blue
#define BkM 6	///< Black-Magenta
#define BkC 7	///< Black-Cyan
#define BkW 8	///< Black-White
// red
#define RBk 9	///< Red-Black
#define RR 10	///< Red-Red
#define RG 11	///< Red-Green
#define RY 12	///< Red-Yellow
#define RBl 13	///< Red-Blue
#define RM 14	///< Red-Magenta
#define RC 15	///< Red-Cyan
#define RW 16	///< Red-White
// green
#define GBk 17	///< Green-Black
#define GR 18	///< Green-Red
#define GG 19	///< Green-Green
#define GY 20	///< Green-Yellow
#define GBl 21	///< Green-Blue
#define GM 22	///< Green-Magenta
#define GC 23	///< Green-Cyan
#define GW 24	///< Green-White
// yellow
#define YBk 25	///< Yellow-Black
#define YR 26	///< Yellow-Red
#define YG 27	///< Yellow-Green
#define YY 28	///< Yellow-Yellow
#define YBl 29	///< Yellow-Blue
#define YM 30	///< Yellow-Magenta
#define YC 31	///< Yellow-Cyan
#define YW 32	///< Yellow-White
// blue
#define BlBk 33	///< Blue-Black
#define BlR 34	///< Blue-Red
#define BlG 35	///< Blue-Green
#define BlY 36	///< Blue-Yellow
#define BlBl 37	///< Blue-Blue
#define BlM 38	///< Blue-Magenta
#define BlC 39	///< Blue-Cyan
#define BlW 40	///< Blue-White
// magenta
#define MBk 41	///< Magenta-Black
#define MR 42	///< Magenta-Red
#define MG 43	///< Magenta-Green
#define MY 44	///< Magenta-Yellow
#define MBl 45	///< Magenta-Blue
#define MM 46	///< Magenta-Magenta
#define MC 47	///< Magenta-Cyan
#define MW 48	///< Magenta-White
// cyan
#define CBk 49	///< Cyan-Black
#define CR 50	///< Cyan-Red
#define CG 51	///< Cyan-Green
#define CY 52	///< Cyan-Yellow
#define CBl 53	///< Cyan-Blue
#define CM 54	///< Cyan-Magenta
#define CC 55	///< Cyan-Cyan
#define CW 56	///< Cyan-White
// white
#define WBk 57	///< White-Black
#define WR 58	///< White-Red
#define WG 59	///< White-Green
#define WY 60	///< White-Yellow
#define WBl 61	///< White-Blue
#define WM 62	///< White-Magenta
#define WC 63	///< White-Cyan
#define WW 64	///< White-White
