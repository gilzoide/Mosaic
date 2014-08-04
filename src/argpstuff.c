#include "argpstuff.h"

/* ARGP for parsing the arguments */
#include <argp.h>

const char *argp_program_version = "Maae 0.6";
const char *argp_program_bug_address = "<gilzoide@gmail.com>";
static char doc[] = "Maae, a Curses and Mosaic based asc art editor";
static char args_doc[] = "[FILE]";

// our options
static struct argp_option options[] = {
	{"dimensions",  'd', 0, 0, "Show image dimensions"},
	{"color", 'c', 0, 0,  "Produce colored output" },
	{ 0 }
};

/* Used by arguments to communicate with parse_opt */
struct arguments {
	char *input;
	char dimensions, color;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
	struct arguments *argumentos = (struct arguments*) state->input;

	switch (key)
	{
		case 'c':
			argumentos->color = 1;
			break;
		case 'd':
			argumentos->dimensions = 1;
			break;

		case ARGP_KEY_ARG:
			argumentos->input = arg;
			break;

		case ARGP_KEY_END:
			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}


/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };


const char *arguments (int argc, char *argv[]) {
	struct arguments args;
	args.input = NULL;
	argp_parse (&argp, argc, argv, 0, 0, &args);

	return args.input;

}
