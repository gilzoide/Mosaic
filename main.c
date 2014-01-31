#include "nmos.h"
#include <ctype.h>

int main () {
	CursInit ();
	WINDOW *hud = CreateHud ();
	
	Cursor position;	position.x = position.y = 0;
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
				
			case KEY_UP:
				Move (&position, current, UP);
				break;

			case KEY_SUP:
				default_direction = UP;
				break;

			case KEY_DOWN:
				Move (&position, current, DOWN);
				break;
				
			case KEY_SDOWN:
				default_direction = DOWN;
				break;
				
			case KEY_LEFT:
				Move (&position, current, LEFT);
				break;
				
			case KEY_SLEFT:
				default_direction = LEFT;
				break;
				
			case KEY_RIGHT:
				Move (&position, current, RIGHT);
				break;
				
			case KEY_SRIGHT:
				default_direction = RIGHT;
				break;
				
			case KEY_PPAGE:
				if (current->prev != NULL)
					current = current->prev;
				break;
				
			case KEY_NPAGE:
				if (current->next != NULL)
					current = current->next;
				break;

			case KEY_F(1):
				Help ();
				break;
				
			case KEY_CTRL_S:
				SaveImg (current, "teste.mosi");
				PrintHud (hud, "Saved successfully!");
				break;
			
			/// @todo arrumar isso
			case KEY_BACKSPACE:
				current->img.mosaic[current->img.width*position.y + position.x] = ' ';
				echochar (' ');
				break;
				
			default:
				if (isprint (c)) {
					current->img.mosaic[current->img.width*position.y + position.x] = c;
					mvwaddch (current->win, position.y, position.x, c);
					wrefresh (current->win);
					Move (&position, current, default_direction);
				}
		}
		
		UpdateHud (hud, position, default_direction);
		c = getch ();
	}
	

	DestroyIMGS (&everyone);

	endwin ();
	return 0;
}
