#include "nmos.h"


void CursInit () {
	initscr ();	// init curses screen
	
	keypad (stdscr, TRUE);	// we can now use the arrow keys and Fn keys
	raw ();	// no need to wait for the RETURN key [for interactive means]
	noecho ();
	
	start_color ();	// Colors!
	assume_default_colors (-1, -1);	// there's the default terminal color
	InitColors ();	// initialize all the colors -> color.c
}


WINDOW *CreateHud () {
	WINDOW *win = subwin (stdscr, 1, COLS, LINES - 1, 0);
	
	wattron (win, A_BOLD);
	waddstr (win, "F1: ");
	wattroff (win, A_BOLD);
	waddstr (win, "Help   ");
	wattron (win, A_BOLD);
	waddstr (win, "Esc: ");
	wattroff (win, A_BOLD);
	waddstr (win, "Menu");
	
	wrefresh (win);
	return win;
}


void UpdateHud (WINDOW *hud, Cursor cur, Direction dir) {
	char c;
	switch (dir) {
		case UP: c = 'A'; break;
		case DOWN: c = 'V'; break;
		case LEFT: c = '<'; break;
		case RIGHT: c = '>'; break;
	}
	
	mvwprintw (hud, 0, COLS - 9, "%dx%d", cur.y, cur.x);
	wclrtoeol (hud);
	mvwprintw (hud, 0, COLS - 1, "%c", c);
	wrefresh (hud);
	move (cur.y, cur.x);
}


void PrintHud (WINDOW *hud, const char *message) {
	mvwaddstr (hud, 0, 23, message);
	wrefresh (hud);
	getch ();
	wmove (hud, 0, 23);
	wclrtoeol (hud);
	wrefresh (hud);
}


/* Displays the help (in a created window and panel, for going back to the normal field after) */
void Help () {
	WINDOW *help;
	PANEL *up;

	help = newwin (HELP_HEIGHT, HELP_WIDTH, LINES - 15, 0);
	up = new_panel (help);
	update_panels ();
	doupdate ();

	box (help, 0, 0);
	wbkgd (help, COLOR_PAIR (WBl));
	wrefresh (help);
	mvwaddstr (help, 0, HELP_WIDTH/2 - 2, "HELP");
	wattron (help, A_BOLD);
	mvwaddstr (help, 1, 1, "Arrow Keys or W,A,S,D:");
	mvwaddstr (help, 2, 1, "'z':");
	mvwaddstr (help, 3, 1, "Space:");
	mvwaddstr (help, 4, 1, "'q':");

	mvwaddstr (help, 6, 1, "O:");
	mvwaddstr (help, 7, 1,  "/VC");
	mvwaddstr (help, 8, 1, "\\_/:");

	mvwaddstr (help, 10, 1, "K:");

	wattrset (help, COLOR_PAIR (WBl));
	mvwaddstr (help, 1, 24, "turn");
	mvwaddstr (help, 2, 6, "change second head's side");
	mvwaddstr (help, 3, 8, "pause");
	mvwaddstr (help, 4, 6, "quit");
	mvwaddstr (help, 6, 4, "food: eat it for some points");
	mvwaddstr (help, 8, 6, "apple: eat it with both heads for 500 points!");
	mvwaddstr (help, 10, 4, "K stands for Hunter: it'll try to kill you!");

// writes the help window, wait for some key to be pressed and delete the help window
	wrefresh (help);
	getch ();

	werase (help);
	wrefresh (help);
	del_panel (up);
	delwin (help);
}


int Menu () {
	int c = 0;
	
	WINDOW *menu;
	PANEL *up;

	menu = newwin (12, HELP_WIDTH, 0, 0);
	up = new_panel (menu);
	update_panels ();
	doupdate ();
	
	return c;
}


inline void Move (Cursor *position, MOSIMG *current, Direction dir) {
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
	// and move!
	move (position->y, position->x);
}



void InitIMGS (IMGS *everyone) {
	everyone->list = NULL;
	everyone->size = 0;
}


void CreateNewImg (IMGS *everyone) {
	int new_height = 10, new_width = 10;
	everyone->size++;
	
	if (everyone->list == NULL) {
		everyone->list = NewImg (new_height, new_width);
		everyone->list->prev = everyone->list->next = NULL;
	}
}


void DestroyIMGS (IMGS *everyone) {
	MOSIMG *aux, *next;
	
	for (aux = everyone->list; aux != NULL; aux = next) {
		next = aux->next;
		FreeImg (aux);
	}
}
