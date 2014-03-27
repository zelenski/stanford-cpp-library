/*
 * File: simpio.h
 * --------------
 * This file exports a set of functions that simplify input/output
 * operations in C++ and provide some error-checking on console input.
 */

#ifndef _simpio_h
#define _simpio_h

#include <iostream>
#include <string>

/*
 * Function: getInteger
 * Usage: int n = getInteger(prompt);
 * ----------------------------------
 * Reads a complete line from <code>cin</code> and scans it as an
 * integer. If the scan succeeds, the integer value is returned. If
 * the argument is not a legal integer or if extraneous characters
 * (other than whitespace) appear in the string, the user is given
 * a chance to reenter the value.
 *
 * If supplied, the optional <code>prompt</code> string is printed before
 * reading the value.
 *
 * The also optional <code>reprompt</code> argument provides an output message
 * displayed each time if the user types a file that is not found.
 * If no value is passed, defaults to, "Illegal integer format. Try again.".
 */
int getInteger(std::string prompt = "", std::string reprompt = "");

/*
 * Function: getReal
 * Usage: double x = getReal(prompt);
 * ----------------------------------
 * Reads a complete line from <code>cin</code> and scans it as a
 * floating-point number. If the scan succeeds, the floating-point
 * value is returned.  If the input is not a legal number or if
 * extraneous characters (other than whitespace) appear in the string,
 * the user is given a chance to reenter the value.
 *
 * If supplied, the optional <code>prompt</code> string is printed before
 * reading the value.
 *
 * The also optional <code>reprompt</code> argument provides an output message
 * displayed each time if the user types a file that is not found.
 * If no value is passed, defaults to, "Illegal integer format. Try again.".
 */
double getReal(std::string prompt = "", std::string reprompt = "");

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

/*
 * Alternate version of getLine that accepts a prompt and fills a given output
 * variable with its result.
 */
void getLine(std::string prompt, std::string& out);

/*
 * Alternate version of getLine that accepts an input stream to read from.
 * Meant as a drop-in replacement for the standard C++ getline (lowercase L)
 * function.
 */
void getLine(std::istream& input, std::string& out);

/*
 * Reads a complete line from <code>cin</code> and treats it as a
 * yes-or-no answer to a question.  Returns <code>true</code> if the line
 * typed begins with a 'y' or 'Y', and returns <code>false</code> if it begins
 * with a 'n' or 'N'.  Otherwise the user is given a chance to reenter the
 * value.
 *
 * If supplied, the optional <code>prompt</code> string is printed before
 * reading the value.
 *
 * The also optional <code>reprompt</code> argument provides an output message
 * displayed each time if the user types a file that is not found.
 * If no value is passed, defaults to,
 * "Please type a word that starts with 'Y' or 'N'.".
 */
bool getYesOrNo(std::string prompt = "", std::string reprompt = "");

#endif
