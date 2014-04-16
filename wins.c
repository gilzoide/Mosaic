#include "wins.h"


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
		"F2", "^S", "^O", "^R", "^C/^X", "^V", "Tab"
	};
	// and how many are there for each subtitle
	int n_hotkeys[] = {3, 5, 7};
	// what the hotkeys do
	char *explanations[] = {
		"show this help window", "show the menu", "quit Nmos",
		"move through the mosaic", "change the moving direction after input (default direction)", "toggle box selection mode", "previous/next mosaic", "move to first/last character (in the default direction)",
		"new mosaic", "save mosaic", "load mosaic", "resize mosaic", "copy/cut selection", "paste selection", "show the attribute table"
	};
	
	// aux counters; only 'i' gets reseted at 0, as it counts until n_hotkeys ends
	int sub, hot, i, line = 1;
	// for each subtitle
	for (sub = hot = 0; sub < 3; sub++, line++) {
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
	
	MENU *menu;
	WINDOW *win;	// window for the menu
	PANEL *up;		// the panel for the menu's window
	ITEM **items;
	const int num_items = 1;
	const char *choices[] = {
		"Help"
	};

	// create the items
	items = (ITEM**) calloc (num_items, sizeof (ITEM*));
	int i;
	for (i = 0; i < num_items; i++) {
		items[i] = new_item (choices[i], choices[i]);
	}
	
	// create the menu's window and panel
	win = newwin (HELP_HEIGHT, HELP_WIDTH, 0, 0);
	up = new_panel (win);
	update_panels ();
	doupdate ();

	box (win, 0, 0);
	wbkgd (win, COLOR_PAIR (WG));
	
	// create the menu
	menu = new_menu (items);
	set_menu_win (menu, win);
	
	// display the menu
	post_menu (menu);
	wrefresh (win);
	
	getch ();

	free_menu (menu);
	for (i = 0; i < num_items; i++)
		free_item (items[i]);
	free (items);
	
	werase (win);
	wrefresh (win);
	del_panel (up);
	delwin (win);

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
	
	return 0;
}
