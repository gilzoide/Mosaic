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
		"Nmos commands (basic hotkeys)",
		"Navigation",
		"Mosaic editing"
	};
	// the hotkeys
	const char *hotkeys[] = {
		"F1", "F10/Mouse Right Button", "^Q",
		"Arrow Keys", "^D", "^B", "^A", "Page Up/Page Down", "Home/End",
		"F2", "^S", "^O", "^R", "^C/^X", "^V", "Tab", "^U", "^W"
	};
	// and how many are there for each subtitle
	int n_hotkeys[] = {3, 6, 9};
	// what the hotkeys do
	const char *explanations[] = {
		"show this help", "show the menu", "quit Nmos",
		"move through the mosaic", "change the moving direction after input (default direction)", "toggle box selection mode", "select all", "previous/next mosaic", "move to first/last character (in the default direction)",
		"new mosaic", "save mosaic", "load mosaic", "resize mosaic", "copy/cut selection", "paste selection", "show the attribute table", "erase line", "erase word"
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
	
	int aux = HUD_CURSOR_X - HUD_MSG_X;
	mvwaddch (helpWindow, HELP_HEIGHT - 1, 29, ACS_ULCORNER);
	for (i = 0; i < aux; i++) {
		wechochar (helpWindow, ACS_HLINE);
	}
	waddch (helpWindow, ACS_URCORNER);
	mvwaddstr (helpWindow, HELP_HEIGHT - 1, HUD_MSG_X + (aux / 2), " message area ");
	
	mvwaddch (helpWindow, HELP_HEIGHT - 1, HELP_WIDTH - 11, ACS_ULCORNER);
	waddstr (helpWindow, "position");
	waddch (helpWindow, ACS_URCORNER); waddch (helpWindow, ACS_VLINE);
	mvwaddstr (helpWindow, HELP_HEIGHT - 2, HELP_WIDTH - 18, "default direction");
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
