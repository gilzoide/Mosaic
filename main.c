#include "nmos.h"

int main () {
	CursInit ();
	WINDOW *hud = CreateHud ();
	
	Cursor position;	position.x = position.y = 0;
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
			case KEY_UP:
				if (position.y > 0)
					move (--position.y, position.x);
				break;

			case KEY_DOWN:
				if (position.y < current->height)
					move (++position.y, position.x);
				break;
				
			case KEY_LEFT:
				if (position.x > 0)
					move (position.y, --position.x);
				break;
				
			case KEY_RIGHT:
				if (position.x < current->width)
					move (position.y, ++position.x);
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
		}
		
		 c = getch ();
	}
	

	DestroyIMGS (&everyone);

	endwin ();
	return 0;
}
