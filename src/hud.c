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
	move (cur.y, cur.x);
}


int PrintHud (const char *message, char wait_for_input) {
	// clear anything that was there
	wmove (hud, 0, HUD_MSG_X);
	wclrtoeol (hud);
	// add the message
	mvwaddch (hud, 0, HUD_MSG_X, ACS_DIAMOND);
	waddstr (hud, message);
	wrefresh (hud);

	int c = 0;
	// wait for input
	if (wait_for_input) {
		c = getch ();
	}
	// if not, let the system know the message is there
	else {
		ENTER_(HUD_MESSAGE);
	}

	return c;
}
