#include "wins.h"

// Global WINDOWS, PANELS, MENUS, etc
WINDOW *hud;

// Menu and Help definitions (it was too big all here)
#include "menu.c"
#include "help.c"
#include "save_load.c"
#include "new_img.c"
#include "attr_table.c"

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
	// show insert mode icon
	if (IS_(INSERT)) {
		wattron (hud, A_BOLD);
		mvwaddch (hud, 0, COLS - HUD_CURSOR_X, 'I');
	}
	// show transparent mode icon
	if (IS_(TRANSPARENT)) {
		wattron (hud, A_BOLD);
		mvwaddch (hud, 0, COLS - HUD_CURSOR_X + 1, 'T');
	}
	// update coordinates
	wstandend (hud);
	mvwprintw (hud, 0, COLS - HUD_CURSOR_X + 3, "%dx%d", cur.y, cur.x);
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


char AskQuit () {
	// show in the Hud the Quit is requested (eye-candy =P)
	mvwchgat (hud, 0, 19, 3, A_UNDERLINE | A_BOLD, CN, NULL);
	mvwchgat (hud, 0, 22, 5, A_UNDERLINE, CN, NULL);
	wrefresh (hud);

	int choice = PrintHud ("Do you really wanna quit? [y/N]", TRUE);
	
	// hud goes back to normal
	mvwchgat (hud, 0, 19, 3, A_BOLD, 0, NULL);
	mvwchgat (hud, 0, 22, 5, A_NORMAL, 0, NULL);
	wrefresh (hud);

	return tolower (choice) == 'y' ? 1 : 0;
}


WINDOW *CreateCenteredBoxedTitledWindow (
		int nlines, int ncols, 
		const char *title) {
	int y = (LINES - nlines) / 2;
	int x = (COLS - ncols) / 2;
	return CreateBoxedTitledWindow (nlines, ncols, y, x, title);
}


WINDOW *CreateBoxedTitledWindow (
		int nlines, int ncols,
		int begin_y, int begin_x,
		const char *title) {
	WINDOW *win = newwin (nlines, ncols, begin_y, begin_x);

	box (win, 0, 0);
	
	int length = strlen (title);
	int title_begin = (ncols - length)/2;
	mvwaddstr (win, 0, title_begin, title);
	mvwaddch (win, 0, title_begin - 1, ' ');
	mvwaddch (win, 0, title_begin + length, ' ');
	
	return win;
}


void DeletePanel (PANEL **pan) {
	if (pan && *pan) {
		WINDOW *win = panel_window (*pan);
		DeleteWindow (&win);

		del_panel (*pan);

		*pan = NULL;
	}
}


void DeleteWindow (WINDOW **win) {
	if (win && *win) {
		delwin (*win);

		*win = NULL;
	}
}


void DeleteMenu (MENU **menu) {
	if (menu && *menu) {
		unpost_menu (*menu);
		free_menu (*menu);

		// Destroy all the items!
		int i;
		ITEM **items = menu_items (*menu);
		for (i = 0; i < item_count (*menu); i++)
			free_item (items[i]);
		free (items);
		
		WINDOW *win = menu_win (*menu);
		DeleteWindow (&win);
		WINDOW *sub = menu_sub (*menu);
		DeleteWindow (&sub);

		*menu = NULL;
	}
}


void DeleteForm (FORM **form) {
	if (form && *form) {
		unpost_form (*form);
		free_form (*form);

		// Destroy all the fields!
		int i;
		FIELD **fields = form_fields (*form);
		for (i = 0; i < field_count (*form); i++) {
			free_field (fields[i]);
		}
		free (fields);

		WINDOW *win = form_win (*form);
		DeleteWindow (&win);
		WINDOW *sub = form_sub (*form);
		DeleteWindow (&sub);

		*form = NULL;
	}
}


void DestroyWins () {
// Help
	DeletePanel (&helpPanel);
// Hud
	DeleteWindow (&hud);
// Menus
	DeleteMenu (&menu);
	DeleteMenu (&file_menu);
	DeleteMenu (&edit_menu);
	DeleteMenu (&image_menu);
	DeleteMenu (&help_menu);
	DeletePanel (&menuPanel);
	DeletePanel (&submenuPanel);
// New MOSAIC
	DeleteForm (&newMOSAIC_form);
	DeletePanel (&newMOSAICPanel);
// Load MOSAIC
	DeleteForm (&saveloadMOSAIC_form);
	DeletePanel (&saveloadMOSAICPanel);
// About
	DeletePanel (&aboutPanel);
// AttrTable
	DeletePanel (&attrPanel);

	endwin ();
}
