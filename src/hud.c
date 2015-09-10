WINDOW *hud;


void InitHud () {
	hud = subwin (stdscr, 1, COLS, LINES - 1, 0);
	ReHud ();
}


void ReHud () {
	wmove (hud, 0, 0);
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


void UpdateHud (Cursor cur, Direction dir) {
	int arrow;
	// arrows, to show the direction!
	switch (dir) {
		case UP: 	arrow = ACS_UARROW; break;
		case DOWN: 	arrow = ACS_DARROW; break;
		case LEFT:	arrow = ACS_LARROW; break;
		case RIGHT:	arrow = ACS_RARROW; break;
	}

	// if there ain't no message in the hud for showing, so clear it for update
	if (!IS_(HUD_MESSAGE)) {
		wmove (hud, 0, HUD_MSG_X);
		wclrtoeol (hud);
	}
	// maybe there is a message, so next time we'll destroy it, VWAHAHAHAHA!
	else {
		UN_(HUD_MESSAGE);
	}
	
	// show paint mode icon
	if (IS_(PAINT)) {
		wattron (hud, A_BOLD);
		mvwaddch (hud, 0, COLS - HUD_CURSOR_X, 'P');
	}
	// show insert mode icon
	if (IS_(INSERT)) {
		wattron (hud, A_BOLD);
		mvwaddch (hud, 0, COLS - HUD_CURSOR_X + 1, 'I');
	}
	// show transparent mode icon
	if (IS_(TRANSPARENT)) {
		wattron (hud, A_BOLD);
		mvwaddch (hud, 0, COLS - HUD_CURSOR_X + 2, 'T');
	}
	// update coordinates
	wstandend (hud);
	mvwprintw (hud, 0, COLS - HUD_CURSOR_X + 4, "%dx%d", cur.y, cur.x);
	mvwaddch (hud, 0, COLS - 1, arrow);
	wrefresh (hud);
	move (cur.y % MOSAIC_PAD_HEIGHT, cur.x % MOSAIC_PAD_WIDTH);
}


int PrintHud (char wait_for_input, const char *message) {
	// clear anything that was there
	wmove (hud, 0, HUD_MSG_X);
	wclrtoeol (hud);
	// add the message
	mvwaddch (hud, 0, HUD_MSG_X, ACS_DIAMOND);
	waddstr (hud, message);
	wrefresh (hud);

	int c = 0;
	// wait for input
	switch (wait_for_input) {
		case TRUE:
			c = getch ();
			break;

		case SCAN:
			echo ();
			waddch (hud, ' ');
			wrefresh (hud);
			if (wscanw (hud, "%d", &c) == ERR) {
				c = ERR;
			}
			noecho ();
			break;

		case FALSE:
			ENTER_(HUD_MESSAGE);
			break;
	}

	return c;
}


int VPrintHud (char wait_for_input, const char *message, ...) {
	char buffer[100];
	va_list ap;
	va_start (ap, message);
	// make the string
	vsprintf (buffer, message, ap);

	return PrintHud (wait_for_input, buffer);
}
