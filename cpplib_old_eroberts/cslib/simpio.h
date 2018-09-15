/*
 * File: simpio.h
 * --------------
 * This interface exports several functions that simplify the
 * reading of input data.
 */

#ifndef _simpio_h
#define _simpio_h

#include "cslib.h"

/*
 * Function: getInteger
 * Usage: n = getInteger();
 * ------------------------
 * Reads a line of text from standard input and scans it as an integer.
 * If an integer cannot be scanned or if extraneous characters follow
 * the number, the user is given a chance to retry.
 */

int getInteger(void);

/*
 * Function: getLong
 * Usage: l = getLong();
 * ---------------------
 * Reads a line of text from standard input and scans it as a
 * <code>long</code>.  If an integer cannot be scanned or if
 * extraneous characters follow the number, the user is given
 * a chance to retry.
 */

long getLong(void);

/*
 * Function: getReal
 * Usage: d = getReal();
 * ---------------------
 * Reads a line of text from standard input and scans it as a
 * <code>double</code>.  If an number cannot be scanned or if
 * extraneous characters follow the number, the user is given
 * a chance to retry.
 */

double getReal(void);

/*
 * Function: getLine
 * Usage: s = getLine();
 * ---------------------
 * Reads a line of text from standard input and returns the line
 * as a string.  The newline character that terminates the input
 * is not stored as part of the result.
 */

string getLine(void);

/*
 * Function: readLine
 * Usage: s = readLine(infile);
 * ----------------------------
 * Reads a line of text from the input file and returns the line
 * as a string.  The newline character that terminates the input
 * is not stored as part of the result.  The <code>readLine</code>
 * function returns <code>NULL</code> if <code>infile</code> is
 * at the end-of-file position.
 */

string readLine(FILE *infile);

/*
 * Function: readLinesFromStream
 * Usage: string *array = readLinesFromStream(infile);
 * ---------------------------------------------------
 * Reads an entire file into a <code>NULL</code>-terminated array of lines.
 * Opening and closing the file stream is the responsibility of the caller.
 */

string *readLinesFromStream(FILE *infile);

/*
 * Function: readLinesFromFile
 * Usage: string *array = readLinesFromFile(filename);
 * ---------------------------------------------------
 * Reads an entire file into a <code>NULL</code>-terminated array of lines.
 * This version opens the file, reads it, and closes it at the end.  If the
 * file name is <code>"-"</code>, the function reads from <code>stdin</code>.
 */

string *readLinesFromFile(string filename);

#endif
