// Attr stuff
WINDOW *attrWindow;
PANEL *attrPanel;

#define ATTR_HEIGHT 13
#define ATTR_WIDTH 23
#define ATTR_TEST_Y (ATTR_HEIGHT - 2)
#define ATTR_TEST_X 11
#define ATTR_COLOR_X 9

void InitAttrTable () {
	attrWindow = CreateCenteredBoxedTitledWindow (ATTR_HEIGHT, ATTR_WIDTH,
			"PALLETE");
	attrPanel = new_panel (attrWindow);

	const char *colors[] = {
		" normal",
		"  black",
		"    red",
		"  green",
		" yellow",
		"   blue",
		"magenta",
		"   cyan",
		"  white"
	};

	int i;
	// for each color with normal background
	for (i = 0; i < COLORS_STEP; i ++) {
		// print the color name, colored
		wattron (attrWindow, COLOR_PAIR (i * COLORS_STEP));
		mvwaddstr (attrWindow, 1 + i, 1, colors[i]);
	}

	mvwaddstr (attrWindow, ATTR_TEST_Y, 1, "[ ] ");
	wattron (attrWindow, A_BOLD);
	waddstr (attrWindow, "Bold");
	wstandend (attrWindow);
}


int chooseAttr (Attr current_color) {
	// input from user
	int c = 0;
	// is it bold yet?
	// current chosen foreground, current chosen background, bold
	Attr attrs[] = {
		current_color / COLORS_STEP,
		current_color % COLORS_STEP,
		extractBold (&current_color)
	};
	int moving = 0;

	do {
		switch (c) {
			// switch moving
			case KEY_LEFT:
				moving = (moving - 1);
				if (moving < 0) {
					moving = 2;
				}
				break;
			
			case KEY_RIGHT:
				moving = (moving + 1) % 3;
				break;

			// switch values
			case KEY_UP:
				if (moving < 2) {
					// erase } moving, before moving it
					mvwaddstr (attrWindow, 1 + attrs[moving],
							ATTR_COLOR_X, "           ");
					attrs[moving]--;
					if (attrs[moving] > COLORS_STEP) {
						attrs[moving] = COLORS_STEP - 1;
					}
				}
				break;

			case KEY_DOWN:
				if (moving < 2) {
					// erase } moving, before moving it
					mvwaddstr (attrWindow, 1 + attrs[moving],
							ATTR_COLOR_X, "           ");
					attrs[moving]++;
					attrs[moving] %= COLORS_STEP;
				}
				break;

			case ' ':
				attrs[2] ^= BOLD;
				break;

			// cancelled, so return what came
			case KEY_ESC:
				// erase } fore back
				mvwaddstr (attrWindow, 1 + attrs[0],
						ATTR_COLOR_X, "           ");
				mvwaddstr (attrWindow, 1 + attrs[1],
						ATTR_COLOR_X, "           ");
				return (current_color | attrs[2]);
		}

		// we write back before fore, so when they're both
		// on the same line, both of them appear
		mvwaddstr (attrWindow, 1 + attrs[1], ATTR_COLOR_X, "}      back");
		mvwaddstr (attrWindow, 1 + attrs[0], ATTR_COLOR_X, "} fore");
		mvwaddch (attrWindow, ATTR_TEST_Y, 2, attrs[2] ? 'X' : ' ');

		// Underline the moving attr
		if (moving == 1) {
			mvwchgat (attrWindow, 1 + attrs[moving], ATTR_COLOR_X + 7, 4,
					A_UNDERLINE, Normal, NULL);
		}
		else if (moving == 0) {
			mvwchgat (attrWindow, 1 + attrs[moving], ATTR_COLOR_X + 2, 4,
					A_UNDERLINE, Normal, NULL);
		}
		else {
			mvwchgat (attrWindow, ATTR_TEST_Y, 2, 1,
					A_UNDERLINE, Normal, NULL);
		}
		// write the " TestArea "
		wattrset (attrWindow, COLOR_PAIR (attrs[0] * COLORS_STEP + attrs[1]));
		if (attrs[2]) {
			wattron (attrWindow, A_BOLD);
		}
		mvwaddstr (attrWindow, ATTR_TEST_Y, ATTR_TEST_X, " TestArea ");
		wstandend (attrWindow);

		wrefresh (attrWindow);

		c = getch ();
	} while (c != '\n');

	// erase } fore back
	mvwaddstr (attrWindow, 1 + attrs[0], ATTR_COLOR_X, "           ");
	mvwaddstr (attrWindow, 1 + attrs[1], ATTR_COLOR_X, "           ");

	return ((attrs[0] * COLORS_STEP + attrs[1]) | attrs[2]);
}


Attr AttrTable (CURS_MOS *current, Cursor cur) {
	if (!attrPanel) {
		InitAttrTable ();
	}

	show_panel (attrPanel);
	update_panels ();
	doupdate ();

	Attr value = chooseAttr (mosGetAttr (current->img, cur.y, cur.x));

	hide_panel (attrPanel);
	update_panels ();
	doupdate ();

	return value;
}


