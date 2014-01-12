#include "nmos.h"
#include "color.h"
#include "img.h"

void CursInit () {
	initscr ();	// init curses screen
	
	keypad (stdscr, TRUE);	// we can now use the arrow keys and Fn keys
	raw ();	// no need to wait for the RETURN key [for interactive means]
	noecho ();
	
	start_color ();	// Colors!
	assume_default_colors (-1, -1);	// there's the default terminal color
	InitColors ();	// initialize all the colors -> color.c
}


WINDOW *CreateHud () {
	WINDOW *win = subwin (stdscr, 1, COLS, LINES - 2, 0);
	
	wbkgd (win, COLOR_PAIR (WG) | A_BOLD);
	waddstr (win, "Help: '^H'");

	wrefresh (win);
	return win;
}


/* Displays the help (in a created window and panel, for going back to the normal field after) */
void Help () {
	WINDOW *help;
	PANEL *up;

	help = newwin (12, HELP_WIDTH, LINES - 15, 0);
	up = new_panel (help);
	update_panels ();
	doupdate ();

	box (help, 0, 0);
	wbkgd (help, COLOR_PAIR (WBl));
	wrefresh (help);
	mvwaddstr (help, 0, HELP_WIDTH/2 - 2, "HELP");
	wattron (help, A_BOLD);
	mvwaddstr (help, 1, 1, "Arrow Keys or W,A,S,D:");
	mvwaddstr (help, 2, 1, "'z':");
	mvwaddstr (help, 3, 1, "Space:");
	mvwaddstr (help, 4, 1, "'q':");

	mvwaddstr (help, 6, 1, "O:");
	mvwaddstr (help, 7, 1,  "/VC");
	mvwaddstr (help, 8, 1, "\\_/:");

	mvwaddstr (help, 10, 1, "K:");

	wattrset (help, COLOR_PAIR (WBl));
	mvwaddstr (help, 1, 24, "turn");
	mvwaddstr (help, 2, 6, "change second head's side");
	mvwaddstr (help, 3, 8, "pause");
	mvwaddstr (help, 4, 6, "quit");
	mvwaddstr (help, 6, 4, "food: eat it for some points");
	mvwaddstr (help, 8, 6, "apple: eat it with both heads for 500 points!");
	mvwaddstr (help, 10, 4, "K stands for Hunter: it'll try to kill you!");
	// but actually you need to spend 500 - (dificulty + 1) points in it, so is it worth it? xP


// writes the help window, wait for some key to be pressed and delete the help window
	wrefresh (help);
	getch ();

	werase (help);
	wrefresh (help);
	del_panel (up);
	delwin (help);
}
