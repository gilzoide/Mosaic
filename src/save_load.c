// The save and load stuff
// save and load have the same form, as it does the same (and keeps the buffer)
FORM *saveloadMOSAIC_form;
WINDOW *saveloadMOSAICWindow;
PANEL *saveloadMOSAICPanel;


void InitSaveLoadMOSAIC () {
	saveloadMOSAICWindow = CreateCenteredBoxedTitledWindow (6, 
			SAVELOAD_WIDTH + 2, "LOAD IMAGE");
	saveloadMOSAICPanel = new_panel (saveloadMOSAICWindow);

	mvwaddstr (saveloadMOSAICWindow, 2, 5, "File name");

	// subwindow: inside the box
	WINDOW *subwindow = derwin (saveloadMOSAICWindow, 1, SAVELOAD_WIDTH, 3, 1);

	/* MAKING OF FORM */
	FIELD **fields = (FIELD**) calloc (2, sizeof (FIELD*));
	fields[0] = new_field (1, SAVELOAD_WIDTH, 0, 0, 0, 0);
	set_field_back (fields[0], A_BOLD);
	field_opts_off (fields[0], O_PASSOK | O_STATIC);

	// the FORM itself, WINDOW and post it!
	saveloadMOSAIC_form = new_form (fields);
	set_form_sub (saveloadMOSAIC_form, subwindow);
	post_form (saveloadMOSAIC_form);

	wrefresh (saveloadMOSAICWindow);
}


char *AskSaveLoadMOSAIC (enum io io) {
	// creates it if not already there
	if (!saveloadMOSAICPanel)
		InitSaveLoadMOSAIC ();

	// changes the title if saving or loading
	if (io == save) {
		mvwaddstr (saveloadMOSAICWindow, 0, 5, "SAVE");
	}
	else {
		mvwaddstr (saveloadMOSAICWindow, 0, 5, "LOAD");
	}

	// cursor on the form, please!
	pos_form_cursor (saveloadMOSAIC_form);

	// display the panel
	show_panel (saveloadMOSAICPanel);
	update_panels ();
	doupdate ();

	int c;

	do {
		c = getch ();

		switch (c) {
			case KEY_RIGHT:
				form_driver (saveloadMOSAIC_form, REQ_RIGHT_CHAR);
				break;

			case KEY_LEFT:
				form_driver (saveloadMOSAIC_form, REQ_LEFT_CHAR);
				break;

			// get out, don't load the image
			case KEY_ESC:
				hide_panel (saveloadMOSAICPanel);
				doupdate ();
				return NULL;

			case KEY_DC:
				form_driver (saveloadMOSAIC_form, REQ_DEL_CHAR);
				break;

			case KEY_BACKSPACE: case 127:
				form_driver (saveloadMOSAIC_form, REQ_DEL_PREV);
				break;

			// write the dimensions in the field
			default:
				form_driver (saveloadMOSAIC_form, c);
				break;
		}

		wrefresh (saveloadMOSAICWindow);
	} while (c != '\n');

	hide_panel (saveloadMOSAICPanel);
	update_panels ();
	doupdate ();

	// validate the string (current field doesn't validate by itself)
	form_driver (saveloadMOSAIC_form, REQ_VALIDATION);

	// get the field's data...
	char *aux = field_buffer (form_fields (saveloadMOSAIC_form)[0], 0);
	// ...and take the trailing spaces off
	int i;
	for (i = strlen (aux) - 1; aux[i] == ' '; i--);
	aux[i + 1] = '\0';

	// if it's an empty string, returns as canceled
	return aux[0] ? aux : NULL;
}
