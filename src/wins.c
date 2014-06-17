#include "wins.h"

// Global WINDOWS, PANELS, MENU, etc
MENU *menu = NULL;
WINDOW *menuWindow = NULL;
PANEL *menuPanel = NULL;

MENU *file_menu = NULL;
WINDOW *file_menuWindow = NULL;
// the PANEL for the submenus (we just change the WINDOW inside to display the different menus)
PANEL *submenuPanel = NULL;

WINDOW *helpWindow = NULL;
PANEL *helpPanel = NULL;

WINDOW *hud = NULL;

void InitWins () {
// HUD
	hud = subwin (stdscr, 1, COLS, LINES - 1, 0);

	wattron (hud, A_BOLD);
	waddstr (hud, "F1: ");
	wattroff (hud, A_BOLD);
	waddstr (hud, "Help ");
	wattron (hud, A_BOLD);
	waddstr (hud, "F10: ");
	wattroff (hud, A_BOLD);
	waddstr (hud, "Menu ");
	wattron (hud, A_BOLD);
	waddstr (hud, "^Q: ");
	wattroff (hud, A_BOLD);
	waddstr (hud, "Quit");
	
	wrefresh (hud);
// END HUD
// HELP
	helpWindow = CreateBoxedTitledWindow (HELP_HEIGHT, HELP_WIDTH, 0, 0, "HELP");
	helpPanel = new_panel (helpWindow);
	hide_panel (helpPanel);

	wbkgd (helpWindow, COLOR_PAIR (WBl));

	// the subtitles
	const char *subtitles[] = {
		"Nmos commands (basic hotkeys)",
		"Navigation",
		"Mosaic editing"
	};
	// the hotkeys
	const char *hotkeys[] = {
		"F1", "F10", "^Q",
		"Arrow Keys", "^D", "^B", "Page Up/Page Down", "Home/End",
		"F2", "^S", "^O", "^R", "^C/^X", "^V", "Tab"
	};
	// and how many are there for each subtitle
	int n_hotkeys[] = {3, 5, 7};
	// what the hotkeys do
	const char *explanations[] = {
		"show this help", "show the menu", "quit Nmos",
		"move through the mosaic", "change the moving direction after input (default direction)", "toggle box selection mode", "previous/next mosaic", "move to first/last character (in the default direction)",
		"new mosaic", "save mosaic", "load mosaic", "resize mosaic", "copy/cut selection", "paste selection", "show the attribute table"
	};
	
	// aux counters; only 'i' gets reseted at 0, as it counts until n_hotkeys ends
	int sub, hot, i, line = 1;
	// for each subtitle
	for (sub = hot = 0; sub < 3; sub++, line++) {
		// write subtitle
		wattron (helpWindow, A_BOLD | A_UNDERLINE);
		mvwaddstr (helpWindow, line, 1, subtitles[sub]);
		wattroff (helpWindow, A_UNDERLINE);
		line++;
		// write rest =P
		for (i = 0; i < n_hotkeys[sub]; i++, hot++) {
			mvwaddstr (helpWindow, line++, 1, hotkeys[hot]);
			waddstr (helpWindow, ": ");
			wattroff (helpWindow, A_BOLD);
			waddstr (helpWindow, explanations[hot]);
			wattron (helpWindow, A_BOLD);
		}
	}

	wstandend (helpWindow);	
	// HUD explanation
	mvwaddch (helpWindow, HELP_HEIGHT - 1, 0, ACS_ULCORNER);
	waddch (helpWindow, ACS_HLINE); waddch (helpWindow, ACS_HLINE);
	waddstr (helpWindow, " Nmos basic hotkeys ");
	waddch (helpWindow, ACS_HLINE); waddch (helpWindow, ACS_HLINE); waddch (helpWindow, ACS_HLINE); waddch (helpWindow, ACS_URCORNER);
	wclrtoeol (helpWindow);
	
	int aux = COLS - 44;
	mvwaddch (helpWindow, HELP_HEIGHT - 1, 29, ACS_ULCORNER);
	for (i = 0; i < aux; i++) {
		wechochar (helpWindow, ACS_HLINE);
	}
	waddch (helpWindow, ACS_URCORNER);
	mvwaddstr (helpWindow, HELP_HEIGHT - 1, 22 + (aux / 2), " message area ");
	
	mvwaddch (helpWindow, HELP_HEIGHT - 1, HELP_WIDTH - 11, ACS_ULCORNER);
	waddstr (helpWindow, "position");
	waddch (helpWindow, ACS_URCORNER); waddch (helpWindow, ACS_VLINE);
	mvwaddstr (helpWindow, HELP_HEIGHT - 2, HELP_WIDTH - 18, "default direction");
	waddch (helpWindow, ACS_URCORNER);

	touchwin (helpWindow);
// END HELP

// MENU
	// Menu's menu
	menuWindow = newwin (1, LINES, LINES - 2, 0);
	WINDOW *menuSub = derwin (menuWindow, 1, 25, 0, 0);
	
	menuPanel = new_panel (menuWindow);
	hide_panel (menuPanel);
	
	const int menu_numItems = 4;
	const char *menu_titles[] = {
		"File",
		"Edit",
		"Image",
		"Help"
	};
	
	ITEM **items;
	items = (ITEM**) calloc (menu_numItems + 1, sizeof (ITEM*));
	for (i = 0; i < menu_numItems; i++) {
		items[i] = new_item (menu_titles[i], menu_titles[i]);
	}
	
	menu = new_menu (items);
	menu_opts_off (menu, O_NONCYCLIC | O_SHOWDESC);
	set_menu_format (menu, 1, menu_numItems);
	
	set_menu_mark (menu, "");
	set_menu_fore (menu, A_BOLD | A_UNDERLINE);
	set_menu_win (menu, menuWindow);
	set_menu_sub (menu, menuSub);
	
	post_menu (menu);
	// END Menu's menu
	
	file_menuWindow = CreateBoxedTitledWindow (MENU_HEIGHT, MENU_WIDTH, LINES - MENU_HEIGHT - 2, 0, "MENU");		// the panel for the menu's window
	
	
	const int num_items = 6;
	const char *choice_titles[] = {	// 
		"Help",
		"New Image",
		"Save Image",
		"Load Image",
		"Resize Image",
		"Exit Mosaic"
	};
	const char *choice_description[] = {
		"F1",
		"F2",
		"^S",
		"^O",
		"^R",
		"^Q"
	};
	static const int choices[] = {
		KEY_F(1),
		KEY_F(2),
		KEY_CTRL_S,
		KEY_CTRL_O,
		KEY_CTRL_R,
		KEY_CTRL_Q
	};
	// create the items
	items = (ITEM**) calloc (num_items + 1, sizeof (ITEM*));
	for (i = 0; i < num_items; i++) {
		items[i] = new_item (choice_titles[i], choice_description[i]);
		set_item_userptr (items[i], (void*) &choices[i]);
	}
	
	// create the menu
	file_menu = new_menu (items);
	// options: cycle and don't show description, mark...
	menu_opts_off (file_menu, O_NONCYCLIC);
	set_menu_mark (file_menu, "");

	// menu's windows
	set_menu_win (file_menu, file_menuWindow);
	set_menu_sub (file_menu, derwin (file_menuWindow, MENU_HEIGHT - 2, MENU_WIDTH - 2, 1, 1));
	
	// file_menu is the first menu
	post_menu (file_menu);
	set_item_userptr (menu_items (menu)[0], file_menu);
	set_item_userptr (menu_items (menu)[1], file_menu);
	set_item_userptr (menu_items (menu)[2], file_menu);
	set_item_userptr (menu_items (menu)[3], file_menu);
	submenuPanel = new_panel (file_menuWindow);
// END MENU
}


void UpdateHud (Cursor cur, Direction dir) {
	int arrow;
	// arrows, to show the direction!
	switch (dir) {
		case UP: 	arrow = ACS_UARROW; break;
		case DOWN: 	arrow = ACS_DARROW; break;
		case LEFT:	arrow = ACS_LARROW; break;
		case RIGHT:	arrow = ACS_RARROW; break;
	}

	// update coordinates
	mvwprintw (hud, 0, COLS - 11, "%dx%d", cur.y, cur.x);
	wclrtoeol (hud);
	mvwaddch (hud, 0, COLS - 1, arrow);
	wrefresh (hud);
	move (cur.y, cur.x);
}


int PrintHud (const char *message) {
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

	// displays the help
	show_panel (helpPanel);
	update_panels ();
	doupdate ();
	// waits for some key to be pressed
	getch ();
	
	// hide the help
	hide_panel (helpPanel);
	doupdate ();
	
	curs_set (1);	// and back with the cursor
}


int Menu () {
	// display the menu
	show_panel (menuPanel);
	show_panel (submenuPanel);
	update_panels ();
	doupdate ();
	
	// get the chosen option
	int c = GetChosenOption (menu);

	// hide the menu
	hide_panel (menuPanel);
	hide_panel (submenuPanel);
	doupdate ();

	return c;
}


int GetChosenOption (MENU *menu) {
	int c;
	
	// starts at the first item
	menu_driver (menu, REQ_FIRST_ITEM);
	// The submenu, vertically shown
	MENU *submenu;

	// drives through the menu options
	while (c != ' ') {
		// update what submenu we're watching, hiding the previous menu and showing the next
		submenu = (MENU*) item_userptr (current_item (menu));
		replace_panel (submenuPanel, menu_win (submenu));
		// refreshes the submenu's window
		update_panels ();
		doupdate ();

		c = getch ();
		switch (c) {
			case '\n':		// Return: accepts choice, just as the ' ' does
				c = ' ';
			case ' ':
				break;

			case KEY_F(10): case KEY_ESC:	// Esc or F(x): exit menu
				return 0;

			case KEY_DOWN:
				menu_driver (submenu, REQ_DOWN_ITEM);
				break;

			case KEY_UP:
				menu_driver (submenu, REQ_UP_ITEM);
				break;
				
			case KEY_LEFT:
				menu_driver (menu, REQ_LEFT_ITEM);
				break;

			case KEY_RIGHT:
				menu_driver (menu, REQ_RIGHT_ITEM);
				break;
				
			case KEY_HOME:
				menu_driver (menu, REQ_FIRST_ITEM);
				break;
				
			case KEY_END:
				menu_driver (menu, REQ_LAST_ITEM);
				break;

			default:		// Case match: if it fails, exit menu
				if (menu_driver (menu, c) == E_NO_MATCH)
					return 0;
		}
	}
	
	return *(int*) (item_userptr (current_item (submenu)));
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


WINDOW *CreateBoxedTitledWindow (int nlines, int ncols, int begin_y, int begin_x, const char *title) {
	WINDOW *win = newwin (nlines, ncols, begin_y, begin_x);

	box (win, 0, 0);
	
	int length = strlen (title);
	int title_begin = (ncols - length)/2;
	mvwaddstr (win, 0, title_begin, title);
	mvwaddch (win, 0, title_begin - 1, ' ');
	mvwaddch (win, 0, title_begin + length, ' ');
	
	return win;
}


void DeletePanel (PANEL *pan) {
	if (pan != NULL) {
		DeleteWindow (panel_window (pan));
		
		del_panel (pan);
	}
}


void DeleteWindow (WINDOW *win) {
	if (win != NULL) {
		werase (win);
		wrefresh (win);
		delwin (win);
	}
}


void DestroyWins () {
// HELP
	DeletePanel (helpPanel);
	
// MENU
	if (menu != NULL) {
		unpost_menu (menu);
		free_menu (menu);

		int i;
		ITEM **items = menu_items (menu);
		for (i = 0; i < item_count (menu); i++)
			free_item (items[i]);
		free (items);
		
		DeletePanel (menuPanel);
	}
// FILE_MENU
	if (file_menu != NULL) {
		unpost_menu (file_menu);
		free_menu (file_menu);

		int i;
		ITEM **items = menu_items (file_menu);
		for (i = 0; i < item_count (file_menu); i++)
			free_item (items[i]);
		free (items);
		
		DeleteWindow (file_menuWindow);
	}
}
