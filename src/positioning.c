#include "positioning.h"
#include "wins.h"

void InitCursor (Cursor *cur) {
	cur->x = cur->y = cur->origin_x = cur->origin_y = 0;
}


void DisplayCurrentMOSAIC (CURS_MOS *current) {
	show_panel (current->pan);
	update_panels ();
	doupdate ();
	prefresh (current->win,
			current->y * MOSAIC_PAD_HEIGHT, current->x * MOSAIC_PAD_WIDTH,
			0, 0, MOSAIC_PAD_HEIGHT - 1, MOSAIC_PAD_WIDTH - 1);
}


void PrintSelection (Cursor *position, CURS_MOS *current) {
	int ULy = min (position->origin_y, position->y);
	int ULx = min (position->origin_x, position->x);
	int BRy = max (position->origin_y, position->y);
	int BRx = max (position->origin_x, position->x);

	// undo
	UnprintSelection (current);

	// redo
	int i;
	for (i = ULy; i <= BRy; i++) {
		mvwchgat (current->win, i, ULx, BRx - ULx + 1, A_REVERSE, Normal, NULL);
	}

	DisplayCurrentMOSAIC (current);
}


void UnprintSelection (CURS_MOS *current) {
	RewriteCURS_MOS (current);
}


void MoveTo (Cursor *position, CURS_MOS *current, int y, int x) {
	// make sure y and x are within 'current'
	y = max (y, 0);
	x = max (x, 0);
	y = min (y, current->img->height - 1);
	x = min (x, current->img->width - 1);

	// change the position
	position->y = y;
	position->x = x;

	if (!IS_(SELECTION)) {
		position->origin_y = y;
		position->origin_x = x;
	}
	else {
		PrintSelection (position, current);
	}

	// cache old CURS_MOS::y/x
	int old_curs_mos_y = current->y;
	int old_curs_mos_x = current->x;
	
	// update
	current->y = y / MOSAIC_PAD_HEIGHT;
	current->x = x / MOSAIC_PAD_WIDTH;

	// if we changed block, reprint it all
	if (current->y != old_curs_mos_y || current->x != old_curs_mos_x) {
		erase ();
		ReHud ();
		ENTER_(REDRAW);
	}
}


void Move (Cursor *position, CURS_MOS *current, Direction dir) {
	// change the cursor position, deppending on the direction
	switch (dir) {
		case UP:
			--position->y;
			break;

		case DOWN:
			++position->y;
			break;

		case LEFT:
			--position->x;
			break;

		case RIGHT:
			++position->x;
			break;
	}

	MoveTo (position, current, position->y, position->x);
}


void MoveAll (Cursor *position, CURS_MOS *current, Direction dir) {
	// change the cursor position, depending on the direction
	switch (dir) {
		case UP:
			position->y = 0;
			break;
		
		case DOWN:
			position->y = current->img->height - 1;
			break;
			
		case LEFT:
			position->x = 0;
			break;
			
		case RIGHT:
			position->x = current->img->width - 1;
			break;
	}

	MoveTo (position, current, position->y, position->x);
}


void MoveResized (Cursor *position, CURS_MOS *current) {
	MoveTo (position, current, min (position->y, current->img->height),
			min (position->x, current->img->width));
}


CURS_MOS * GoToPage (IMGS *everyone, unsigned int index) {
	if (index < everyone->size) {
		CURS_MOS *aux;
		for (aux = everyone->list; index; index--) {
			aux = aux->next;
		}

		return aux;
	}
	else {
		return NULL;
	}
}


void ChangeDefaultDirection (int c, Direction *dir) {
	switch (c) {
		case KEY_UP:	*dir = UP;		break;
		case KEY_DOWN:	*dir = DOWN;	break;
		case KEY_LEFT:	*dir = LEFT;	break;
		case KEY_RIGHT:	*dir = RIGHT;	break;
	}
}
