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
	waddstr (win, "^?: ");
	wattroff (win, A_BOLD);
	waddstr (win, "Help");
	
	wrefresh (win);
	return win;
}


/* Displays the help (in a created window and panel, for going back to the normal field after) */
void Help () {
	WINDOW *help;
	PANEL *up;

	help = newwin (12, HELP_WIDTH, LINES - 15, 0);
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
	int c;
	
	WINDOW *menu;
	PANEL *up;

	menu = newwin (12, HELP_WIDTH, 0, 0);
	up = new_panel (menu);
	update_panels ();
	doupdate ();
	
	return c;
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
