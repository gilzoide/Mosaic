// The New Img stuff
FORM *resize_form;
WINDOW *resizeWindow;
PANEL *resizePanel;

#define RESIZE_height 4
#define RESIZE_width 17

void InitResizeMOSAIC () {
	resizeWindow = CreateCenteredBoxedTitledWindow (RESIZE_height,
			RESIZE_width, "RESIZE");
	resizePanel = new_panel (resizeWindow);

	mvwaddstr (resizeWindow, 1, 1, "New height");
	mvwaddstr (resizeWindow, 2, 1, "New width");

	/* MAKING OF FORM */
	FIELD **fields = (FIELD **) malloc (3 * sizeof (FIELD *));
	fields[0] = new_field (1, 3, 0, 0, 0, 0);
	set_field_back (fields[0], A_BOLD);
	field_opts_off (fields[0], O_PASSOK);
	set_field_just (fields[0], JUSTIFY_LEFT);
	set_field_type (fields[0], TYPE_INTEGER, 0, 1, 999);

	fields[1] = new_field (1, 3, 1, 0, 0, 0);
	set_field_back (fields[1], A_BOLD);
	field_opts_off (fields[1], O_PASSOK);
	set_field_just (fields[1], JUSTIFY_LEFT);
	set_field_type (fields[1], TYPE_INTEGER, 0, 1, 999);

	fields[2] = NULL;

	// the FORM itself, WINDOW and post it!
	resize_form = new_form (fields);
	// subwindow: inside the box
	WINDOW *subwindow = derwin (resizeWindow, 2, 3, 1, 12);
	set_form_win (resize_form, subwindow);
	set_form_sub (resize_form, subwindow);
	post_form (resize_form);

	touchwin (resizeWindow);
}


char AskResizeMOSAIC (int *new_height, int *new_width) {
	// creates it if not already there
	if (!resizePanel) {
		InitResizeMOSAIC ();
	}

	// current height, to start the buffer
	char height[4];
	sprintf (height, "%3d", *new_height);
	set_field_buffer (form_fields (resize_form)[0], 0, height);
	// current width, to start the buffer
	char width[4];
	sprintf (width, "%3d", *new_width);
	set_field_buffer (form_fields (resize_form)[1], 0, width);

	// display the panel
	show_panel (resizePanel);
	update_panels ();
	doupdate ();

	int c;

	do {
		c = getch ();

		switch (c) {
			// previous
			case KEY_UP:
				form_driver (resize_form, REQ_PREV_FIELD);
				break;

			// neeeext
			case KEY_DOWN:
				form_driver (resize_form, REQ_NEXT_FIELD);
				break;

			// get out, don't create the new image
			case KEY_ESC:
				hide_panel (resizePanel);
				doupdate ();
				return ERR;

			// backspace: start over
			case KEY_BACKSPACE: case 127:
				form_driver (resize_form, REQ_PREV_FIELD);
				form_driver (resize_form, REQ_NEXT_FIELD);
				break;

			// write the dimensions in the field
			default:
				if (isdigit (c)) {	// only allow digits (for the dimensions)
					form_driver (resize_form, c);
				}
				break;
		}

		wrefresh (resizeWindow);
	} while (c != '\n');

	// validate the string (current field doesn't validate by itself)
	form_driver (resize_form, REQ_VALIDATION);

	// get the fields' data
	*new_height = atoi (field_buffer (form_fields (resize_form)[0], 0));
	*new_width = atoi (field_buffer (form_fields (resize_form)[1], 0));

	hide_panel (resizePanel);
	update_panels ();
	doupdate ();

	return OK;
}
