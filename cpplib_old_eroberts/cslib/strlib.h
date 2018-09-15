/*
 * File: strlib.h
 * --------------
 * This interface defines a general library for dynamically allocated
 * strings.  The major differences between traditional C strings and
 * those defined using this interface are:
 *
 *<ol>
 *  <li>The <code>strlib.h</code> interface takes care of memory
 *      allocation, ensuring that there is sufficient space to hold
 *      the result of each string operation.
 *  <li>Clients of the <code>strlib.h</code> interface are expected
 *      to treat all strings as immutable and refrain from writing
 *      into the character array.
 *</ol>
 */

/*
 * Cautionary note:
 * ----------------
 * Although this interface provides an extremely convenient abstraction
 * for working with strings, it is not appropriate for all applications.
 * In this interface, the functions that return string values (such as
 * concat and substring) do so by allocating new memory.  Over time, a
 * program that uses this package will consume increasing amounts of
 * memory and eventually exhaust the available supply.  If you are
 * writing a program that runs for a short time and stops, the fact
 * that the package consumes memory is not a problem.  If, however,
 * you are writing an application that must run for an extended period
 * of time, using this package requires that you make some provision
 * for freeing any allocated storage.
 */

#ifndef _strlib_h
#define _strlib_h

#include <stdarg.h>
#include "cslib.h"
#include "generic.h"

/* Section 1 -- Basic string operations */

/*
 * Function: concat
 * Usage: s = concat(s1, s2);
 * --------------------------
 * Concatenates two strings by joining them end to end.  For
 * example, <code>concat("ABC", "DE")</code> returns the
 * string <code>"ABCDE"</code>.
 */

string concat(string s1, string s2);

/*
 * Function: charAt
 * Usage: ch = charAt(s, i);
 * -------------------------
 * Returns the character at position <code>i</code> in the
 * string <code>s</code>.  This function is included in the
 * library to make the type <code>string</code> a true abstract
 * type in the sense that all of the necessary operations can be
 * invoked using functions.  Calling <code>charAt(s, i)</code>
 * is like selecting <code>s[i]</code>, except that
 * <code>charAt</code> checks to see if <code>i</code> is
 * within the range of legal index positions, which extend
 * from 0 to <code>stringLength(s)</code>.  Calling
 * <code>charAt(s, stringLength(s))</code returns the null
 * character at the end of the string.
 */

char charAt(string s, int i);

/*
 * Function: substring
 * Usage: t = substring(s, p1, p2);
 * --------------------------------
 * Returns a copy of the substring of <code>s</code> consisting of the
 * characters between index positions <code>p1</code> and <code>p2</code>,
 * inclusive.  The following special cases apply:
 *
 *<ol>
 *  <li> If <code>p1</code> is less than 0, it is assumed to be 0.
 *  <li> If <code>p2</code> is greater than or equal to the length of the
 *       string, <code>p2</code> is set to <code>stringLength(s) - 1</code>.
 *  <li> If <code>p2 &lt; p1</code>, <code>substring</code> returns the
 *       empty string.
 *</ol>
 */

string substring(string s, int p1, int p2);

/*
 * Function: charToString
 * Usage: s = charToString(ch);
 * ----------------------------
 * Takes a single character and returns a one-character string
 * consisting of that character.  The <code>charToString</code>
 * function is useful, for example, if you need to concatenate
 * a string and a character.  Since <code>concat</code> requires
 * two strings, you must first convert the character into a string.
 */

string charToString(char ch);

/*
 * Function: stringLength
 * Usage: len = stringLength(s);
 * -----------------------------
 * Returns the length of the string <code>s</code>.
 */

int stringLength(string s);

/*
 * Function: copyString
 * Usage: newstr = copyString(s);
 * ------------------------------
 * Copies the string <code>s</code> into dynamically allocated storage
 * and returns the new string.  This function is not ordinarily required
 * when this package is used on its own but is often necessary when you
 * are working with more than one string package.
 */

string copyString(string s);

/* Section 2 -- String comparison functions */

/*
 * Function: stringEqual
 * Usage: if (stringEqual(s1, s2)) ...
 * -----------------------------------
 * Returns <code>true</code> if the strings <code>s1</code>
 * and <code>s2</code> are equal.  For the strings to be
 * considered equal, every character in one string must
 * precisely match the corresponding character in the other.
 * Uppercase and lowercase characters are considered to be
 * different.
 */

bool stringEqual(string s1, string s2);

/*
 * Function: stringEqualIgnoreCase
 * Usage: if (stringEqualIgnoreCase(s1, s2)) ...
 * ---------------------------------------------
 * Returns <code>true</code> if the strings <code>s1</code>
 * and <code>s2</code> are equal, ignoring differences in case.
 */

bool stringEqualIgnoreCase(string s1, string s2);

/*
 * Function: stringCompare
 * Usage: if (stringCompare(s1, s2) < 0) ...
 * -----------------------------------------
 * Returns -1 if string <code>s1</code> comes before <code>s2</code>
 * in lexicographic order, 0 if they are equal, and +1 if
 * <code>s1</code> comes after <code>s2</code>.
 */

int stringCompare(string s1, string s2);

/*
 * Function: startsWith
 * Usage: if (startsWith(s1, s2)) ...
 * ----------------------------------
 * Returns <code>true</code> if <code>s1</code> starts with <code>s2</code>.
 */

bool startsWith(string s1, string s2);

/*
 * Function: endsWith
 * Usage: if (endsWith(s1, s2)) ...
 * --------------------------------
 * Returns <code>true</code> if <code>s1</code> ends with <code>s2</code>.
 */

bool endsWith(string s1, string s2);

/* Section 3 -- Search functions */

/*
 * Function: findChar
 * Usage: p = findChar(ch, text, start);
 * -------------------------------------
 * Searches for the character <code>ch</code> beginning at position
 * <code>start</code> in the string <code>text</code> and returns
 * the first index at which it appears or -1 if no match is found.
 */

int findChar(char ch, string text, int start);

/*
 * Function: findString
 * Usage: p = findString(str, text, start);
 * ----------------------------------------
 * Searches for the string <code>str</code> beginning at position
 * <code>start</code> in the string <code>text</code> and returns
 * the first index at which it appears or -1 if no match is found.
 */

int findString(string str, string text, int start);

/*
 * Function: findLastChar
 * Usage: p = findLastChar(ch, text);
 * ----------------------------------
 * Returns the last index of <code>ch</code> in <code>text</code>,
 * or -1 if the search value does not appear.
 */

int findLastChar(char ch, string text);

/*
 * Function: findLastString
 * Usage: p = findLastString(str, text);
 * -------------------------------------
 * Returns the last index of <code>str</code> in <code>text</code>,
 * or -1 if the search value does not appear.
 */

int findLastString(string str, string text);

/* Section 4 -- Conversion functions */

/*
 * Function: toLowerCase
 * Usage: s = toLowerCase(s);
 * --------------------------
 * Returns a new string with all alphabetic characters converted
 * to lowercase.
 */

string toLowerCase(string s);

/*
 * Function: toUpperCase
 * Usage: s = toUpperCase(s);
 * --------------------------
 * Returns a new string with all alphabetic characters converted
 * to uppercase.
 */

string toUpperCase(string s);

/*
 * Function: integerToString
 * Usage: s = integerToString(n);
 * ------------------------------
 * Converts an integer into the corresponding string of digits.
 * For example, <code>integerToString(123)</code> returns
 * <code>"123"</code> as a string.
 */

string integerToString(int n);

/*
 * Function: stringToInteger
 * Usage: n = stringToInteger(s);
 * ------------------------------
 * Converts a string of digits into an integer.  If the string
 * is not a legal integer or contains extraneous characters,
 * <code>stringToInteger</code> signals an error condition.
 */

int stringToInteger(string s);

/*
 * Function: realToString
 * Usage: string  s = realToString(d);
 * -----------------------------------
 * Converts a floating-point number into the corresponding string
 * form.  For example, calling <code>realToString(23.45)</code>
 * returns </code>"23.45"</code>.  The conversion is the same as
 * that used for <code>"%G"</code> format in <code>printf</code>.
 */

string realToString(double d);

/*
 * Function: stringToReal
 * Usage: d = stringToReal(s);
 * ---------------------------
 * Converts a string representing a real number into its
 * corresponding value.  If the string is not a legal
 * floating-point number or if it contains extraneous characters,
 * <code>stringToReal</code> signals an error condition.
 */

double stringToReal(string s);

/* Section 5 - Miscellaneous functions */

/*
 * Function: trim
 * Usage: trimmed = trim(str);
 * ---------------------------
 * Returns a new string after removing any whitespace characters
 * from the beginning and end of the argument.
 */

string trim(string str);

/*
 * Function: quoteString
 * Usage: quoted = quoteString(str);
 * ---------------------------------
 * Returns a quoted string that can be read by the C parser.
 * This string includes double quotes around the entire value,
 * and uses standard escape sequences to indicate special
 * characters.
 */

string quoteString(string str);

/*
 * Function: quoteHTML
 * Usage: quoted = quoteHTML(str);
 * -------------------------------
 * Returns a string that appears correctly in HTML by changing HTML special
 * characters to character entities.
 */

string quoteHTML(string str);

/*
 * Function: stringArrayLength
 * Usage: len = stringArrayLength(array);
 * --------------------------------------
 * Returns the length of a <code>NULL</code>-terminated string array.
 */

int stringArrayLength(string array[]);

/*
 * Function: searchStringArray
 * Usage: index = searchStringArray(str, array);
 * ---------------------------------------------
 * Finds a string in a <code>NULL</code>-terminated string array and
 * returns the first index at which the string appears, or -1 if it
 * is not found.
 */

int searchStringArray(string str, string array[]);

#endif
