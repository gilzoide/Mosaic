// needed WINDOWs and PANELs
WINDOW *helpWindow;
PANEL *helpPanel;

WINDOW *aboutWindow;
PANEL *aboutPanel;


void InitHelp () {
	helpWindow = CreateBoxedTitledWindow (HELP_HEIGHT, HELP_WIDTH, 0, 0, "HELP");
	helpPanel = new_panel (helpWindow);
	hide_panel (helpPanel);

	wbkgd (helpWindow, COLOR_PAIR (WBl));

	// the subtitles
	const char *subtitles[] = {
		"Maae commands (basic hotkeys)",
		"Navigation",
		"Modes",
		"Mosaic editing"
	};
	// the hotkeys
	const char *hotkeys[] = {
		"F1", "F10/Mouse Right Button", "^Q",
		"Arrow Keys", "^D", "^A", "Page Up/Page Down", "^G", "Home/End", "Mouse Left Button", "Mouse Left Button double click",
		"^B", "^T", "^P", "Insert",
		"F2", "^S", "^O", "^R", "^C/^X", "^V", "Tab", "^U", "^W"
	};
	// and how many are there for each subtitle
	int n_hotkeys[] = {3, 8, 4, 9};
	// what the hotkeys do
	const char *explanations[] = {
		"show this help", "show the menu", "quit Nmos",
		"move through the mosaic", "change the moving direction after input (default direction)", "select all", "previous/next mosaic", "go to mosaic by index", "move to first/last character (in the default direction)", "move to", "select until",
		"toggle box selection mode", "toggle transparent pasting mode", "toggle paint mode", "toggle insert mode",
		"new mosaic", "save mosaic", "load mosaic", "resize mosaic", "copy/cut selection", "paste selection", "show the attribute table", "erase line", "erase word"
	};
	
	// aux counters; only 'i' gets reseted at 0, as it counts until n_hotkeys ends
	int sub, hot, i, line = 1;
	// for each subtitle
	for (sub = hot = 0; sub < 4; sub++, line++) {
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
	waddstr (helpWindow, " Maae basic hotkeys ");
	waddch (helpWindow, ACS_HLINE);
	waddch (helpWindow, ACS_HLINE);
	waddch (helpWindow, ACS_HLINE);
	waddch (helpWindow, ACS_URCORNER);

	wclrtoeol (helpWindow);
	
	// print the HLines and corners for the message area
	int message_area_size = HELP_WIDTH - HUD_CURSOR_X - HUD_MSG_X - MODES;
	mvwaddch (helpWindow, HELP_HEIGHT - 1, HUD_MSG_X, ACS_ULCORNER);
	for (i = 0; i < message_area_size; i++) {
		wechochar (helpWindow, ACS_HLINE);
	}
	waddch (helpWindow, ACS_URCORNER);
	mvwaddstr (helpWindow, HELP_HEIGHT - 1,
			HUD_MSG_X + (message_area_size / 2), " message area ");

	// modes (1 line above)
	mvwaddstr (helpWindow,
			HELP_HEIGHT - 2, HELP_WIDTH - HUD_CURSOR_X - 4, "modes");
	waddch (helpWindow, ACS_URCORNER);
	mvwaddch (helpWindow,
			HELP_HEIGHT - 1, HELP_WIDTH - HUD_CURSOR_X, ACS_ULCORNER);
	waddch (helpWindow, ACS_PLUS);
	waddch (helpWindow, ACS_URCORNER);

	// position (last in the line, that's what that VLine is for)
	mvwaddch (helpWindow, HELP_HEIGHT - 1,
			HELP_WIDTH - HUD_CURSOR_X + MODES + 1, ACS_ULCORNER);
	waddstr (helpWindow, "position");
	waddch (helpWindow, ACS_URCORNER);
	waddch (helpWindow, ACS_VLINE);

	// default direction (2 lines above)
	mvwaddstr (helpWindow,
			HELP_HEIGHT - 3, HELP_WIDTH - 18, "default direction");
	waddch (helpWindow, ACS_URCORNER);

	touchwin (helpWindow);
}


void InitAbout () {
	aboutWindow = CreateCenteredBoxedTitledWindow (ABOUT_HEIGHT, ABOUT_WIDTH,
			"ABOUT");
	aboutPanel = new_panel (aboutWindow);
	hide_panel (aboutPanel);

	mvwaddstr (aboutWindow, 1, 1, 
			"Maae: the curses based Mosaic Asc Art Editor");

	touchwin (aboutWindow);
}


/* Displays the help (in a created window and panel, for going back to the normal field after) */
void Help () {
	curs_set (0);	// don't display the cursor, pliz

	// show in the Hud the help is opened (eye-candy =P)
	mvwchgat (hud, 0, 0, 3, A_UNDERLINE | A_BOLD, CN, NULL);
	mvwchgat (hud, 0, 3, 5, A_UNDERLINE, CN, NULL);
	wrefresh (hud);

	// creates it if not already there
	if (!helpPanel) {
		InitHelp ();
	}
	// display the help
	show_panel (helpPanel);
	update_panels ();
	doupdate ();
	// waits for some key to be pressed
	getch ();
	
	// hud goes back to normal
	mvwchgat (hud, 0, 0, 3, A_BOLD, 0, NULL);
	mvwchgat (hud, 0, 3, 5, A_NORMAL, 0, NULL);
	wrefresh (hud);

	// hide the help
	hide_panel (helpPanel);
	doupdate ();

	curs_set (1);	// and back with the cursor
}


void About () {
	curs_set (0);	// hide the cursor
	
	// creates it if not already there
	if (!aboutPanel) {
		InitAbout ();
	}

	// display the About
	show_panel (aboutPanel);
	update_panels ();
	doupdate ();
	// waits for some key to be pressed
	getch ();
	
	// hide the About
	hide_panel (aboutPanel);
	doupdate ();

	curs_set (1);	// and back with the cursor
}
