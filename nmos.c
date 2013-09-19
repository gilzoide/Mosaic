#include <ncurses.h>
#include <panel.h>
#include <stdio.h>

#include "color.h"



void CursInit () {
	initscr ();	// init curses screen
	
	keypad (stdscr, TRUE);	// we can now use the arrow keys and Fn keys
	cbreak ();	// no need to wait for the RETURN key [for interactive means]
	
	start_color ();	// Colors!
	assume_default_colors (-1, -1);	// there's the default terminal color
	InitColors ();	// initialize all the colors -> color.c
	
}


int main () {
	CursInit ();
	
	TestColors ();
	
	nodelay (stdscr, FALSE);
	getch ();
	
	endwin ();
}
