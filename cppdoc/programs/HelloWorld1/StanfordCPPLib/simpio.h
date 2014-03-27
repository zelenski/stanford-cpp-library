/*
 * File: simpio.h
 * --------------
 * This file exports a set of functions that simplify input/output
 * operations in C++ and provide some error-checking on console input.
 */

#ifndef _simpio_h
#define _simpio_h

#include <string>

/*
 * Function: getInteger
 * Usage: int n = getInteger(prompt);
 * ----------------------------------
 * Reads a complete line from <code>cin</code> and scans it as an
 * integer. If the scan succeeds, the integer value is returned. If
 * the argument is not a legal integer or if extraneous characters
 * (other than whitespace) appear in the string, the user is given
 * a chance to reenter the value. If supplied, the optional
 * <code>prompt</code> string is printed before reading the value.
 */

int getInteger(std::string prompt = "");

/*
 * Function: getReal
 * Usage: double x = getReal(prompt);
 * ----------------------------------
 * Reads a complete line from <code>cin</code> and scans it as a
 * floating-point number. If the scan succeeds, the floating-point
 * value is returned.  If the input is not a legal number or if
 * extraneous characters (other than whitespace) appear in the string,
 * the user is given a chance to reenter the value. If supplied, the
 * optional <code>prompt</code> string is printed before reading the value.
 */

double getReal(std::string prompt = "");

/*
 * Function: getLine
 * Usage: string line = getLine(prompt);
 * -------------------------------------
 * Reads a line of text from <code>cin</code> and returns that line
 * as a string.  The newline character that terminates the input is
 * not stored as part of the return value.  If supplied, the optional
 * <code>prompt</code> string is printed before reading the value.
 */

std::string getLine(std::string prompt = "");

#endif
