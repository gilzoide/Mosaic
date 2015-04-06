#include "wins.h"

// Special Windows definitions (it was too big all here)
#include "hud.c"
#include "menu.c"
#include "help.c"
#include "save_load.c"
#include "new_img.c"
#include "attr_table.c"
#include "resize.c"


char AskQuit () {
	// show in the Hud the Quit is requested (eye-candy =P)
	mvwchgat (hud, 0, 19, 3, A_UNDERLINE | A_BOLD, CN, NULL);
	mvwchgat (hud, 0, 22, 5, A_UNDERLINE, CN, NULL);
	wrefresh (hud);

	int choice = AskMessage ("Do you really want to quit?");
	
	// hud goes back to normal
	mvwchgat (hud, 0, 19, 3, A_BOLD, 0, NULL);
	mvwchgat (hud, 0, 22, 5, A_NORMAL, 0, NULL);
	wrefresh (hud);

	return choice;
}


char AskMessage (const char *msg) {
	// msg's size, vastly used
	int size = strlen (msg);
	// the auxiliary for sending msg with the asking part for PrintHud
	char aux[size + 7];
	strcpy (aux, msg);
	strcpy (aux + size, " [y/N]");

	return 'y' == tolower (PrintHud (TRUE, aux));
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
// Resize MOSAIC
	DeleteForm (&resize_form);
	DeletePanel (&resizePanel);
// Load MOSAIC
	DeleteForm (&saveloadMOSAIC_form);
	DeletePanel (&saveloadMOSAICPanel);
// About
	DeletePanel (&aboutPanel);
// AttrTable
	DeletePanel (&attrPanel);

	endwin ();
}
