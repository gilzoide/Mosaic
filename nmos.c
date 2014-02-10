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
	cur->x = cur->y = cur->origin_x = cur->origin_y = 0;
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
	
	// the subtitles
	char *subtitles[] = {
		"Nmos commands (basic hotkeys)",
		"Navigation",
		"Mosaic editing"
	};
	// the hotkeys
	char *hotkeys[] = {
		"F1", "Esc", "^Q",
		"Arrow Keys", "^D", "^B", "Page Up/Page Down", "Home/End",
		"F2", "^S", "^O", "^C/^X", "^V", "Tab"
	};
	// and how many are there for each subtitle
	int n_hotkeys[] = {3, 5, 6};
	// what the hotkeys do
	char *explanations[] = {
		"show this help window", "show the menu", "quit Nmos",
		"move through the mosaic", "change the moving direction after input (default direction)", "toggle box/selection mode", "previous/next mosaic", "move to first/last character (in the default direction)",
		"new mosaic", "save mosaic", "load mosaic", "copy/cut selection", "paste selection", "show the attribute table"
	};
	
	// aux counters; only 'i' gets reseted at 0, as it counts until n_hotkeys ends
	int sub, hot, i, line = 1;
	// for each subtitle
	for (sub = hot = 0; sub < 3; sub++, line++) {	// ATENTION! ignore warning (aggressive-loop-optimizations)
		// write subtitle
		wattron (help, A_BOLD | A_UNDERLINE);
		mvwaddstr (help, line, 1, subtitles[sub]);
		wattroff (help, A_UNDERLINE);
		line++;
		// write rest =P
		for (i = 0; i < n_hotkeys[sub]; i++, hot++) {
			mvwaddstr (help, line++, 1, hotkeys[hot]);
			waddstr (help, ": ");
			wattroff (help, A_BOLD);
			waddstr (help, explanations[hot]);
			wattron (help, A_BOLD);
		}
	}
	
	


	wstandend (help);	
	// HUD explanation
	mvwaddch (help, HELP_HEIGHT - 1, 0, ACS_ULCORNER);
	waddch (help, ACS_HLINE); waddch (help, ACS_HLINE);
	waddstr (help, " Nmos basic hotkeys ");
	waddch (help, ACS_HLINE); waddch (help, ACS_HLINE); waddch (help, ACS_HLINE); waddch (help, ACS_URCORNER);
	wclrtoeol (help);
	
	int aux = COLS - 44;
	mvwaddch (help, HELP_HEIGHT - 1, 29, ACS_ULCORNER);
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


MOSIMG *CreateNewImg (IMGS *everyone, MOSIMG *current) {
	int new_height = INITIAL_HEIGHT, new_width = INITIAL_WIDTH;
	everyone->size++;
	
	MOSIMG *new_image = NewImg (new_height, new_width);
	
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


void DestroyIMGS (IMGS *everyone) {
	MOSIMG *aux, *next;
	int i;
	
	for (aux = everyone->list, i = 0; i < everyone->size; i++, aux = next) {
		next = aux->next;
		FreeImg (aux);
	}
}
