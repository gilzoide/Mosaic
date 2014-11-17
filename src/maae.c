#include "maae.h"
#include <locale.h>

void CursInit () {
	setlocale (LC_ALL, "");	// wide_chars!

	initscr ();	// init curses screen

	keypad (stdscr, TRUE);	// we can now use the arrow keys and Fn keys
	raw ();	// no need to wait for the RETURN key [for interactive means]
	noecho ();

	set_escdelay (0);	// no need to wait for the Esc key (we don't use the meta modifier)

	start_color ();	// Colors!
	InitColors ();	// initialize all the colors -> color.c
	InitHud ();	// initialize the HUD
}


char getBit (char c, int i) {
	char out = c & (0b10000000 >> i);
	return out >> (8 - i);
}


int toUTF8 (int c) {
	int out = 0;

	// it's a multibyte UTF-8 char
	if (c > 127) {
		int i, j;
		for (i = 0; i < 8; i++) {
			// stopping condition; we still need to read the information here
			if (!(c & (0b10000000 >> i))) {
				for ( ; i < 8; i++) {
					out <<= 1;
					out += getBit (c, i);
				}
				break;
			}
		}
		int bytes = i;
		for (i = 0; i < bytes; i++) {
			for (j = 0; j < 6; j++) {
				out <<= 1;
				out += getBit (c, j + 2);
			}
		}
	}
	else {
		return c;
	}

	return out;
}


void DefaultDirection (Direction *dir) {
	int c = PrintHud ("New default direction (arrow keys)", TRUE);
	ChangeDefaultDirection (c, dir);
}


void InitCopyBuffer (CopyBuffer *buffer) {
	buffer->buff = NULL;
	buffer->coordinates.x = buffer->coordinates.y = buffer->coordinates.origin_y = buffer->coordinates.origin_x = 0;
}


void DestroyCopyBuffer (CopyBuffer *buffer) {
	if (buffer != NULL)
		delwin (buffer->buff);
}


void Copy (CopyBuffer *buffer, CURS_MOS *current, Cursor selection) {
	// if there was something stored, bye bye
	DestroyCopyBuffer (buffer);

	// copy buffer is a copy of the entire window, since the
	// curses implementation certainly is better than I would do =P
	buffer->buff = dupwin (current->win);
	// set it's coordinates, so we know what part the user want to copy
	buffer->coordinates.origin_y = min (selection.y, selection.origin_y);
	buffer->coordinates.origin_x = min (selection.x, selection.origin_x);
	buffer->coordinates.y = abs (selection.y - selection.origin_y);
	buffer->coordinates.x = abs (selection.x - selection.origin_x);
}


void Cut (CopyBuffer *buffer, CURS_MOS *current, Cursor selection) {
	// we copy to the buffer
	Copy (buffer, current, selection);
	// and erase what was in there
	int i, j;
	int y = min (selection.y, selection.origin_y);	// we need the upper-left
	int x = min (selection.x, selection.origin_x);	// corner only
	for (i = 0; i <= buffer->coordinates.y; i++) {
		for (j = 0; j <= buffer->coordinates.x; j++) {
			curs_mosAddch (current, y + i, x + j, ' ');
		}
	}
	DisplayCurrentMOSAIC (current);
}


char Paste (CopyBuffer *buffer, CURS_MOS *current, Cursor cursor) {
	if (buffer->buff != NULL) {
		int i, j, c;
		for (i = 0; i <= buffer->coordinates.y; i++) {
			for (j = 0; j <= buffer->coordinates.x; j++) {
				// read the char from the CopyBuffer
				c = mvwinch (buffer->buff,
						buffer->coordinates.origin_y + i,
						buffer->coordinates.origin_x + j);
				// if transparent pasting, and it's a ' ',
				// leave the old char there
				if (IS_(TRANSPARENT) && c == ' ')
					continue;
				// if outside CURS_MOS window, don't try to put 'c' in it, or it'll crash
				else if (curs_mosAddch (current, cursor.y + i, cursor.x + j, c) == ERR)
					break;
			}
		}

		DisplayCurrentMOSAIC (current);
		return 1;
	}
	else {
		return 0;
	}
}


CURS_MOS *CreateNewMOSAIC (IMGS *everyone, CURS_MOS *current) {
	int height = INITIAL_HEIGHT, width = INITIAL_WIDTH;
	enum direction dir;
	char duplicate = 0;

	// user canceled the creation: return NULL
	if (AskCreateNewMOSAIC (&height, &width, &duplicate, &dir) == ERR) {
		return NULL;
	}

	CURS_MOS *new_image = NewCURS_MOS (height, width);

	// copy if asked for a duplicate
	if (duplicate) {
		CopyMOSAIC (new_image->img, current->img);
		RefreshCURS_MOS (new_image);
	}

	// now there's one more CURS_MOS
	everyone->size++;

	// first image: no one's after or before
	if (everyone->list == NULL) {
		CircularIMGS (everyone, new_image);
	}
	// not the first, so link it to someone
	else {
		LinkCURS_MOS (current, new_image, dir);
	}

	return new_image;
}


void ChAttrs (CURS_MOS *current, Cursor *cur, Attr attr) {
	int y;
	int x;
	if (IS_(SELECTION)) {
		int ULy = min (cur->origin_y, cur->y);
		int ULx = min (cur->origin_x, cur->x);
		int BRy = max (cur->origin_y, cur->y);
		int BRx = max (cur->origin_x, cur->x);

		for (y = ULy; y <= BRy; y++) {
			for (x = ULx; x <= BRx; x++) {
				curs_mosSetAttr (current, y, x, attr);
			}
		}

		// Retract selection
		UN_(SELECTION);
		cur->origin_y = cur->y;
		cur->origin_x = cur->x;
		// don't move after input, please
		ENTER_(NO_MOVING_CURSOR);
		// don't insert c at next position
		return;
	}
	// normal insertion
	y = cur->y;
	x = cur->x;
	curs_mosSetAttr (current, y, x, attr);
}


void InsertCh (CURS_MOS *current, Cursor *cur, int c, Direction dir) {
	int y = cur->y;
	int x = cur->x;
	// insert mode: need to push everyone one char in dir
	if (IS_(INSERT)) {
		// moving coordinate and the stop point (cur->x or cur->y)
		// 	note that moving is just a reference to the moving variable,
		// 	so that we don't need to know which one is the moving one at
		// 	the verification
		int *moving, end;
		switch (dir) {
			case UP:
				y = 0;
				moving = &y;
				end = cur->y;
				break;

			case DOWN:
				y = current->img->height;
				moving = &y;
				end = cur->y;
				break;

			case LEFT:
				x = 0;
				moving = &x;
				end = cur->x;
				break;

			case RIGHT:
				x = current->img->width;
				moving = &x;
				end = cur->x;
				break;

			default:
				moving = &y;
				end = cur->y;
		}

		while (*moving != end) {
			// update target_y/target_x: where we'll put the copied chars
			int target_y = y;
			int target_x = x;
			// go in reverse moving the chars
			switch (REVERSE (dir)) {
				case UP:	--y;	break;
				case DOWN:	++y;	break;
				case LEFT:	--x;	break;
				case RIGHT:	++x;	break;
			}
			// read next char
			mos_char aux = mosGetch (current->img, y, x);
			// add it in it's new place
			curs_mosAddch (current, target_y, target_x, aux);
		}

		// redraw WINDOW
		RefreshCURS_MOS (current);
	}
	// selection mode: fill the selection with the char c
	else if (IS_(SELECTION)) {
		int ULy = min (cur->origin_y, cur->y);
		int ULx = min (cur->origin_x, cur->x);
		int BRy = max (cur->origin_y, cur->y);
		int BRx = max (cur->origin_x, cur->x);

		for (y = ULy; y <= BRy; y++) {
			for (x = ULx; x <= BRx; x++) {
				curs_mosAddch (current, y, x, c);
			}
		}

		// Retract selection
		UN_(SELECTION);
		cur->origin_y = cur->y;
		cur->origin_x = cur->x;
		// don't move after input, please
		ENTER_(NO_MOVING_CURSOR);
		// don't insert c at next position
		return;
	}
	// normal insertion
	curs_mosAddch (current, y, x, c);
}


int Load (CURS_MOS *current) {
	char *file_name = AskSaveLoadMOSAIC (load);

	// if user canceled
	if (!file_name) {
		return ERR;
	}
	else {
		return LoadCURS_MOS (current, file_name);
	}
}


int Save (CURS_MOS *current) {
	char *file_name = AskSaveLoadMOSAIC (save);

	if (!file_name) {
		return ERR;
	}
	else {
		if (!strstr (file_name, ".mosi")) {
			strcat (file_name, ".mosi");
		}

		return SaveMOSAIC (current->img, file_name);
	}
}


void EraseLine (CURS_MOS *current) {
	// just take the greater, so we don't need to worry about the direction
	int i = max (current->img->height, current->img->width);

	while (i > 0 && i--) {
		// let main's erasure work it's magic
		ungetch (KEY_BACKSPACE);
	}
}


void EraseWord (Cursor *cursor, CURS_MOS *current, Direction dir) {
	int y = cursor->y;
	int x = cursor->x;

	// count the chars, until a blank (' ') or end of line (out of bounds)
	int i = 0, c;
	do {
		// move to get the next char
		switch (dir) {
			case UP:	--y;	break;
			case DOWN:	++y;	break;
			case LEFT:	--x;	break;
			case RIGHT:	++x;	break;
		}
		// let's check what's in y/x
		c = mosGetch (current->img, y, x);
		// one more char to erase
		i++;
	} while (c && c != ' ');

	while (i > 0 && i--) {
		// let main's erasure work it's magic for the counted chars
		ungetch (KEY_BACKSPACE);
	}
}


void InformToggleState (State S, const char *if_s, const char *if_not_s) {
	TOGGLE_(S);
	IS_(S) ? PrintHud (if_s, FALSE) : PrintHud (if_not_s, FALSE);
}


void DestroyIMGS (IMGS *everyone) {
	CURS_MOS *aux, *next;
	int i;

	for (aux = everyone->list, i = 0; i < everyone->size; i++, aux = next) {
		next = aux->next;
		FreeCURS_MOS (aux);
	}
}
