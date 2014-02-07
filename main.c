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
	
	MOSIMG *current = CreateNewImg (&everyone);
	
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
				DisplayCurrentPanel (current);
				break;
				
			case KEY_NPAGE:	// next mosaic
				current = current->next;
				DisplayCurrentPanel (current);
				break;

			case KEY_F(1):	// show help
				Help ();
				break;
				
			case KEY_F(2):	// new mosaic
				current = CreateNewImg (&everyone);
				DisplayCurrentPanel (current);
				break;
				
			case KEY_CTRL_S:	// save mosaic
				SaveImg (current, "teste.mosi");
				PrintHud (hud, "Saved successfully!");
				break;
				
			case KEY_CTRL_Q:	// quit; aww =/
				// asks if you really want to quit this tottally awesome software
				break;
				
			case '\t':	// attribute table
				wattrset (current->win, AttrTable (current, cursor));
				break;
			
			/// @todo get this right, backspace
			case KEY_DC:
				c = ' ';
				
			default:	// write at the mosaic, and show it to us
				if (isprint (c)) {
					current->img.mosaic[cursor.y][cursor.x] = c;
					mvwaddch (current->win, cursor.y, cursor.x, c);
					wrefresh (current->win);
					Move (&cursor, current, default_direction);
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
