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
	CreateNewImg (&everyone);
	
	MOSIMG *current = everyone.list;
	
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

			case KEY_SUP:	// default direction is now up
				default_direction = UP;
				break;

			case KEY_DOWN:	// move down
				Move (&cursor, current, DOWN);
				break;
				
			case KEY_SDOWN:	// default direction is now down
				default_direction = DOWN;
				break;
				
			case KEY_LEFT:	// move left
				Move (&cursor, current, LEFT);
				break;
				
			case KEY_SLEFT:	// default direction is now left
				default_direction = LEFT;
				break;
				
			case KEY_RIGHT:	// move right
				Move (&cursor, current, RIGHT);
				break;
				
			case KEY_SRIGHT:	// default direction is now right
				default_direction = RIGHT;
				break;
				
			case KEY_PPAGE:	// previous mosaic
				current = current->prev;
				break;
				
			case KEY_NPAGE:	// next mosaic
				current = current->next;
				break;

			case KEY_F(1):	// show help
				Help ();
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
			case '\r':
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
