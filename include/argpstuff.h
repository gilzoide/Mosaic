/** @file argpstuff.h
 * Do the argument parsing
 */
#ifndef ARGPSTUFF_H
#define ARGPSTUFF_H

#include "state.h"

/**
 * Parse command line options
 *
 * Ask the parser for the options, and enable them in the state
 *
 * @return the optional filename, for opening maae loading a file
 */
const char *arguments (int argc, char *argv[]);

#endif
