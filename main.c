#include "nmos.h"

int main () {
	CursInit ();
	WINDOW *hud = CreateHud ();
	
	int c;
	while ((c = getch ()) != CTRL_Q) {
		switch (c) {
			case CTRL_H:
				Help ();
				break;
		}
		printw ("%d ", c);
	}
	


	endwin ();
	return 0;
}
