#include "nmos.h"
#include <locale.h>


void CursInit () {
	setlocale (LC_ALL, "");	// wide_chars!

	initscr ();	// init curses screen
	
	keypad (stdscr, TRUE);	// we can now use the arrow keys and Fn keys
	raw ();	// no need to wait for the RETURN key [for interactive means]
	noecho ();
	
	start_color ();	// Colors!
	assume_default_colors (-1, -1);	// there's the default terminal color
	InitColors ();	// initialize all the colors -> color.c
}


void DefaultDirection (WINDOW *hud, Direction *dir) {
	int c = PrintHud (hud, "new default direction (arrow keys)");
	ChangeDefaultDirection (c, dir);
}


void InitCopyBuffer (CopyBuffer *buffer) {
	buffer->buff = NULL;
	buffer->coordinates.x = buffer->coordinates.y = buffer->coordinates.origin_y = buffer->coordinates.origin_x = 0;
}


inline void DestroyCopyBuffer (CopyBuffer *buffer) {
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


int Paste (CopyBuffer *buffer, MOSIMG *current, Cursor cursor) {
	if (buffer->buff != NULL) {
		int i, j, c;
		for (i = 0; i <= buffer->coordinates.y; i++) {
			for (j = 0; j <= buffer->coordinates.x; j++) {
				// read the char
				c = mvwinch (buffer->buff, buffer->coordinates.origin_y + i, buffer->coordinates.origin_x + j);
				// if outside MOSIMG window, don't try to put 'c' in it, or it'll crash
				if (mosAddch (current, cursor.y + i, cursor.x + j, c) == ERR)
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
	int new_height = INITIAL_HEIGHT, new_width = INITIAL_WIDTH;
	everyone->size++;
	
	MOSIMG *new_image = NewMOSIMG (new_height, new_width);
	
	// first image: no one's after or before
	if (everyone->list == NULL) {
		everyone->list = new_image;
		new_image->next = new_image->prev = new_image;
	}
	// not the first, so link it to someone
	else
		LinkImg (current, new_image, before);
	
	return new_image;
}

void RefreshMOSIMG (MOSIMG *current) {
	wmove (current->win, 0, 0);
	
	// write in the WINDOW
	int i, j;
	for (i = 0; i < current->img.height; i++) {
		for (j = 0;  j < current->img.width; j++) {
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
