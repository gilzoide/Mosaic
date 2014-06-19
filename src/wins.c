#include "wins.h"

// Global WINDOWS, PANELS, MENU, etc
MENU *menu;
WINDOW *menuWindow;
PANEL *menuPanel;

MENU *file_menu;
WINDOW *file_menuWindow;

MENU *edit_menu;
WINDOW *edit_menuWindow;

MENU *image_menu;
WINDOW *image_menuWindow;

MENU *help_menu;
WINDOW *help_menuWindow;
// the PANEL for the submenus (we just change the WINDOW inside to display the different menus)
PANEL *submenuPanel;

WINDOW *helpWindow;
PANEL *helpPanel;

WINDOW *hud;


void InitHud () {
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
}


void InitHelp () {
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
	waddch (helpWindow, ACS_HLINE);
	waddch (helpWindow, ACS_HLINE);
	waddstr (helpWindow, " Nmos basic hotkeys ");
	waddch (helpWindow, ACS_HLINE);
	waddch (helpWindow, ACS_HLINE);
	waddch (helpWindow, ACS_HLINE);
	waddch (helpWindow, ACS_URCORNER);

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
}


void InitMenus () {
// MENU'S MENU
	menuWindow = newwin (1, LINES, LINES - 2, 0);
	WINDOW *menuSub = derwin (menuWindow, 1, LINES, 0, 0);
	
	menuPanel = new_panel (menuWindow);
	hide_panel (menuPanel);
	
	const int menu_numItems = 4;
	const char *menu_titles[] = {
		"1)",
		"2)",
		"3)",
		"4)"
	};
	const char *menu_descriptions[] = {
		"File",
		"Edit",
		"Image",
		"Help"
	};
	
	ITEM **items;
	items = (ITEM**) calloc (menu_numItems + 1, sizeof (ITEM*));
	int i;
	for (i = 0; i < menu_numItems; i++) {
		items[i] = new_item (menu_titles[i], menu_descriptions[i]);
	}
	
	menu = new_menu (items);
	menu_opts_off (menu, O_NONCYCLIC);
	set_menu_format (menu, 1, menu_numItems);
	
	set_menu_mark (menu, "");
	set_menu_fore (menu, A_REVERSE);
	set_menu_win (menu, menuWindow);
	set_menu_sub (menu, menuSub);
	
	post_menu (menu);

// FILE MENU
	int x_aux = 0;
	file_menuWindow = CreateBoxedTitledWindow (MENU_HEIGHT, MENU_WIDTH, LINES - MENU_HEIGHT - 2, x_aux, "FILE");
	
	int num_items = 4;
	const char *file_titles[] = {
		"New Image",
		"Save Image",
		"Load Image",
		"Exit Mosaic"
	};
	const char *file_descriptions[] = {
		"F2",
		"^S",
		"^O",
		"^Q"
	};
	// The choices are static so the userptr points to something that exists
	static const int file_choices[] = {		
		KEY_F(2),
		KEY_CTRL_S,
		KEY_CTRL_O,
		KEY_CTRL_Q
	};
	// create the items
	items = (ITEM**) calloc (num_items + 1, sizeof (ITEM*));
	for (i = 0; i < num_items; i++) {
		items[i] = new_item (file_titles[i], file_descriptions[i]);
		set_item_userptr (items[i], (void*) &file_choices[i]);
	}
	
	// create the menu
	file_menu = new_menu (items);
	// options: cycle, mark...
	menu_opts_off (file_menu, O_NONCYCLIC);
	set_menu_mark (file_menu, "");

	// menu's windows
	set_menu_win (file_menu, file_menuWindow);
	set_menu_sub (file_menu, derwin (file_menuWindow, MENU_HEIGHT - 2, MENU_WIDTH - 2, 1, 1));
	
	post_menu (file_menu);

// EDIT MENU
	x_aux = MENU_X_SEPARATOR / 2;
	edit_menuWindow = CreateBoxedTitledWindow (MENU_HEIGHT, MENU_WIDTH, LINES - MENU_HEIGHT - 2, x_aux, "EDIT");
	
	num_items = 4;
	const char *edit_titles[] = {
		"Cut",
		"Copy",
		"Paste",
		"Selection mode"
	};
	const char *edit_descriptions[] = {
		"^X",
		"^C",
		"^V",
		"^B"
	};
	// The choices are static so that the userptr points to something that exists
	static const int edit_choices[] = {
		KEY_CTRL_X,
		KEY_CTRL_C,
		KEY_CTRL_V,
		KEY_CTRL_B
	};
	// create the items
	items = (ITEM**) calloc (num_items + 1, sizeof (ITEM*));
	for (i = 0; i < num_items; i++) {
		items[i] = new_item (edit_titles[i], edit_descriptions[i]);
		set_item_userptr (items[i], (void*) &edit_choices[i]);
	}
	
	// create the menu
	edit_menu = new_menu (items);
	// options: cycle, mark...
	menu_opts_off (edit_menu, O_NONCYCLIC);
	set_menu_mark (edit_menu, "");

	// menu's windows
	set_menu_win (edit_menu, edit_menuWindow);
	set_menu_sub (edit_menu, derwin (edit_menuWindow, MENU_HEIGHT - 2, MENU_WIDTH - 2, 1, 1));
	
	post_menu (edit_menu);
	
// IMAGE MENU
	x_aux += MENU_X_SEPARATOR;
	image_menuWindow = CreateBoxedTitledWindow (MENU_HEIGHT, MENU_WIDTH, LINES - MENU_HEIGHT - 2, x_aux, "IMAGE");
	
	num_items = 4;
	const char *image_titles[] = {
		"New Image",
		"Save Image",
		"Load Image",
		"Resize Image"
	};
	const char *image_descriptions[] = {
		"^N",
		"^S",
		"^O",
		"^R"
	};
	// The choices are static so that the userptr points to something that exists
	static const int image_choices[] = {
		KEY_F(2),
		KEY_CTRL_S,
		KEY_CTRL_O,
		KEY_CTRL_R
	};
	// create the items
	items = (ITEM**) calloc (num_items + 1, sizeof (ITEM*));
	for (i = 0; i < num_items; i++) {
		items[i] = new_item (image_titles[i], image_descriptions[i]);
		set_item_userptr (items[i], (void*) &image_choices[i]);
	}
	
	// create the menu
	image_menu = new_menu (items);
	// options: cycle, mark...
	menu_opts_off (image_menu, O_NONCYCLIC);
	set_menu_mark (image_menu, "");

	// menu's windows
	set_menu_win (image_menu, image_menuWindow);
	set_menu_sub (image_menu, derwin (image_menuWindow, MENU_HEIGHT - 2, MENU_WIDTH - 2, 1, 1));
	
	post_menu (image_menu);

// HELP MENU
	x_aux += MENU_X_SEPARATOR;
	help_menuWindow = CreateBoxedTitledWindow (MENU_HEIGHT, MENU_WIDTH, LINES - MENU_HEIGHT - 2, x_aux, "HELP");
	
	num_items = 2;
	const char *help_titles[] = {
		"Show help",
		"About"
	};
	const char *help_descriptions[] = {
		"F1",
		" ",
	};
	// The choices are static so that the userptr points to something that exists
	static const int help_choices[] = {
		KEY_F(1),
		0
	};
	// create the items
	items = (ITEM**) calloc (num_items + 1, sizeof (ITEM*));
	for (i = 0; i < num_items; i++) {
		items[i] = new_item (help_titles[i], help_descriptions[i]);
		set_item_userptr (items[i], (void*) &help_choices[i]);
	}
	
	// create the menu
	help_menu = new_menu (items);
	// options: cycle, mark...
	menu_opts_off (help_menu, O_NONCYCLIC);
	set_menu_mark (help_menu, "");

	// menu's windows
	set_menu_win (help_menu, help_menuWindow);
	set_menu_sub (help_menu, derwin (help_menuWindow, MENU_HEIGHT - 2, MENU_WIDTH - 2, 1, 1));
	
	post_menu (help_menu);

// allocate the submenus in the menu
	set_item_userptr (menu_items (menu)[0], file_menu);
	set_item_userptr (menu_items (menu)[1], edit_menu);
	set_item_userptr (menu_items (menu)[2], image_menu);
	set_item_userptr (menu_items (menu)[3], help_menu);
	submenuPanel = new_panel (file_menuWindow);
}


void InitWins () {
// HUD
	InitHud ();
// HELP
	InitHelp ();
// MENU
	InitMenus ();
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
	// show in the Hud the menu is opened (eye-candy =P)
	mvwchgat (hud, 0, 9, 4, A_UNDERLINE | A_BOLD, CN, NULL);
	mvwchgat (hud, 0, 13, 5, A_UNDERLINE, CN, NULL);
	wrefresh (hud);
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
	
	// hud goes back to normal
	mvwchgat (hud, 0, 9, 4, A_BOLD, 0, NULL);
	mvwchgat (hud, 0, 13, 5, A_NORMAL, 0, NULL);
	wrefresh (hud);

	return c;
}


int GetChosenOption (MENU *menu) {
	int c;
	
	// The submenu, vertically shown
	MENU *submenu;

	// drives through the menu options
	while (c != ' ') {
		// update what submenu we're watching, 
		// switching to the new submenu's WINDOW
		submenu = (MENU*) item_userptr (current_item (menu));
		replace_panel (submenuPanel, menu_win (submenu));
		// refreshes the submenu's window
		update_panels ();
		doupdate ();

		c = getch ();
		switch (c) {
			// Return: accepts choice, just as the ' ' does
			case '\n':
				c = ' ';
			case ' ':
				break;

			// choose between submenus (like a case match)
			case '1': case '2': case '3': case '4':
				/* 
				 * note that 'c' being an integer, the first byte is the
				 * char representation of the number and the second byte
				 * is a 0, so it's a valid '\0' terminated string xD 
				 */
				set_menu_pattern (menu, (char*) &c);
				break;

			// drive the menu's menu
			case KEY_LEFT:
				menu_driver (menu, REQ_LEFT_ITEM);
				break;

			case KEY_RIGHT:
				menu_driver (menu, REQ_RIGHT_ITEM);
				break;
				
			// Esc or F(10): exit menu
			case KEY_ESC: case KEY_F(10): 
				return 0;

			// drive the submenu
			case KEY_DOWN:
				menu_driver (submenu, REQ_DOWN_ITEM);
				break;

			case KEY_UP:
				menu_driver (submenu, REQ_UP_ITEM);
				break;
				
			case KEY_HOME:
				menu_driver (submenu, REQ_FIRST_ITEM);
				break;
				
			case KEY_END:
				menu_driver (submenu, REQ_LAST_ITEM);
				break;

			// Submenu case match: if it fails, exit menu
			default:
				if (menu_driver (submenu, c) == E_NO_MATCH) {
					// reset submenu pattern matching, for the next time
					set_menu_pattern (submenu, "");
					return 0;
				}
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


void DeleteMenu (MENU *menu) {
	if (menu != NULL) {
		unpost_menu (menu);
		free_menu (menu);

		// Destroy all the items!
		int i;
		ITEM **items = menu_items (menu);
		for (i = 0; i < item_count (menu); i++)
			free_item (items[i]);
		free (items);
		
		DeleteWindow (menu_win (menu));
		DeleteWindow (menu_sub (menu));
	}
}


void DestroyWins () {
// Help
	DeletePanel (helpPanel);
// Hud
	DeleteWindow (hud);
// Menus
	DeleteMenu (menu);
	DeleteMenu (file_menu);
	DeleteMenu (edit_menu);
	DeleteMenu (image_menu);
	DeleteMenu (help_menu);
	del_panel (menuPanel);
	del_panel (submenuPanel);
}
