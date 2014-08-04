#include "mosaic.h"
#include "argpstuff.h"

int main (int argc, char *argv[]) {
	const char *file_name = arguments (argc, argv);
	CursInit ();
	
	// initialize stuff
	Cursor cursor;
	InitCursor (&cursor);
	Direction default_direction = RIGHT;
	CopyBuffer buffer;
	InitCopyBuffer (&buffer);
	IMGS everyone;
	InitIMGS (&everyone);

	ENTER_(TRANSPARENT);	// it's a test: it does work xD

	int c = 0;
	// Mouse support: bt1 and bt3 click
	MEVENT event;
	mousemask (BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED | BUTTON3_CLICKED, NULL);

	// the current image in edition
	MOSIMG *current = NULL;
	// we really need a current image, so ask for it until user creates it!
	// but if asked to open a file directly, create an Img and loads it
	if (file_name) {
		ungetch ('\n');
		c = KEY_CTRL_O;
	}
	while (!(current = CreateNewImg (&everyone, current)));

	while (!IS_(QUIT)) {
		if (c == KEY_F(10)) {
			c = Menu ();
			DisplayCurrentImg (current);
		}

		//~ printw ("%d ", c);
		switch (c) {
			/* if nothing is returned by the menu, do nothing */
			case 0:	
				break;

			/* ESC: exit selection mode */
			case KEY_ESC:
				UN_(SELECTION);
				UnprintSelection (current);
				break;

			/* Mouse event: clicked the window, or help/menu/quit */
			case KEY_MOUSE:
				getmouse (&event);
				// bt1 click: MoveTo
				if (event.bstate & BUTTON1_CLICKED)
					MoveTo (&cursor, current, event.y, event.x);
				// bt3 (right button) click: Menu (yep, anywhere)
				else if (event.bstate & BUTTON3_CLICKED)
					ungetch (KEY_F(10));
				break;
				
			/* move up */
			case KEY_UP:
				Move (&cursor, current, UP);
				break;

			/* move down */
			case KEY_DOWN:
				Move (&cursor, current, DOWN);
				break;

			/* move left */
			case KEY_LEFT:
				Move (&cursor, current, LEFT);
				break;

			/* move right */
			case KEY_RIGHT:
				Move (&cursor, current, RIGHT);
				break;

			/* next click change direction */
			case KEY_CTRL_D:
				DefaultDirection (&default_direction);
				break;
				
			/* previous mosaic */
			case KEY_PPAGE:
				current = current->prev;
				DisplayCurrentImg (current);
				break;
				
			/* next mosaic */
			case KEY_NPAGE:
				current = current->next;
				DisplayCurrentImg (current);
				break;

			/* move to first */
			case KEY_HOME:
				MoveAll (&cursor, current, REVERSE (default_direction));
				break;
			
			/* move to last */
			case KEY_END:
				MoveAll (&cursor, current, default_direction);
				break;

			/* show help */
			case KEY_F(1):
				Help ();
				DisplayCurrentImg (current);
				break;
				
			/* new mosaic */
			case KEY_F(2):
				; MOSIMG *aux = CreateNewImg (&everyone, current);
				if (aux)
					current = aux;
				DisplayCurrentImg (current);
				break;
				
			/* save mosaic */
			case KEY_CTRL_S:
				switch (SaveMOSIMG (current)) {
					case 0:
						PrintHud ("Saved successfully!");
						break;

					case ERR:	// canceled
						break;

					default:
						PrintHud ("Sorry, no can save this... =/");
						break;
				}
				UN_(TOUCHED);
				break;
				
			/* load mosaic */
			case KEY_CTRL_O:
				switch (LoadMOSIMG (current)) {
					case 0:
						RefreshMOSIMG (current);
						PrintHud ("Loaded successfully!");
						ENTER_(TOUCHED);
						break;

					case 1:
						PrintHud ("No dimensions in this file, dude! =/");
						break;

					case ERR:	// canceled
						break;

					case ENOENT:
						PrintHud ("File doesn't exist");
						break;

					default:
						PrintHud ("Sorry, no can load this... =/");
						break;
				}
				break;
				
			/* resize mosaic */
			case KEY_CTRL_R:
				werase (current->win);
				wrefresh (current->win);
				ResizeMOSIMG (current, 20, 30);
				RefreshMOSIMG (current);
				break;
				
			/* box selection mode! */
			case KEY_CTRL_B:
				TOGGLE_(SELECTION);
				UnprintSelection (current);
				break;
				
			/* copy selected area */
			case KEY_CTRL_C:	
				UnprintSelection (current);
				UN_(SELECTION);
				Copy (&buffer, current, cursor);
				break;

			/* cut selected area */
			case KEY_CTRL_X:
				UnprintSelection (current);
				UN_(SELECTION);
				Cut (&buffer, current, cursor);
				break;

			/* paste copy buffer */
			case KEY_CTRL_V:	
				UnprintSelection (current);
				UN_(SELECTION);
				if (!Paste (&buffer, current, cursor))
					PrintHud ("Nothing in the buffer...");
				else
					ENTER_(TOUCHED);
				break;

			/* attribute table */
			case '\t':
				wattrset (current->win, AttrTable (current, cursor));
				break;
				
			/* quit; aww =/ */
			case KEY_CTRL_Q:
				// asks if you really want to quit this tottally awesome SW
				if (IS_(TOUCHED) && !AskQuit ())
					break;

				ENTER_(QUIT);
				// needed to jump the getch ()
				ungetch (0);
				break;
				
			/* erase entire line/column before cursor
			 * it actually calls enough times the backspace button */
			case KEY_CTRL_U:
				; int i = max (current->img.height, current->img.width);
				while (i > 0 && i--)
					ungetch (KEY_BACKSPACE);
				break;
			
			// WARNING: don't change BACKSPACE nor DC out of here nor out of
			// order, as they deppend on 'default' 
			// (so I guess you know we shouldn't put any 'break's either)
			/* Backspace: delete the char before (the curses definition 
			 * says something else, but in general it's 127) */
			case KEY_BACKSPACE: case 127:	
				Move (&cursor, current, REVERSE (default_direction));
			/* delete: well, just erase the damn char 
			 * (put a ' ' in it, default takes care of this for us =P) */
			case KEY_DC:	
				ENTER_(ERASED);
				c = ' ';

			/* write at the mosaic, and show it to us */
			default:
				if (isprint (c)) {
					mosAddch (current, cursor.y, cursor.x, c);
					DisplayCurrentImg (current);
					ENTER_(TOUCHED);
					// didn't erase anything, so move to the next
					if (!IS_(ERASED))	
						Move (&cursor, current, default_direction);
					else 	// we erased something, so it erased and that's that
						UN_(ERASED);
				}
				break;
		}
		
		UpdateHud (cursor, default_direction);
		
		c = getch ();
	}
	
	DestroyCopyBuffer (&buffer);
	DestroyIMGS (&everyone);
	DestroyWins ();

	endwin ();
	return 0;
}
