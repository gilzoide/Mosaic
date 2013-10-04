#include <ncurses.h>
#include <panel.h>
#include <stdio.h>

#include "color.h"

#define CTRL_Q 17


void CursInit () {
	initscr ();	// init curses screen
	
	keypad (stdscr, TRUE);	// we can now use the arrow keys and Fn keys
	raw ();	// no need to wait for the RETURN key [for interactive means]
	noecho ();
	
	start_color ();	// Colors!
	assume_default_colors (-1, -1);	// there's the default terminal color
	InitColors ();	// initialize all the colors -> color.c
	
}


/* Draw the hud, with shortcuts, asks for things, that kinda stuff */
WINDOW *CreateHud () {
	WINDOW *win = newwin (1, COLS, LINES - 1, 0);
	
	
	
	return win;
}


int main () {
	CursInit ();
	WINDOW *hud = CreateHud ();
	
	int c;
	while ((c = getch ()) != CTRL_Q) {
		printw ("%d ", c);
	}
	
	endwin ();
}
