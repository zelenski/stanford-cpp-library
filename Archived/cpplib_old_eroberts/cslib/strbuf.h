/*
 * File: strbuf.h
 * --------------
 * This interface defines a class that allows strings to grow by
 * concatentation.
 */

#ifndef _strbuf_h
#define _strbuf_h

#include "cslib.h"

/*
 * Type: StringBuffer
 * ------------------
 * This type provides an efficient, memory-safe mechanism for strings
 * that grow by the addition of characters.
 */

typedef struct StringBufferCDT *StringBuffer;

/*
 * Function: newStringBuffer
 * Usage: sb = newStringBuffer();
 * ------------------------------
 * Creates a new string buffer that expands dynamically if needed.
 * The buffer is initially empty.
 */

StringBuffer newStringBuffer();

/*
 * Function: freeStringBuffer
 * Usage: freeStringBuffer(sb);
 * ----------------------------
 * Frees the storage associated with the string buffer.
 */

void freeStringBuffer(StringBuffer sb);

/*
 * Function: pushChar
 * Usage: pushChar(sb, ch);
 * ------------------------
 * Appends (pushes) the character <code>ch</code> onto the end of the
 * string buffer.
 */

void pushChar(StringBuffer sb, char ch);

/*
 * Function: popChar
 * Usage: ch = popChar(sb);
 * ------------------------
 * Pops and removes the last character from the string buffer.
 */

char popChar(StringBuffer sb);

/*
 * Function: appendString
 * Usage: appendString(sb, str);
 * -----------------------------
 * Appends the string <code>str</code> to the end of the string buffer.
 */

void appendString(StringBuffer sb, string str);

/*
 * Function: sbprintf
 * Usage: sbprintf(sb, format, ...);
 * ---------------------------------
 * Expands a <code>printf</code>-style format string and arguments onto
 * the end of the string buffer.
 */

void sbprintf(StringBuffer sb, string format, ...);

/*
 * Function: isEmpty
 * Usage: if (isEmpty(sb)) . . .
 * -----------------------------
 * Returns <code>true</code> if the string buffer is empty.
 */

bool isEmptyStringBuffer(StringBuffer vec);

/*
 * Function: size
 * Usage: n = size(sb);
 * --------------------
 * Returns the number of characters in the string buffer.
 */

int sizeStringBuffer(StringBuffer vector);

/*
 * Function: clear
 * Usage: clear(sb);
 * -----------------
 * Removes all characters from the string buffer.
 */

void clearStringBuffer(StringBuffer sb);

/*
 * Function: getString
 * Usage: str = getString(sb);
 * ---------------------------
 * Returns the string stored inside the buffer.  Clients must copy this
 * string if they need to retain it.
 */

string getString(StringBuffer sb);

/*
 * Friend function: printfCapacity
 * Usage: capacity = printfCapacity(format, args);
 * -----------------------------------------------
 * Returns a character count that will be sufficient to hold the result
 * of printing <code>format</code> with arguments substituted from the
 * <code>args</code> list.  This bound is guaranteed to be adequate but
 * is not necessarily tight.
 */

int printfCapacity(string format, va_list args);

/*
 * Friend function: sbFormat
 * Usage: sbFormat(sb, capacity, format, list);
 * --------------------------------------------
 * Works like <code>sbprintf</code> except that the list is an argument
 * list as in <code>stdarg.h</code>.  The <code>capacity</code> argument
 * is the required capacity, as returned by <code>printfCapacity</code>.
 */

void sbFormat(StringBuffer sb, int capacity, string format, va_list args);

#endif
