#include "nmos.h"

int main () {
	CursInit ();
	WINDOW *hud = CreateHud ();
	
	Cursor position;
	IMGS everyone;
	InitIMGS (&everyone);
	
	int c;
	while ((c = getch ()) != CTRL_Q) {
		//printw ("%d ", c);
		
		switch (c) {
			case KEY_UP:
				if (position.y > 0)
					move (--position.y, position.x);
				break;

			case KEY_DOWN:
				if (position.y < CURRENT.height)
					move (++position.y, position.x);
				break;
				
			case KEY_LEFT:
				if (position.x > 0)
					move (position.y, --position.x);
				break;
				
			case KEY_RIGHT:
				if (position.x < CURRENT.width)
					move (position.y, ++position.x);
				break;
				
			case CTRL_INTERROGATION:
				Help ();
				break;
		}
	}
	

	DestroyImg (&CURRENT);

	endwin ();
	return 0;
}
