#ifndef ARGPSTUFF_H
#define ARGPSTUFF_H

#include "state.h"

/**
 * Ask the parser for the options, enable them
 * in the state and return the optional filename,
 * for opening maae loading a file
 */
const char *arguments (int argc, char *argv[]);

#endif
