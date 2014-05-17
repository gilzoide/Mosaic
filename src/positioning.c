#include "positioning.h"

inline int min (int a, int b) {
	return (a < b ? a : b);
}


inline int max (int a, int b) {
	return (a > b ? a : b);
}


void InitCursor (Cursor *cur) {
	cur->x = cur->y = cur->origin_x = cur->origin_y = 0;
}


void PrintSelection (Cursor *position, MOSIMG *current, Direction dir) {
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
	DisplayCurrentImg (current);
}


void UnprintSelection (MOSIMG *current) {
	int i;
	for (i = 0; i < current->img.height; i++)
		mvwchgat (current->win, i, 0, current->img.width, A_NORMAL, 0, NULL);
	
	DisplayCurrentImg (current);
}


void Move (Cursor *position, MOSIMG *current, Direction dir) {
	// change the cursor position, deppending on the direction
	switch (dir) {
		case UP:
			if (position->y > 0)
				--position->y;
			break;
		
		case DOWN:
			if (position->y < current->img.height - 1)
				++position->y;
			break;
			
		case LEFT:
			if (position->x > 0)
				--position->x;
			break;
			
		case RIGHT:
			if (position->x < current->img.width - 1)
				++position->x;
			break;
	}
	// if not in selection mode
	if (!IS_(SELECTION)) {
		// upper-left corner and bottom-right corner are one now
		position->origin_y = position->y;
		position->origin_x = position->x;
	}
	else
		PrintSelection (position, current, dir);

	// and move!
	move (position->y, position->x);
}


void MoveAll (Cursor *position, MOSIMG *current, Direction dir) {
	// change the cursor position, deppending on the direction
	switch (dir) {
		case UP:
			position->y = 0;
			break;
		
		case DOWN:
			position->y = current->img.height - 1;
			break;
			
		case LEFT:
			position->x = 0;
			break;
			
		case RIGHT:
			position->x = current->img.width - 1;
			break;
	}
	// if not in selection mode
	if (!IS_(SELECTION)) {
		// upper-left corner and bottom-right corner are one now
		position->origin_y = position->y;
		position->origin_x = position->x;
	}
	else
		PrintSelection (position, current, dir);

	// and move!
	move (position->y, position->x);
}


void ChangeDefaultDirection (int c, Direction *dir) {
	switch (c) {
		case KEY_UP:	*dir = UP;		break;
		case KEY_DOWN:	*dir = DOWN;	break;
		case KEY_LEFT:	*dir = LEFT;	break;
		case KEY_RIGHT:	*dir = RIGHT;	break;
	}
}
