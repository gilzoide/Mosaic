#include "color.h"

void InitColors () {
	assume_default_colors (-1, -1);	// there's the default terminal color (Normal, in the colors enum)
	// black
	init_pair (BkBk, COLOR_BLACK, COLOR_BLACK);
	init_pair (BkR, COLOR_BLACK, COLOR_RED);
	init_pair (BkG, COLOR_BLACK, COLOR_GREEN);
	init_pair (BkY, COLOR_BLACK, COLOR_YELLOW);
	init_pair (BkBl, COLOR_BLACK, COLOR_BLUE);
	init_pair (BkM, COLOR_BLACK, COLOR_MAGENTA);
	init_pair (BkC, COLOR_BLACK, COLOR_CYAN);
	init_pair (BkW, COLOR_BLACK, COLOR_WHITE);
	// red
	init_pair (RBk, COLOR_RED, COLOR_BLACK);
	init_pair (RR, COLOR_RED, COLOR_RED);
	init_pair (RG, COLOR_RED, COLOR_GREEN);
	init_pair (RY, COLOR_RED, COLOR_YELLOW);
	init_pair (RBl, COLOR_RED, COLOR_BLUE);
	init_pair (RM, COLOR_RED, COLOR_MAGENTA);
	init_pair (RC, COLOR_RED, COLOR_CYAN);
	init_pair (RW, COLOR_RED, COLOR_WHITE);
	// green
	init_pair (GBk, COLOR_GREEN, COLOR_BLACK);
	init_pair (GR, COLOR_GREEN, COLOR_RED);
	init_pair (GG, COLOR_GREEN, COLOR_GREEN);
	init_pair (GY, COLOR_GREEN, COLOR_YELLOW);
	init_pair (GBl, COLOR_GREEN, COLOR_BLUE);
	init_pair (GM, COLOR_GREEN, COLOR_MAGENTA);
	init_pair (GC, COLOR_GREEN, COLOR_CYAN);
	init_pair (GW, COLOR_GREEN, COLOR_WHITE);
	// yellow
	init_pair (YBk, COLOR_YELLOW, COLOR_BLACK);
	init_pair (YR, COLOR_YELLOW, COLOR_RED);
	init_pair (YG, COLOR_YELLOW, COLOR_GREEN);
	init_pair (YY, COLOR_YELLOW, COLOR_YELLOW);
	init_pair (YBl, COLOR_YELLOW, COLOR_BLUE);
	init_pair (YM, COLOR_YELLOW, COLOR_MAGENTA);
	init_pair (YC, COLOR_YELLOW, COLOR_CYAN);
	init_pair (YW, COLOR_YELLOW, COLOR_WHITE);
	// blue
	init_pair (BlBk, COLOR_BLUE, COLOR_BLACK);
	init_pair (BlR, COLOR_BLUE, COLOR_RED);
	init_pair (BlG, COLOR_BLUE, COLOR_GREEN);
	init_pair (BlY, COLOR_BLUE, COLOR_YELLOW);
	init_pair (BlBl, COLOR_BLUE, COLOR_BLUE);
	init_pair (BlM, COLOR_BLUE, COLOR_MAGENTA);
	init_pair (BlC, COLOR_BLUE, COLOR_CYAN);
	init_pair (BlW, COLOR_BLUE, COLOR_WHITE);
	// magenta
	init_pair (MBk, COLOR_MAGENTA, COLOR_BLACK);
	init_pair (MR, COLOR_MAGENTA, COLOR_RED);
	init_pair (MG, COLOR_MAGENTA, COLOR_GREEN);
	init_pair (MY, COLOR_MAGENTA, COLOR_YELLOW);
	init_pair (MBl, COLOR_MAGENTA, COLOR_BLUE);
	init_pair (MM, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair (MC, COLOR_MAGENTA, COLOR_CYAN);
	init_pair (MW, COLOR_MAGENTA, COLOR_WHITE);
	// cyan
	init_pair (CBk, COLOR_CYAN, COLOR_BLACK);
	init_pair (CR, COLOR_CYAN, COLOR_RED);
	init_pair (CG, COLOR_CYAN, COLOR_GREEN);
	init_pair (CY, COLOR_CYAN, COLOR_YELLOW);
	init_pair (CBl, COLOR_CYAN, COLOR_BLUE);
	init_pair (CM, COLOR_CYAN, COLOR_MAGENTA);
	init_pair (CC, COLOR_CYAN, COLOR_CYAN);
	init_pair (CW, COLOR_CYAN, COLOR_WHITE);
	// white
	init_pair (WBk, COLOR_WHITE, COLOR_BLACK);
	init_pair (WR, COLOR_WHITE, COLOR_RED);
	init_pair (WG, COLOR_WHITE, COLOR_GREEN);
	init_pair (WY, COLOR_WHITE, COLOR_YELLOW);
	init_pair (WBl, COLOR_WHITE, COLOR_BLUE);
	init_pair (WM, COLOR_WHITE, COLOR_MAGENTA);
	init_pair (WC, COLOR_WHITE, COLOR_CYAN);
	init_pair (WW, COLOR_WHITE, COLOR_WHITE);
}


void TestColors_Curses () {
	int i;
	
	for (i = BkBk; i <= WW; i++) {
		attron (COLOR_PAIR (i));
		addch ('U');
		refresh ();
	}
}


void TestColors_Stdout () {
	int i;
	
	for (i = BkBk; i <= WW; i++) {
		printf ("%sU", Tcolor (i));
	}
}

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

//~ #define RESET   "\033[0m"
//~ #define BLACK   "\033[30m"      /* Black */
//~ #define RED     "\033[31m"      /* Red */
//~ #define GREEN   "\033[32m"      /* Green */
//~ #define YELLOW  "\033[33m"      /* Yellow */
//~ #define BLUE    "\033[34m"      /* Blue */
//~ #define MAGENTA "\033[35m"      /* Magenta */
//~ #define CYAN    "\033[36m"      /* Cyan */
//~ #define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

char *Tcolor (enum colors color) {
	char *color_table[] = {
		"\033[0m",
		"\033[31m"
	};
	
	return color_table[color];
}
