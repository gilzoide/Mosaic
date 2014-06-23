#include "mosaic.h"
#include <ctype.h>

int main (int argc, char *argv[]) {
	CursInit ();
	
	Cursor cursor;
	InitCursor (&cursor);
	Direction default_direction = RIGHT;
	CopyBuffer buffer;
	InitCopyBuffer (&buffer);
	IMGS everyone;
	InitIMGS (&everyone);
	
	MOSIMG *current = NULL;
	current = CreateNewImg (&everyone, current);
	
	ENTER_(TRANSPARENT);	// it's a test: it does work xD
	
	int i;
	int c = KEY_F(10);		// choice int -> starts with a F10: enter the menu
	// Mouse support: bt1 and bt3 click
	MEVENT event;
	mousemask (BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED | BUTTON3_CLICKED, NULL);
	
	while (c != KEY_CTRL_Q) {
		if (c == KEY_F(10)) {
			c = Menu ();
			DisplayCurrentImg (current);
		}
		
		//~ printw ("%d ", c);
		switch (c) {
			case 0:	// if nothing is returned by the menu, do nothing
				break;

			// Mouse event: clicked the window, or help/menu/quit
			case KEY_MOUSE:
				getmouse (&event);
				// bt1 click: MoveTo
				if (event.bstate & BUTTON1_CLICKED)
					MoveTo (&cursor, current, event.y, event.x);
				// bt3 (right button) click: Menu (yep, anywhere)
				else if (event.bstate & BUTTON3_CLICKED)
					ungetch (KEY_F(10));
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
				DefaultDirection (&default_direction);
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
				DisplayCurrentImg (current);
				break;
				
			case KEY_F(2):	// new mosaic
				current = CreateNewImg (&everyone, current);
				DisplayCurrentImg (current);
				break;
				
			case KEY_CTRL_S:	// save mosaic
				SaveImg (current, "teste.mosi");
				PrintHud ("Saved successfully!");
				UN_(TOUCHED);
				break;
				
			case KEY_CTRL_O:	// load mosaic
				switch (LoadImg (current, "teste.mosi")) {
					case 0:	RefreshMOSIMG (current), PrintHud ("Loaded successfully!"); break;
					case 1: PrintHud ("No dimensions in this file, dude! =/");	break;
					default: PrintHud ("Sorry, no can load this... =/"); break;
				}
				ENTER_(TOUCHED);
				break;
				
			case KEY_CTRL_R:	// resize mosaic
				werase (current->win);
				wrefresh (current->win);
				ResizeMOSIMG (current, 20, 30);
				RefreshMOSIMG (current);
				break;
				
			case KEY_CTRL_B:	// box selection mode!
				TOGGLE_(SELECTION);
				UnprintSelection (current);
				break;
				
			case KEY_CTRL_C:	// copy selected area
				UnprintSelection (current);
				UN_(SELECTION);
				Copy (&buffer, current, cursor);
				break;

			case KEY_CTRL_X:	// cut selected area
				UnprintSelection (current);
				UN_(SELECTION);
				Cut (&buffer, current, cursor);
				break;

			case KEY_CTRL_V:	// paste copy buffer
				UnprintSelection (current);
				UN_(SELECTION);
				if (!Paste (&buffer, current, cursor))
					PrintHud ("Nothing in the buffer...");
				else
					ENTER_(TOUCHED);
				break;

			case '\t':	// attribute table
				wattrset (current->win, AttrTable (current, cursor));
				break;
				
			case KEY_CTRL_Q:	// quit; aww =/
				// asks if you really want to quit this tottally awesome software
				break;
				
			case KEY_CTRL_U:	// erase entire line/column before cursor;		it actually calls enough times the backspace button
				i = max (current->img.height, current->img.width);
				while (i > 0 && i--)
					ungetch (KEY_BACKSPACE);
				break;
			
			// WARNING: don't change BACKSPACE nor DC out of here nor out of order, as they deppend on 'default' (so I guess you know we shouldn't put any 'break's either)
			case KEY_BACKSPACE: case 127:	// Backspace: delete the char before (the curses definition says something else, but in general it's 127)
				Move (&cursor, current, REVERSE (default_direction));
			case KEY_DC:	// delete: well, just erase the damn char (put a ' ' in it, default takes care of this for us =P)
				ENTER_(ERASED);
				c = ' ';

			default:	// write at the mosaic, and show it to us
				if (isprint (c)) {
					mosAddch (current, cursor.y, cursor.x, c);
					DisplayCurrentImg (current);
					ENTER_(TOUCHED);
					if (!IS_(ERASED))	// didn't erase anything, so move to the next
						Move (&cursor, current, default_direction);
					else 	// we erased something, so it erased and that's that
						UN_(ERASED);
				}
				break;
		}
		
		UpdateHud (cursor, default_direction);
		
		// Exit the program
		if (c == KEY_CTRL_Q) {
			break;
		}
		c = getch ();
	}
	
	DestroyCopyBuffer (&buffer);
	DestroyIMGS (&everyone);
	DestroyWins ();

	endwin ();
	return 0;
}
