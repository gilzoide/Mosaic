#include "positioning.h"



void InitCursor (Cursor *cur) {
	cur->x = cur->y = cur->origin_x = cur->origin_y = 0;
}


void PrintSelection (Cursor *position, CURS_MOS *current) {
	int ULy = min (position->origin_y, position->y);
	int ULx = min (position->origin_x, position->x);
	int BRy = max (position->origin_y, position->y);
	int BRx = max (position->origin_x, position->x);
	
	UnprintSelection (current);
	
	int i;
	for (i = ULy; i <= BRy; i++) {
		mvwchgat (current->win, i, ULx, BRx - ULx + 1, A_REVERSE, 0, NULL);
	}
	/*
	char vertical = 0, horizontal = 0;
	
	switch (dir) {
		case UP:
			if (position.origin_y > position.y)
				mvwchgat (current->win, 
			else
			
			break;
			
		case DOWN:
			if (position.origin_y < position.y)
			
			else
			
			break;
			
		case LEFT:
			if (position.origin_x > position.x)
			
			else
			
			break;
			
		case RIGHT:
			if (position.origin_x < position.x)
			
			else
			
			break;
	}
	
	if (vertical < 0)
		mvwchgat (current->win, );
	else if	(vertical > 0)
		mvwchgat (current->win, );
	else if (horizontal < 0)
		mvwchgat (current->win, );
	else
		mvwchgat (current->win, );
	*/
	DisplayCurrentMOSAIC (current);
}


void UnprintSelection (CURS_MOS *current) {
	int i;
	for (i = 0; i < current->img->height; i++) {
		mvwchgat (current->win, i, 0, current->img->width, A_NORMAL, 0, NULL);
	}
	
	DisplayCurrentMOSAIC (current);
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

	move (y, x);
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


void ChangeDefaultDirection (int c, Direction *dir) {
	switch (c) {
		case KEY_UP:	*dir = UP;		break;
		case KEY_DOWN:	*dir = DOWN;	break;
		case KEY_LEFT:	*dir = LEFT;	break;
		case KEY_RIGHT:	*dir = RIGHT;	break;
	}
}
