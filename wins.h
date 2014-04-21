/** @file wins.h
 * Auxilary windows' definitions: help, menu, new/save/load
 */
 
#ifndef WINS_H
#define WINS_H

#define HELP_WIDTH COLS
#define HELP_HEIGHT (LINES - 1)

#include <curses.h>
#include <panel.h>
#include <menu.h>
#include "positioning.h"

/// Draw the HUD, with a few shortcuts and the cursor position
WINDOW *CreateHud ();
/// Updates the position in the HUD
void UpdateHud (WINDOW *hud, Cursor cur, Direction dir);
/** 
 * Prints a message in the HUD and waits for keystroke
 * 
 * @param[in] hud the HUD
 * @param[in] message Message to be written in the HUD
 * 
 * @return the key pressed for leaving
 */
int PrintHud (WINDOW *hud, const char *message);
/// Draw the non-interactive help screen
void Help ();
/// Show the options and actions interactive menu
int Menu ();
/// Show the attributes table
int AttrTable (MOSIMG *current, Cursor cur);



#endif
