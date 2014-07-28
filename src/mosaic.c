#include "mosaic.h"
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
	int c = PrintHud ("New default direction (arrow keys)");
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


void Copy (CopyBuffer *buffer, MOSIMG *current, Cursor selection) {
	// if there was something stored, bye bye
	DestroyCopyBuffer (buffer);

	// copy buffer is a copy of the entire window, since the curses implementation certainly is better than I would do =P
	buffer->buff = dupwin (current->win);
	// set it's coordinates, so we know what part the user want to copy
	buffer->coordinates.origin_y = min (selection.y, selection.origin_y);
	buffer->coordinates.origin_x = min (selection.x, selection.origin_x);
	buffer->coordinates.y = abs (selection.y - selection.origin_y);
	buffer->coordinates.x = abs (selection.x - selection.origin_x);
}


void Cut (CopyBuffer *buffer, MOSIMG *current, Cursor selection) {
	// we copy to the buffer
	Copy (buffer, current, selection);
	// and erase what was in there
	int i, j;
	int y = min (selection.y, selection.origin_y), x = min (selection.x, selection.origin_x);	// we need the upper-left corner only
	for (i = 0; i <= buffer->coordinates.y; i++) {
		for (j = 0; j <= buffer->coordinates.x; j++) {
			mosAddch (current, y + i, x + j, ' ');
		}
	}
	DisplayCurrentImg (current);
}


char Paste (CopyBuffer *buffer, MOSIMG *current, Cursor cursor) {
	if (buffer->buff != NULL) {
		int i, j, c;
		for (i = 0; i <= buffer->coordinates.y; i++) {
			for (j = 0; j <= buffer->coordinates.x; j++) {
				// read the char from the CopyBuffer
				c = mvwinch (buffer->buff, buffer->coordinates.origin_y + i, buffer->coordinates.origin_x + j);
				// if transparent pasting, and it's a ' ', leave the old char there
				if (IS_(TRANSPARENT) && c == ' ')
					continue;
				// if outside MOSIMG window, don't try to put 'c' in it, or it'll crash
				else if (mosAddch (current, cursor.y + i, cursor.x + j, c) == ERR)
					break;
			}
		}

		DisplayCurrentImg (current);
		return 1;
	}
	else
		return 0;
}


MOSIMG *CreateNewImg (IMGS *everyone, MOSIMG *current) {
	int height = INITIAL_HEIGHT, width = INITIAL_WIDTH;
	enum direction dir;

	// user canceled the creation: return NULL
	if (AskCreateNewImg (&height, &width, &dir) == ERR)
		return NULL;
	// one more IMG
	everyone->size++;
	
	MOSIMG *new_image = NewMOSIMG (height, width);
	
	// first image: no one's after or before
	if (everyone->list == NULL) {
		everyone->list = new_image;
		new_image->next = new_image->prev = new_image;
	}
	// not the first, so link it to someone
	else
		LinkImg (current, new_image, dir);
	
	return new_image;
}

void RefreshMOSIMG (MOSIMG *current) {
	wmove (current->win, 0, 0);
	
	// write in the WINDOW
	int i, j;
	for (i = 0; i < current->img.height; i++) {
		for (j = 0; j < current->img.width; j++) {
			mvwaddch (current->win, i, j, current->img.mosaic[i][j]);
		}
	}
	
	// display it
	DisplayCurrentImg (current);
}


void DestroyIMGS (IMGS *everyone) {
	MOSIMG *aux, *next;
	int i;
	
	for (aux = everyone->list, i = 0; i < everyone->size; i++, aux = next) {
		next = aux->next;
		FreeMOSIMG (aux);
		free (aux);
	}
}


