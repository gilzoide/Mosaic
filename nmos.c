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


void InitCursor (Cursor *cur) {
	cur->x = cur->y = cur->bot_x = cur->bot_y = 0;
}


WINDOW *CreateHud () {
	WINDOW *win = subwin (stdscr, 1, COLS, LINES - 1, 0);

	wattron (win, A_BOLD);
	waddstr (win, "F1: ");
	wattroff (win, A_BOLD);
	waddstr (win, "Help ");
	wattron (win, A_BOLD);
	waddstr (win, "Esc: ");
	wattroff (win, A_BOLD);
	waddstr (win, "Menu ");
	wattron (win, A_BOLD);
	waddstr (win, "^Q: ");
	wattroff (win, A_BOLD);
	waddstr (win, "Quit");
	
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


int PrintHud (WINDOW *hud, const char *message) {
	mvwaddch (hud, 0, 29, ACS_DIAMOND);
	waddstr (hud, message);
	wrefresh (hud);
	int c = getch ();
	wmove (hud, 0, 29);
	wclrtoeol (hud);
	wrefresh (hud);
	
	return c;
}


/* Displays the help (in a created window and panel, for going back to the normal field after) */
void Help () {
	curs_set (0);	// don't display the cursor, pliz
	
	WINDOW *help;
	PANEL *up;

	help = newwin (HELP_HEIGHT, HELP_WIDTH, 0, 0);
	up = new_panel (help);
	update_panels ();
	doupdate ();

	box (help, 0, 0);
	wbkgd (help, COLOR_PAIR (WBl));
	
	mvwaddstr (help, 0, HELP_WIDTH/2 - 3, " HELP ");
	wattron (help, A_BOLD | A_UNDERLINE);
	// subtitles
	mvwaddstr (help, 1, 1, "Nmos commands (basic hotkeys)");
	mvwaddstr (help, 6, 1, "Navigation");
	mvwaddstr (help, 13, 1, "Mosaic editing");
	wattroff (help, A_UNDERLINE);
	// hotkeys
	mvwaddstr (help, 2, 1, "F1:");
	mvwaddstr (help, 3, 1, "Esc:");
	mvwaddstr (help, 4, 1, "^Q:");
	
	mvwaddstr (help, 7, 1, "Arrow Keys:");
	mvwaddstr (help, 8, 1, "^D:");
	mvwaddstr (help, 9, 1, "^V:");
	mvwaddstr (help, 10, 1, "PgUp:");
	mvwaddstr (help, 11, 1, "PgDn:");
	
	mvwaddstr (help, 14, 1, "F2:");
	mvwaddstr (help, 15, 1, "^S:");
	mvwaddstr (help, 16, 1, "^O:");
	mvwaddstr (help, 17, 1, "^C:");
	mvwaddstr (help, 18, 1, "^V:");
	mvwaddstr (help, 19, 1, "^X:");
	mvwaddstr (help, 20, 1, "Tab:");


	wstandend (help);
	// what the hotkeys do
	mvwaddstr (help, 2, 5, "show this help window");
	mvwaddstr (help, 3, 6, "show the menu");
	mvwaddstr (help, 4, 5, "quit Nmos");
	
	mvwaddstr (help, 7, 13, "move through the mosaic");
	mvwaddstr (help, 8, 5, "change the moving direction after input");
	mvwaddstr (help, 9, 5, "visual/selection mode; press enter to exit");
	mvwaddstr (help, 10, 7, "next mosaic");
	mvwaddstr (help, 11, 7, "previous mosaic");
	
	mvwaddstr (help, 14, 5, "new mosaic");
	mvwaddstr (help, 15, 5, "save mosaic");
	mvwaddstr (help, 16, 5, "load mosaic");
	mvwaddstr (help, 17, 5, "copy selection");
	mvwaddstr (help, 18, 5, "paste selection");
	mvwaddstr (help, 19, 5, "cut selection");
	mvwaddstr (help, 20, 6, "show color/attribute table");
	
	// HUD explanation
	mvwaddch (help, HELP_HEIGHT - 1, 0, ACS_ULCORNER);
	waddch (help, ACS_HLINE); waddch (help, ACS_HLINE);
	waddstr (help, " Nmos basic hotkeys ");
	waddch (help, ACS_HLINE); waddch (help, ACS_HLINE); waddch (help, ACS_HLINE); waddch (help, ACS_URCORNER);
	wclrtoeol (help);
	
	int aux = COLS - 44;
	mvwaddch (help, HELP_HEIGHT - 1, 29, ACS_ULCORNER);
	int i;
	for (i = 0; i < aux; i++) {
		wechochar (help, ACS_HLINE);
	}
	waddch (help, ACS_URCORNER);
	mvwaddstr (help, HELP_HEIGHT - 1, 22 + (aux / 2), " message area ");
	
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
	
	curs_set (1);	// and back with the cursor
}


int Menu () {
	int c = 0;
	
	WINDOW *menu;
	PANEL *up;

	menu = newwin (12, HELP_WIDTH, 0, 0);
	up = new_panel (menu);
	update_panels ();
	doupdate ();
	
	//getch ();

	werase (menu);
	wrefresh (menu);
	del_panel (up);
	delwin (menu);
	
	return c;
}


int AttrTable (MOSIMG *current, Cursor cur) {
	WINDOW *table;
	PANEL *up;

	table = newwin (12, HELP_WIDTH, 0, 0);
	up = new_panel (table);
	update_panels ();
	doupdate ();
	
	//box (table, 0, 0);
	wbkgd (table, COLOR_PAIR (BkW));
	wrefresh (table);
	
	getch ();

	werase (table);
	wrefresh (table);
	del_panel (up);
	delwin (table);
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
	// upper-left corner and bottom-right corner are one now
	position->bot_y = position->y;
	position->bot_x = position->x;
	// and move!
	move (position->y, position->x);
}


void ChangeDefaultDirection (WINDOW *hud, Direction *dir) {
	int c = PrintHud (hud, "new default direction (arrow keys)");
	
	switch (c) {
		case KEY_UP:	*dir = UP;	break;
		case KEY_DOWN:	*dir = DOWN;	break;
		case KEY_LEFT:	*dir = LEFT;	break;
		case KEY_RIGHT:	*dir = RIGHT;	break;
	}
}


void InitIMGS (IMGS *everyone) {
	everyone->list = NULL;
	everyone->size = 0;
}


MOSIMG *CreateNewImg (IMGS *everyone) {
	int new_height = 15, new_width = 10;
	everyone->size++;
	
	// first image: no one's after or before
	if (everyone->list == NULL) {
		everyone->list = NewImg (new_height, new_width);
		everyone->list->prev = everyone->list->next = everyone->list;	// yep, a circular list with only one item
		return everyone->list;
	}
	// not the first, so put it in the end
	else {
		MOSIMG *last = everyone->list->prev;
		MOSIMG *new_image = NewImg (new_height, new_width);
		
		everyone->list->prev = last->next = new_image;
		new_image->next = everyone->list;
		new_image->prev = last;
		return new_image;
	}
}


void DisplayCurrentPanel (MOSIMG *current) {
	top_panel (current->pan);
	update_panels ();
	doupdate ();
}


void DestroyIMGS (IMGS *everyone) {
	MOSIMG *aux, *next;
	int i;
	
	for (aux = everyone->list, i = 0; i < everyone->size; i++, aux = next) {
		next = aux->next;
		FreeImg (aux);
	}
}
