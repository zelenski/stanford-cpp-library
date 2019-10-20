/*
 * File: filelib.h
 * ---------------
 * This interface exports a standardized set of tools for working with
 * files across various platforms.  It is implemented for each of the
 * following platforms: Mac OSX, Windows, and Linux.  Directory and
 * search paths are allowed to contain separators in any of the
 * supported styles, which often makes it possible to use the same
 * code on different platforms.
 */

#ifndef _filelib_h
#define _filelib_h

#include "cslib.h"
#include "iterator.h"

/* Exported entries */

/*
 * Function: getDirectoryPathSeparator
 * Usage: sep = getDirectoryPathSeparator();
 * -----------------------------------------
 * Returns the standard directory path separator used on this platform.
 */

string getDirectoryPathSeparator(void);

/*
 * Function: getSearchPathSeparator
 * Usage: sep = getSearchPathSeparator();
 * --------------------------------------
 * Returns the standard search path separator used on this platform.
 */

string getSearchPathSeparator(void);

/*
 * Function: getRoot
 * Usage: root = getRoot(filename);
 * --------------------------------
 * Returns the root of a <code>filename.ext</code> pair, which
 * consists of everything in <code>filename</code> up to the last dot and
 * the subsequent extension.  If no dot appears, <code>getRoot</code>
 * returns the entire name.
 */

string getRoot(string filename);

/*
 * Function: getExtension
 * Usage: ext = getExtension(filename);
 * ------------------------------------
 * Returns the extension component of <code>filename</code>, which consists
 * of the rightmost dot in the final component and all subsequent characters.
 * If no dot exists in the final component, <code>getExtension</code>
 * returns the empty string.  Note that this interpretation is different
 * from the one used by the <code>:e</code> substitution in the shell.
 * These semantics were chosen so that the expression
 *
 *<pre>
 *    concat(getRoot(filename), getExtension(filename))
 *</pre>
 *
 * is equal to the original <code>filename</code>.
 */

string getExtension(string filename);

/*
 * Function: getHead
 * Usage: head = getHead(pathname);
 * --------------------------------
 * Returns all but the last component of a path name, where the
 * components are names separated by any of the directory path
 * separators (forward or reverse slashes).  The special cases are
 * illustrated by the following examples:
 *
 *<pre>
 *    getHead("a/b")  = "a"
 *    getHead("a")    = ""
 *    getHead("/a")   = "/"
 *    getHead("/")    = "/"
 *</pre>
 */

string getHead(string pathname);

/*
 * Function: getTail
 * Usage: tail = getTail(pathname);
 * --------------------------------
 * Returns the last component of a path name, where the components
 * are names separated by any of the directory path separators
 * (forward or reverse slashes).  The special cases are illustrated
 * by the following examples:
 *
 *<pre>
 *    getTail("a/b")   = "b"
 *    getTail("a")     = "a"
 *    getTail("/a")    = "a"
 *    getTail("/")     = ""
 *</pre>
 */

string getTail(string pathname);

/*
 * Function: defaultExtension
 * Usage: newname = defaultExtension(filename, ext);
 * -------------------------------------------------
 * Adds an extension to a file name if none already exists.  If the
 * <code>extension</code> argument begins with a leading <code>*</code>,
 * any existing extension in <code>filename</code> is replaced by
 * <code>ext</code>.
 */

string defaultExtension(string filename, string ext);

/*
 * Function: openOnPath
 * Usage: FILE *file = openOnPath(path, filename, mode);
 * -----------------------------------------------------
 * Opens a files by searching directories in a search path for the
 * first matching file.  The <code>openOnPath</code> routine has
 * the same structure as <code>fopen</code> in the standard library
 * and the filename and mode arguments are the same as in that
 * call.  The path argument consists of a list of directories
 * which are prepended to the filename, unless the filename begins
 * with an absolute directory marker, such as <code>/</code> or
 * <code>~</code>.  The directories in the search path may be
 * separated either by colons (Unix style) or semicolons (Windows
 * style).  The <code>openOnPath</code> function returns an open
 * stream to the indicated file, or <code>NULL</code>, if no
 * existing file is found.
 */

FILE *openOnPath(string path, string filename, string mode);

/*
 * Function: findOnPath
 * Usage: pathname = findOnPath(path, filename);
 * ---------------------------------------------
 * Returns a canonical name of a file found using a search path.
 * The <code>findOnPath</code> function is similar to
 * <code>openOnPath</code>, except that it returns the name of
 * the file instead of trying to open in.  If no matching file
 * is found, findOnPath returns <code>NULL</code>.
 */

string findOnPath(string path, string filename);

/*
 * Function: deleteFile
 * Usage: deleteFile(filename);
 * ----------------------------
 * Deletes the specified file.  Errors are reported by calling
 * <code>error</code> in the implementation.
 */

void deleteFile(string filename);

/*
 * Function: renameFile
 * Usage: renameFile(oldname, newname);
 * ------------------------------------
 * Renames a file.  Errors are reported by calling
 * <code>error</code> in the implementation.
 */

void renameFile(string oldname, string newname);

/*
 * Function: fileExists
 * Usage: if (fileExists(pathname)) . . .
 * --------------------------------------
 * Returns <code>true</code> if the specified file exists.
 */

bool fileExists(string pathname);

/*
 * Function: isFile
 * Usage: if (isFile(pathname)) . . .
 * ----------------------------------
 * Returns <code>true</code> if the specified file is a regular file.
 */

bool isFile(string pathname);

/*
 * Function: isSymbolicLink
 * Usage: if (isSymbolicLink(pathname)) . . .
 * ------------------------------------------
 * Returns <code>true</code> if the specified file is a symbolic link.
 */

bool isSymbolicLink(string pathname);

/*
 * Function: isDirectory
 * Usage: if (isDirectory(pathname)) . . .
 * ---------------------------------------
 * Returns <code>true</code> if the specified file is a directory.
 */

bool isDirectory(string pathname);

/*
 * Function: createDirectory
 * Usage: createDirectory(pathname);
 * ---------------------------------
 * Creates a new directory for the specified pathname.  The
 * <code>createDirectory</code> function does not report an error if
 * the directory already exists.  Unlike <code>createDirectoryPath</code>,
 * <code>createDirectory</code> does not create missing directories
 * along the path.  If some component of <code>pathname</code> does
 * not exist, this function signals an error.
 */

void createDirectory(string pathname);

/*
 * Function: createDirectoryPath
 * Usage: createDirectoryPath(pathname);
 * -------------------------------------
 * Creates a new directory for the specified pathname.   If intermediate
 * components of the pathname do not exist, this function creates them
 * as needed.
 */

void createDirectoryPath(string pathname);

/*
 * Function: setCurrentDirectory
 * Usage: setCurrentDirectory(pathname);
 * -------------------------------------
 * Changes the current directory to the specified path.
 */

void setCurrentDirectory(string path);

/*
 * Function: getCurrentDirectory
 * Usage: pathname = getCurrentDirectory();
 * ----------------------------------------
 * Returns an absolute pathname for the current directory.
 */

string getCurrentDirectory(void);

/*
 * Function: expandPathname
 * Usage: fullname = expandPathname(pathname);
 * -------------------------------------------
 * Expands a pathname into a canonical name for the platform.
 */

string expandPathname(string filename);

/*
 * Function: listDirectory
 * Usage: string array[] = listDirectory(dir);
 * -------------------------------------------
 * Returns an alphabetical list of all the files in the specified
 * directory, excluding the unix <code>.</code> and <code>..</code>
 * entries.  The array is terminated with a <code>NULL</code> entry.
 */

string *listDirectory(string dir);

/*
 * Function: newDirectoryIterator
 * Usage: foreach (name in newDirectoryIterator(dir)) . . .
 * --------------------------------------------------------
 * Creates a new directory iterator that will step through the files
 * in the specified directory.
 */

Iterator newDirectoryIterator(string dir);

/*
 * Function: newDirectoryTreeIterator
 * Usage: foreach (name in newDirectoryTreeIterator(dir)) . . .
 * ------------------------------------------------------------
 * Creates a new directory iterator that will walk recursively through the
 * files in the specified directory and its subdirectories.
 */

Iterator newDirectoryTreeIterator(string dir);

/*
 * Function: matchFilenamePattern
 * Usage: if (matchFilenamePattern(filename, pattern)) . . .
 * ---------------------------------------------------------
 * Determines whether the filename matches the specified pattern.  The
 * pattern string is interpreted in much the same way that a Unix shell
 * expands filenames and supports the following wildcard options:
 *
 *<p><table>
 * <tr><td align=center><code>?</code></td>
 *     <td>Matches any single character</td></tr>
 * <tr><td align=center><code>*</code></td>
 *     <td>Matches any sequence of characters</td></tr>
 * <tr><td align=center><code>[...]</code></td>
 *     <td>Matches any of the specified characters</td></tr>
 * <tr><td align=center><code>[^...]</code></td>
 *     <td>Matches any character <i>except</i> the specified ones</td></tr>
 *</table><p>
 *
 * The last two options allow a range of characters to be specified in the
 * form <code>a-z</code>.
 */

bool matchFilenamePattern(string filename, string pattern);

#endif
