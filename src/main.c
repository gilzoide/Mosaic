#include "maae.h"
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

	int c = 0;
	// Mouse support: bt1 and bt3 click
	MEVENT event;
	mousemask (BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED | 
			BUTTON3_CLICKED, NULL);

	// the current image in edition
	CURS_MOS *current = NULL;
	// we really need a current image, so ask for it until user creates it!
	// but if asked to open a file directly, creates a empty MOSAIC and loads it
	if (file_name) {
		current = NewCURS_MOS (0, 0);
		LoadCURS_MOS (current, file_name);
		CircularIMGS (&everyone, current);
		DisplayCurrentMOSAIC (current);
	}
	else while (!(current = CreateNewMOSAIC (&everyone, current)));

	while (!IS_(QUIT)) {
		if (c == KEY_F(10)) {
			c = Menu ();
			DisplayCurrentMOSAIC (current);
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
				if (event.bstate & BUTTON1_CLICKED) {
					MoveTo (&cursor, current, event.y, event.x);
				}
				// bt3 (right button) click: Menu (yep, anywhere)
				else if (event.bstate & BUTTON3_CLICKED) {
					ungetch (KEY_F(10));
				}
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
				DisplayCurrentMOSAIC (current);
				break;
				
			/* next mosaic */
			case KEY_NPAGE:
				current = current->next;
				DisplayCurrentMOSAIC (current);
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
				DisplayCurrentMOSAIC (current);
				break;
				
			/* new mosaic */
			case KEY_F(2):
				; CURS_MOS *aux = CreateNewMOSAIC (&everyone, current);
				// aux was really created, so update the current curs_mos
				if (aux) {
					current = aux;
				}
				DisplayCurrentMOSAIC (current);
				break;

			/* show about Window */
			case KEY_F(12):
				About ();
				break;
				
			/* save mosaic */
			case KEY_CTRL_S:
				switch (Save (current)) {
					case 0:
						PrintHud ("Saved successfully!", FALSE);
						break;

					case ERR:	// canceled
						break;

					default:
						PrintHud ("Sorry, no can save this... =/", TRUE);
						break;
				}
				UN_(TOUCHED);
				break;
				
			/* load mosaic */
			case KEY_CTRL_O:
				switch (Load (current)) {
					case 0:
						RefreshCURS_MOS (current);
						DisplayCurrentMOSAIC (current);
						PrintHud ("Loaded successfully!", FALSE);
						ENTER_(TOUCHED);
						break;

					case 1:
						PrintHud ("No dimensions in this file, dude! =/", TRUE);
						break;

					case ERR:	// canceled
						break;

					case ENOENT:
						PrintHud ("File doesn't exist", TRUE);
						break;

					default:
						PrintHud ("Sorry, no can load this... =/", TRUE);
						break;
				}
				break;
				
			/* resize mosaic */
			case KEY_CTRL_R:
				werase (current->win);
				wrefresh (current->win);
				ResizeCURS_MOS (current, 20, 30);
				RefreshCURS_MOS (current);
				break;
				
			/* box selection mode! */
			case KEY_CTRL_B:
				TOGGLE_(SELECTION);
				UnprintSelection (current);
				break;
			
			/* select all */
			case KEY_CTRL_A:
				// from the beggining...
				MoveTo (&cursor, current, 0, 0);
				// ...select...
				ENTER_(SELECTION);
				// ...until the end
				MoveTo (&cursor, current, 
						current->img.height - 1, current->img.width - 1);
				break;

			/* toggle transparent paste */
			case KEY_CTRL_T:
				TOGGLE_(TRANSPARENT);
				IS_(TRANSPARENT) ? 	PrintHud ("Transparent paste ON", FALSE) :
						PrintHud ("Transparent paste OFF", FALSE);
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
				// if the buffer was never used, sry
				if (!Paste (&buffer, current, cursor)) {
					PrintHud ("Nothing in the buffer...", TRUE);
				}
				else {
					ENTER_(TOUCHED);
				}
				break;

			case KEY_IC:
				TOGGLE_(INSERT);
				break;

			/* attribute table */
			case '\t':
				wattrset (current->win, AttrTable (current, cursor));
				break;
				
			/* quit; aww =/ */
			case KEY_CTRL_Q:
				// asks if you really want to quit this tottally awesome SW
				if (IS_(TOUCHED) && !AskQuit ()) {
					break;
				}

				ENTER_(QUIT);
				// needed to jump the getch ()
				ungetch (0);
				break;
				
			/* erase entire line/column before cursor
			 * it actually calls enough times the backspace button */
			case KEY_CTRL_U:
				EraseLine (current);
				break;

			/* erase word (until space is found) */
			case KEY_CTRL_W:
				EraseWord (&cursor, current, REVERSE (default_direction));
				break;
			
			// WARNING: don't change BACKSPACE nor DC out of here nor out of
			// order, as they deppend on 'default' 
			// (so I guess you know we shouldn't put any 'break's either)
			/* Backspace: delete the char before (the curses definition 
			 * says something else, but in general it's 127) */
			case KEY_BACKSPACE: case 127:	
				// in selection mode backspace acts just as delete
				if (!IS_(SELECTION)) {
					Move (&cursor, current, REVERSE (default_direction));
				}
			/* delete: well, just erase the damn char 
			 * (put a ' ' in it, default takes care of this for us =P) */
			case KEY_DC:	
				ENTER_(ERASED);
				c = ' ';

			/* write at the mosaic, and show it to us */
			default:
				if (isprint (c)) {
					InsertCh (current, cursor, c, default_direction);
					DisplayCurrentMOSAIC (current);
					ENTER_(TOUCHED);
					// didn't erase anything, so move to the next
					if (!IS_(ERASED)) {
						Move (&cursor, current, default_direction);
					}
					else { 	// we erased something, so it erased and that's that
						UN_(ERASED);
					}
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
