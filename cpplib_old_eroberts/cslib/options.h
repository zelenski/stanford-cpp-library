/*
 * File: options.h
 * ---------------
 * This interface defines a set of options that can be scanned on
 * the command line.
 */

#ifndef _cmdscan_h
#define _cmdscan_h

#include "cslib.h"
#include "hashmap.h"

/*
 * Function: parseOptions
 * ----------------------
 * Parses shell arguments into a table of options and a list of
 * unprocessed values.  The general pattern of use is
 *
 *<pre>
 *    options = parseOptions(args, optionSpec);
 *</pre>
 *
 * where <code>args</code> is a <code>NULL</code>-terminated list of
 * strings and <code>optionSpec</code> is a <code>NULL</code>-terminated
 * array of strings (typically initialized at compile time)
 * indicating what options are legal.  The return value is a
 * <code>HashMap</code> that contains both the bindings of the option
 * names (including the leading minus sign) and a special entry under
 * the name <code>args</code> that is a <code>NULL</code>-terminated
 * array of the remaining arguments.
 *
 * <p>Each element of <code>optionSpec</code> is a string that
 * begins with the option name and then includes an additional
 * field to specify the value that appears in the next argument
 * position.  The standard fields are:
 *
 *<p><table>
 * <tr><td><code>&lt;int&gt;</code></td>
 *     <td>An integer value</td></tr>
 * <tr><td><code>&lt;double&gt;</code></td>
 *     <td>A floating-point value</td></tr>
 * <tr><td><code>&lt;string&gt;</code></td>
 *     <td>An arbitrary string</td></tr>
 * <tr><td><code>&lt;char&gt;</code></td>
 *     <td>A single character</td></tr>
 * <tr><td><code>&lt;bool&gt;</code></td>
 *     <td>A boolean value</td></tr>
 * <tr><td><code>x|y|z</code></td>
 *     <td>One of the specified options</td></tr>
 *</table></p>
 *
 * Any argument class that is not defined in the above list is treated
 * as a string, and the client is responsible for checking that the
 * supplied value is valid.
 */

HashMap parseOptions(string args[], string optionSpec[]);

/*
 * Function: parseShellArgs
 * ------------------------
 * Parses a string into individual arguments in the style of the
 * various implementations of the Unix shell.  Argument tokens
 * are separated by whitespace.  This implementation recognizes
 * the quoting conventions of a single backslash (which quotes
 * the next character) and either single or double quotation marks,
 * which quote the intervening characters.  No other metacharacters
 * are recognized.
 */

string *parseShellArgs(string line);

/*
 * Function: getOption
 * -------------------
 * Gets an option value from the table, supplying a default value
 * if the option is undefined.
 */

string getOption(HashMap options, string key, string def);

/*
 * Function: getArgList
 * --------------------
 * Gets the list of arguments that were not interpreted by the option
 * parser.
 */

string *getArgList(HashMap options);

/*
 * Function: getIntOption
 * ----------------------
 * Gets an integer option from the table, supplying a default value
 * if the option is undefined.
 */

int getIntOption(HashMap options, string key, int def);

/*
 * Function: getDoubleOption
 * -------------------------
 * Gets a numeric option from the table, supplying a default value
 * if the option is undefined.
 */

double getDoubleOption(HashMap options, string key, double def);

/*
 * Function: getCharOption
 * -----------------------
 * Gets a character option from the table, supplying a default value
 * if the option is undefined.
 */

int getCharOption(HashMap options, string key, char def);

/*
 * Function: getBoolOption
 * -----------------------
 * Gets a Boolean option from the table, supplying a default value
 * if the option is undefined.
 */

bool getBoolOption(HashMap options, string key, bool def);

/*
 * Function: getUnitsOption
 * ------------------------
 * Gets a units value from an option table.  The value for the units
 * option consists of a number followed by a unit specification, which
 * can be any of the following: <code>in</code> (or <code>i</code>),
 * <code>cm</code>, <code>pt</code>, or <code>px</code>.
 */

double getUnitsOption(HashMap options, string key, double def);

/*
 * Function: getColorOption
 * ------------------------
 * Gets a color value from an option table, which is returned here as
 * a PostScript string that sets the color to the desired value.  The
 * supplied argument can be any of the following:
 *
 * <table>
 *   <tr>
 *     <td valign=top><i>name</i></td><td>&nbsp;&nbsp;</td>
 *     <td>A standard color name</td>
 *   </tr>
 *   <tr>
 *     <td valign=top><code>rrggbb</code></td><td>&nbsp;&nbsp;</td>
 *     <td>A hexadecimal RGB value</td>
 *   </tr>
 *   <tr>
 *     <td valign=top><code>0xrrggbb</code></td><td>&nbsp;&nbsp;</td>
 *     <td>A hexadecimal RGB value using C style</td>
 *   </tr>
 *   <tr>
 *     <td valign=top><code>#rrggbb</code></td><td>&nbsp;&nbsp;</td>
 *     <td>A hexadecimal RGB value using HTML style</td>
 *   </tr>
 *   <tr>
 *     <td valign=top><i>grayscale</i></td><td>&nbsp;&nbsp;</td>
 *     <td>A floating-point value between 0 and 1 indicating grayscale</td>
 *   </tr>
 *   <tr>
 *     <td valign=top><i>screen</i><code>%</code></td><td>&nbsp;&nbsp;</td>
 *     <td>A percentage value of the second-color screen</td>
 *   </tr>
 * </table>
 */

string getColorOption(HashMap options, string key, string def);

/*
 * Function: showUsage
 * -------------------
 * Displays a message describing the command line and its options.
 */

void showUsage(string usage, string spec[]);

#endif
