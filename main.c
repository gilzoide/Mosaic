#include "nmos.h"
#include <ctype.h>

int main () {
	CursInit ();
	WINDOW *hud = CreateHud ();
	
	Cursor cursor;
	InitCursor (&cursor);
	Direction default_direction = RIGHT;
	IMGS everyone;
	InitIMGS (&everyone);
	
	MOSIMG *current = CreateNewImg (&everyone, current);
	
	int c = KEY_ESC;
	while (c != KEY_CTRL_Q) {
		if (c == KEY_ESC)
			c = Menu ();
		
		//printw ("%d ", c);
		switch (c) {
			case 0:	// if nothing is returned by the menu, do nothing
				break;
				
			case KEY_UP:	// move up
				Move (&cursor, current, UP);
				break;

			case KEY_DOWN:	// move down
				Move (&cursor, current, DOWN);
				break;
				
			case KEY_LEFT:	// move left
				Move (&cursor, current, LEFT);
				break;
				
			case KEY_RIGHT:	// move right
				Move (&cursor, current, RIGHT);
				break;

			case KEY_CTRL_D:	// next click change direction
				ChangeDefaultDirection (hud, &default_direction);
				break;
				
			case KEY_PPAGE:	// previous mosaic
				current = current->prev;
				DisplayCurrentImg (current);
				break;
				
			case KEY_NPAGE:	// next mosaic
				current = current->next;
				DisplayCurrentImg (current);
				break;
				
			case KEY_HOME:
				MoveAll (&cursor, current, REVERSE (default_direction));
				break;
			
			case KEY_END:
				MoveAll (&cursor, current, default_direction);
				break;

			case KEY_F(1):	// show help
				Help ();
				break;
				
			case KEY_F(2):	// new mosaic
				current = CreateNewImg (&everyone, current);
				DisplayCurrentImg (current);
				break;
				
			case KEY_CTRL_S:	// save mosaic
				SaveImg (current, "teste.mosi");
				PrintHud (hud, "Saved successfully!");
				break;
				
			case KEY_CTRL_L:	// load mosaic
				switch (LoadImg (current, "teste.mosi")) {
					case 0:	PrintHud (hud, "Loaded successfully!"),	DisplayCurrentImg (current);	break;
					case 1: PrintHud (hud, "No dimensions in this file, dude! =/");	break;

					default:	PrintHud (hud, "Sorry, no can load this... =/");	break;
				}
				break;
				
			case KEY_CTRL_B:	// Box selection mode!
				TOGGLE_(SELECTION);
				break;
				
			case '\t':	// attribute table
				wattrset (current->win, AttrTable (current, cursor));
				break;
				
			case KEY_CTRL_Q:	// quit; aww =/
				// asks if you really want to quit this tottally awesome software
				break;
			
			case KEY_BACKSPACE: case 127:	// Backspace: delete the char before (the curses definition says something else, but in general it's 127)
				Move (&cursor, current, REVERSE (default_direction));
			case KEY_DC:	// delete: well, just erase the damn char (put a ' ' in it, default takes care of this for us =P)
				ENTER_(ERASED);
				c = ' ';
				
			default:	// write at the mosaic, and show it to us
				if (isprint (c)) {
					current->img.mosaic[cursor.y][cursor.x] = c;
					mvwaddch (current->win, cursor.y, cursor.x, c);
					wrefresh (current->win);
					if (!IS_(ERASED))	// didn't erase anything, so move to the next
						Move (&cursor, current, default_direction);
					else 	// we erased something, so it erased and that's that
						UN_(ERASED);
				}
				break;
		}
		
		UpdateHud (hud, cursor, default_direction);
		c = getch ();
	}
	
	DestroyIMGS (&everyone);

	endwin ();
	return 0;
}
