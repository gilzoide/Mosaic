// The New Img stuff
FORM *newMOSAIC_form;
WINDOW *newMOSAICWindow;
PANEL *newMOSAICPanel;

#define newMOSAIC_height 6
#define newMOSAIC_width 20

void InitNewMOSAIC (int initial_height, int initial_width) {
	newMOSAICWindow = CreateCenteredBoxedTitledWindow (newMOSAIC_height,
			newMOSAIC_width, "NEW IMAGE");
	newMOSAICPanel = new_panel (newMOSAICWindow);

	// subwindow: inside the box
	WINDOW *subwindow = derwin (newMOSAICWindow, 4, 6, 1, 12);
	mvwaddstr (newMOSAICWindow, 1, 1, "Height");
	mvwaddstr (newMOSAICWindow, 2, 1, "Width");
	mvwaddstr (newMOSAICWindow, 3, 1, "Duplicate");
	mvwaddstr (newMOSAICWindow, 4, 1, "Link image");

	/* MAKING OF FORM */
	FIELD **fields = (FIELD**) calloc (5, sizeof (FIELD*));
	fields[0] = new_field (1, 3, 0, 0, 0, 0);
	set_field_back (fields[0], A_BOLD);
	field_opts_off (fields[0], O_PASSOK);
	set_field_just (fields[0], JUSTIFY_LEFT);
	set_field_type (fields[0], TYPE_INTEGER, 0, 1, 999);
	// initial_height as a string, to start the buffer
	char height[4];
	sprintf (height, "%3d", initial_height);
	set_field_buffer (fields[0], 0, height);

	fields[1] = new_field (1, 3, 1, 0, 0, 0);
	set_field_back (fields[1], A_BOLD);
	field_opts_off (fields[1], O_PASSOK);
	set_field_just (fields[1], JUSTIFY_LEFT);
	set_field_type (fields[1], TYPE_INTEGER, 0, 1, 999);
	// initial_width as a string, to start the buffer
	char width[4];
	sprintf (width, "%3d", initial_width);
	set_field_buffer (fields[1], 0, width);

	fields[2] = new_field (1, 3, 2, 0, 0, 0);
	set_field_back (fields[2], A_BOLD);
	field_opts_off (fields[2], O_EDIT | O_AUTOSKIP);
	const char *checkbox[] = {
		"no",
		"yes",
		NULL
	};
	set_field_type (fields[2], TYPE_ENUM, checkbox, 0, 1);
	set_field_buffer (fields[2], 0, checkbox[0]);

	fields[3] = new_field (1, 6, 3, 0, 0, 0);
	set_field_back (fields[3], A_BOLD);
	field_opts_off (fields[3], O_EDIT | O_AUTOSKIP);
	const char *directions[] = {
		"after",
		"before",
		NULL
	};
	set_field_type (fields[3], TYPE_ENUM, directions, 0, 1);
	set_field_buffer (fields[3], 0, directions[0]);

	// the FORM itself, WINDOW and post it!
	newMOSAIC_form = new_form (fields);
	set_form_win (newMOSAIC_form, subwindow);
	set_form_sub (newMOSAIC_form, subwindow);
	post_form (newMOSAIC_form);

	touchwin (newMOSAICWindow);
}

char AskCreateNewMOSAIC (int *new_height, int *new_width, char *duplicate, enum direction *new_dir) {
	// creates it if not already there
	if (!newMOSAICPanel) {
		InitNewMOSAIC (*new_height, *new_width);
	}

	// display the panel
	show_panel (newMOSAICPanel);
	update_panels ();
	doupdate ();

	int c;

	do {
		c = getch ();

		switch (c) {
			// previous
			case KEY_UP:
				form_driver (newMOSAIC_form, REQ_PREV_FIELD);
				break;

			// neeeext
			case KEY_DOWN:
				form_driver (newMOSAIC_form, REQ_NEXT_FIELD);
				break;

			// there are only 2 possibilities, so require next
			case KEY_RIGHT: case KEY_LEFT: case ' ':
				form_driver (newMOSAIC_form, REQ_NEXT_CHOICE);
				break;

			// get out, don't create the new image
			case KEY_ESC:
				hide_panel (newMOSAICPanel);
				doupdate ();
				return ERR;

			// backspace: start over
			case KEY_BACKSPACE: case 127:
				form_driver (newMOSAIC_form, REQ_PREV_FIELD);
				form_driver (newMOSAIC_form, REQ_NEXT_FIELD);
				break;

			// write the dimensions in the field
			default:
				if (isdigit (c)) {	// only allow digits (for the dimensions)
					form_driver (newMOSAIC_form, c);
				}
				break;
		}

		wrefresh (newMOSAICWindow);
	} while (c != '\n');

	// validate the string (current field doesn't validate by itself)
	form_driver (newMOSAIC_form, REQ_VALIDATION);

	// get the fields' data
	*new_height = atoi (field_buffer (form_fields (newMOSAIC_form)[0], 0));
	*new_width = atoi (field_buffer (form_fields (newMOSAIC_form)[1], 0));
	// using strncmp because the field's buffer is completed with trailing 
	// blanks, and strcmp wouldn't give us the desired answer
	*duplicate = strncmp (field_buffer (form_fields (newMOSAIC_form)[2], 0),
			"no", 2);
	*new_dir = strncmp (field_buffer (form_fields (newMOSAIC_form)[3], 0),
			"after", 5);

	hide_panel (newMOSAICPanel);
	update_panels ();
	doupdate ();

	return OK;
}



