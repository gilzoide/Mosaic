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
	waddstr (win, "Menu ");
	wattron (win, A_BOLD);
	waddstr (win, "^Q: ");
	wattroff (win, A_BOLD);
	waddstr (win, "quit");
	
	wrefresh (win);
	return win;
}


void UpdateHud (WINDOW *hud, Cursor cur, Direction dir) {
	int c;
	switch (dir) {
		case UP: c = ACS_UARROW; break;
		case DOWN: c = ACS_DARROW; break;
		case LEFT: c = ACS_LARROW; break;
		case RIGHT: c = ACS_RARROW; break;
	}
	
	mvwprintw (hud, 0, COLS - 11, "%dx%d", cur.y, cur.x);
	wclrtoeol (hud);
	mvwaddch (hud, 0, COLS - 1, c);
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
	curs_set (0);
	
	WINDOW *help;
	PANEL *up;

	help = newwin (HELP_HEIGHT, HELP_WIDTH, 0, 0);
	up = new_panel (help);
	update_panels ();
	doupdate ();

	box (help, 0, 0);
	wbkgd (help, COLOR_PAIR (WBl));
	wrefresh (help);
	mvwaddstr (help, 0, HELP_WIDTH/2 - 3, " HELP ");
	wattron (help, A_BOLD | A_UNDERLINE);
	// subtitles
	mvwaddstr (help, 1, 1, "Nmos commands");
	mvwaddstr (help, 6, 1, "Mosaic editing");
	wattroff (help, A_UNDERLINE);
	// hotkeys
	mvwaddstr (help, 2, 1, "F1:");
	mvwaddstr (help, 3, 1, "Esc:");
	mvwaddstr (help, 4, 1, "^Q:");
	
	mvwaddstr (help, 7, 1, "Arrow Keys:");
	mvwaddstr (help, 8, 1, "Shifted Arrow Keys:");
	mvwaddstr (help, 9, 1, "^S:");
	mvwaddstr (help, 10, 1, "^O:");
	mvwaddstr (help, 11, 1, "^N:");
	
	mvwaddstr (help, 12, 1, "^C:");
	mvwaddstr (help, 13, 1, "^V:");
	
	


	wattrset (help, COLOR_PAIR (WBl));
	// what the hotkeys do
	mvwaddstr (help, 2, 5, "show this help window");
	mvwaddstr (help, 3, 6, "show the menu");
	mvwaddstr (help, 4, 5, "quit Nmos");
	
	mvwaddstr (help, 7, 13, "move through the mosaic");
	mvwaddstr (help, 8, 21, "change the moving direction after input");
	mvwaddstr (help, 9, 5, "save mosaic");
	mvwaddstr (help, 10, 5, "load mosaic");
	mvwaddstr (help, 11, 5, "new mosaic");
	mvwaddstr (help, 12, 5, "copy selection");
	mvwaddstr (help, 13, 5, "paste selection");
	
	// HUD explanation
	mvwaddch (help, HELP_HEIGHT - 1, 0, ACS_ULCORNER);
	waddch (help, ACS_HLINE); waddch (help, ACS_HLINE); waddch (help, ACS_HLINE);
	waddstr (help, " Nmos basic hotkeys ");
	waddch (help, ACS_HLINE); waddch (help, ACS_HLINE); waddch (help, ACS_HLINE); waddch (help, ACS_HLINE); waddch (help, ACS_URCORNER);
	wclrtoeol (help);
	mvwaddch (help, HELP_HEIGHT - 1, HELP_WIDTH - 11, ACS_ULCORNER);
	waddstr (help, "position");
	waddch (help, ACS_URCORNER); waddch (help, ACS_VLINE);
	mvwaddstr (help, HELP_HEIGHT - 2, HELP_WIDTH - 18, "default direction");
	waddch (help, ACS_URCORNER);

// writes the help window, wait for some key to be pressed and delete the help window
	wrefresh (help);
	getch ();

	werase (help);
	wrefresh (help);
	del_panel (up);
	delwin (help);
	
	curs_set (1);
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
