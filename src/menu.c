// The menus we'll need (and it's WINDOWs)
// Main menu (menus' menu)
MENU *menu;
WINDOW *menuWindow;
PANEL *menuPanel;
// Submenus
MENU *file_menu;
WINDOW *file_menuWindow;

MENU *edit_menu;
WINDOW *edit_menuWindow;

MENU *image_menu;
WINDOW *image_menuWindow;

MENU *help_menu;
WINDOW *help_menuWindow;
// The PANEL for the submenus (we just change the
// WINDOW inside to display the different menus)
PANEL *submenuPanel;


void InitMenus () {
// MENU'S MENU
	menuWindow = newwin (1, 0, LINES - 2, 0);
	WINDOW *menuSub = derwin (menuWindow, 1, 0, 0, 0);
	
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
		"Save File",
		"Load File",
		"Jump to Image",
		"Exit Maae"
	};
	const char *file_descriptions[] = {
		"^S",
		"^O",
		"^G",
		"^Q"
	};
	// The choices are static so the userptr points to something that exists
	static const int file_choices[] = {		
		KEY_CTRL_S,
		KEY_CTRL_O,
		KEY_CTRL_G,
		KEY_CTRL_Q
	};
	// create the items
	items = (ITEM **) malloc ((num_items + 1) * sizeof (ITEM *));
	for (i = 0; i < num_items; i++) {
		items[i] = new_item (file_titles[i], file_descriptions[i]);
		set_item_userptr (items[i], (void*) &file_choices[i]);
	}
	items[i] = NULL;
	
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
	x_aux = MENU_X_SEPARATOR / 2 - 1;
	edit_menuWindow = CreateBoxedTitledWindow (MENU_HEIGHT, MENU_WIDTH, LINES - MENU_HEIGHT - 2, x_aux, "EDIT");
	
	num_items = 7;
	const char *edit_titles[] = {
		"Cut",
		"Copy",
		"Paste",
		"Selection mode",
		"Transparent [ ]",
		"Insert mode [ ]",
		"Paint mode  [ ]"
	};
	const char *edit_descriptions[] = {
		"^X",
		"^C",
		"^V",
		"^B",
		"^T",
		"Ins",
		"^P"
	};
	// The choices are static so that the userptr points to something that exists
	static const int edit_choices[] = {
		KEY_CTRL_X,
		KEY_CTRL_C,
		KEY_CTRL_V,
		KEY_CTRL_B,
		KEY_CTRL_T,
		KEY_IC,
		KEY_CTRL_P
	};
	// create the items
	items = (ITEM **) malloc ((num_items + 1) * sizeof (ITEM *));
	for (i = 0; i < num_items; i++) {
		items[i] = new_item (edit_titles[i], edit_descriptions[i]);
		set_item_userptr (items[i], (void*) &edit_choices[i]);
	}
	items[i] = NULL;
	
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
	
	num_items = 5;
	const char *image_titles[] = {
		"New Image",
		"Save Image",
		"Load Image",
		"Resize Image",
		"Trim Image"
	};
	const char *image_descriptions[] = {
		"F2",
		"^S",
		"^O",
		"^R",
		"^K"
	};
	// The choices are static so that the userptr points to something that exists
	static const int image_choices[] = {
		KEY_F(2),
		KEY_CTRL_S,
		KEY_CTRL_O,
		KEY_CTRL_R,
		KEY_CTRL_K
	};
	// create the items
	items = (ITEM **) malloc ((num_items + 1) * sizeof (ITEM *));
	for (i = 0; i < num_items; i++) {
		items[i] = new_item (image_titles[i], image_descriptions[i]);
		set_item_userptr (items[i], (void*) &image_choices[i]);
	}
	items[i] = NULL;
	
	// create the menu
	image_menu = new_menu (items);
	// options: cycle, mark...
	menu_opts_off (image_menu, O_NONCYCLIC);
	set_menu_mark (image_menu, "");

	// menu's windows
	set_menu_win (image_menu, image_menuWindow);
	set_menu_sub (image_menu, derwin (image_menuWindow,
			MENU_HEIGHT - 2, MENU_WIDTH - 2, 1, 1));
	
	post_menu (image_menu);

// HELP MENU
	x_aux += MENU_X_SEPARATOR;
	help_menuWindow = CreateBoxedTitledWindow (MENU_HEIGHT, MENU_WIDTH,
			LINES - MENU_HEIGHT - 2, x_aux, "HELP");
	
	num_items = 2;
	const char *help_titles[] = {
		"Show help",
		"About"
	};
	const char *help_descriptions[] = {
		"F1",
		"F12",
	};
	// The choices are static so that the userptr 
	// points to something that exists
	static const int help_choices[] = {
		KEY_F(1),
		KEY_F(12)
	};
	// create the items
	items = (ITEM **) malloc ((num_items + 1) * sizeof (ITEM *));
	for (i = 0; i < num_items; i++) {
		items[i] = new_item (help_titles[i], help_descriptions[i]);
		set_item_userptr (items[i], (void*) &help_choices[i]);
	}
	items[i] = NULL;
	
	// create the menu
	help_menu = new_menu (items);
	// options: cycle, mark...
	menu_opts_off (help_menu, O_NONCYCLIC);
	set_menu_mark (help_menu, "");

	// menu's windows
	set_menu_win (help_menu, help_menuWindow);
	set_menu_sub (help_menu, derwin (help_menuWindow, 
			MENU_HEIGHT - 2, MENU_WIDTH - 2, 1, 1));
	
	post_menu (help_menu);

	// allocate the submenus in the menu
	set_item_userptr (menu_items (menu)[0], file_menu);
	set_item_userptr (menu_items (menu)[1], edit_menu);
	set_item_userptr (menu_items (menu)[2], image_menu);
	set_item_userptr (menu_items (menu)[3], help_menu);
	submenuPanel = new_panel (file_menuWindow);
}


/// Auxilary function for menu
int GetChosenOption (MENU *menu) {
	int c;
	
	// The submenu, vertically shown
	MENU *submenu;
	
	// drives through the menu options
	do {
		// update what submenu we're watching, 
		// switching to the new submenu's WINDOW
		submenu = (MENU*) item_userptr (current_item (menu));
		replace_panel (submenuPanel, menu_win (submenu));

		// updates the MODES checkbox in EditMenu
		if (submenu == edit_menu) {
			char transp = IS_(TRANSPARENT) ? 'X' : ' ';
			char insert = IS_(INSERT) ? 'X' : ' ';
			char paint = IS_(PAINT) ? 'X' : ' ';
			// mark (or not) the modes
			mvwaddch (edit_menuWindow, CHKBX_Y    , CHKBX_X, transp);
			mvwaddch (edit_menuWindow, CHKBX_Y + 1, CHKBX_X, insert);
			mvwaddch (edit_menuWindow, CHKBX_Y + 2, CHKBX_X, paint);
			// if any mode is selected, reverse it's checkbox
			//  discover the mode position
			int aux = item_index (current_item (edit_menu)) - (item_count (edit_menu) - MODES);
			if (aux >= 0) {
				mvwchgat (edit_menuWindow, CHKBX_Y + aux, CHKBX_X,
						1, A_REVERSE, COLOR_PAIR (Normal), NULL);
			}
			wstandend (edit_menuWindow);
			pos_menu_cursor (submenu);
		}

		// refreshes the submenu's window
		update_panels ();
		doupdate ();

		c = getch ();
		switch (c) {
			// Mouse event: if menu, move to the right one;
			// if submenu, where clicked, accept it!
			case KEY_MOUSE:
				// chose the 'menu', go back for other drive
				if (menu_driver (menu, c) == E_OK) {
					break;
				}
				// clicked outside 'menu' or 'submenu': exit menu
				else if (menu_driver (submenu, c) != E_OK) {
					return 0;
				}

				// else (chose the submenu's item), accept it
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
	} while (c != ' '); 
	
	return *(int*) item_userptr (current_item (submenu));
}


int Menu () {
	// show in the Hud the menu is opened (eye-candy =P)
	mvwchgat (hud, 0, 9, 4, A_UNDERLINE | A_BOLD, CN, NULL);
	mvwchgat (hud, 0, 13, 5, A_UNDERLINE, CN, NULL);
	wrefresh (hud);

	// creates it if not already there
	if (!menuPanel) {
		InitMenus ();
	}

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
