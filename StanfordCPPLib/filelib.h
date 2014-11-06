/*
 * File: filelib.h
 * ---------------
 * This file exports a standardized set of tools for working with
 * files.  The library offers at least some portability across the
 * file systems used in the three supported platforms: Mac OSX,
 * Windows, and Linux.  Directory and search paths are allowed to
 * contain separators in any of the supported styles, which usually
 * makes it possible to use the same code on different platforms.
 * 
 * @version 2014/10/19
 * - alphabetized function declarations
 * - converted many funcs to take const string& rather than string for efficiency
 * - added listDirectory overload that returns a Vector
 */

#ifndef _filelib_h
#define _filelib_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "vector.h"

/*
 * Function: createDirectory
 * Usage: createDirectory(path);
 * -----------------------------
 * Creates a new directory for the specified path.  The
 * <code>createDirectory</code> function does not report an error if
 * the directory already exists.  Unlike <code>createDirectoryPath</code>,
 * <code>createDirectory</code> does not create missing directories
 * along the path.  If some component of <code>path</code> does
 * not exist, this function signals an error.
 */
void createDirectory(const std::string& path);

/*
 * Function: createDirectoryPath
 * Usage: createDirectoryPath(path);
 * ---------------------------------
 * Creates a new directory for the specified path.   If intermediate
 * components of <code>path</code> do not exist, this function creates
 * them as needed.
 */
void createDirectoryPath(const std::string& path);

/*
 * Function: defaultExtension
 * Usage: string newname = defaultExtension(filename, ext);
 * --------------------------------------------------------
 * Adds an extension to a file name if none already exists.  If the
 * <code>extension</code> argument begins with a leading <code>*</code>,
 * any existing extension in <code>filename</code> is replaced by
 * <code>ext</code>.
 */
std::string defaultExtension(const std::string& filename, const std::string& ext);

/*
 * Function: deleteFile
 * Usage: deleteFile(filename);
 * ----------------------------
 * Deletes the specified file.  Errors are reported by calling
 * <code>error</code>.
 */
void deleteFile(const std::string& filename);

/*
 * Function: expandPathname
 * Usage: string pathname = expandPathname(filename);
 * --------------------------------------------------
 * Expands a filename into a canonical name for the platform.
 */
std::string expandPathname(const std::string& filename);

/*
 * Function: fileExists
 * Usage: if (fileExists(filename)) ...
 * ------------------------------------
 * Returns <code>true</code> if the specified file exists.
 */

bool fileExists(const std::string& filename);

/*
 * Function: findOnPath
 * Usage: string pathname = findOnPath(path, filename);
 * ----------------------------------------------------
 * Returns the canonical name of a file found using a search path.
 * The <code>findOnPath</code> function is similar to
 * <code>openOnPath</code>, except that it doesn't actually
 * return an open stream.  If no matching file is found,
 * <code>findOnPath</code> returns the empty string.
 */
std::string findOnPath(const std::string& path, const std::string& filename);

/*
 * Function: getCurrentDirectory
 * Usage: string filename = getCurrentDirectory();
 * -----------------------------------------------
 * Returns an absolute filename for the current directory.
 */
std::string getCurrentDirectory();

/*
 * Function: getDirectoryPathSeparator
 * Usage: string sep = getDirectoryPathSeparator();
 * ------------------------------------------------
 * Returns the standard directory path separator used on this platform.
 */
std::string getDirectoryPathSeparator();

/*
 * Function: getExtension
 * Usage: ext = getExtension(filename);
 * ------------------------------------
 * Returns the extension of <code>filename</code>.  The extension
 * consists of the separating dot and all subsequent characters.
 * If no dot exists in the final component, <code>getExtension</code>
 * returns the empty string.  These semantics ensure that concatenating
 * the root and the extension always returns the original filename.
 */
std::string getExtension(const std::string& filename);

/*
 * Function: getHead
 * Usage: head = getHead(filename);
 * --------------------------------
 * Returns all but the last component of a path name.  The components
 * of the path name can be separated by any of the directory path
 * separators (forward or reverse slashes).  The special cases are
 * illustrated by the following examples:
 *
 *<pre>
 *    getHead("a/b")  = "a"     getTail("a/b")   = "b"
 *    getHead("a")    = ""      getTail("a")     = "a"
 *    getHead("/a")   = "/"     getTail("/a")    = "a"
 *    getHead("/")    = "/"     getTail("/")     = ""
 *</pre>
 */
std::string getHead(const std::string& filename);

/*
 * Function: getRoot
 * Usage: string root = getRoot(filename);
 * ---------------------------------------
 * Returns the root of <code>filename</code>.  The root consists
 * of everything in <code>filename</code> up to the last dot and
 * the subsequent extension.  If no dot appears in the final component
 * of the filename, <code>getRoot</code> returns the entire name.
 */
std::string getRoot(const std::string& filename);

/*
 * Function: getSearchPathSeparator
 * Usage: string sep = getSearchPathSeparator();
 * ---------------------------------------------
 * Returns the standard search path separator used on this platform.
 */
std::string getSearchPathSeparator();

/*
 * Function: getTail
 * Usage: tail = getTail(filename);
 * --------------------------------
 * Returns the last component of a path name.  The components of the
 * path name can be separated by any of the directory path separators
 * (forward or reverse slashes).  For details on the interpretation of
 * special cases, see the comments for the <code>getHead</code> function.
 */
std::string getTail(const std::string& filename);

/*
 * Function: getTempDirectory
 * Usage: string dir = getTempDirectory();
 * ---------------------------------------
 * Returns the operating system's "temp" folder.
 * Can be useful if your program needs to store temporary files.
 */
std::string getTempDirectory();

/*
 * Function: isDirectory
 * Usage: if (isDirectory(filename)) ...
 * -------------------------------------
 * Returns <code>true</code> if the specified file is a directory.
 */
bool isDirectory(const std::string& filename);

/*
 * Function: isFile
 * Usage: if (isFile(filename)) ...
 * --------------------------------
 * Returns <code>true</code> if the specified file is a regular file,
 * not a symbolic link or directory.
 */
bool isFile(const std::string& filename);

/*
 * Function: isSymbolicLink
 * Usage: if (isSymbolicLink(filename)) ...
 * ----------------------------------------
 * Returns <code>true</code> if the specified file is a symbolic link.
 */
bool isSymbolicLink(const std::string& filename);

/*
 * Function: listDirectory
 * Usage: listDirectory(path, list);
 * ---------------------------------
 * Adds an alphabetized list of the files in the specified directory
 * to the string vector <code>list</code>.  This list excludes the
 * names <code>.</code> and <code>..</code> entries.
 * The third version of the function returns a Vector rather than
 * accepting one from the caller as a reference parameter.
 */
void listDirectory(const std::string& path, Vector<std::string>& list);
void listDirectory(const std::string& path, std::vector<std::string>& list);
Vector<std::string> listDirectory(const std::string& path);

/*
 * Function: matchFilenamePattern
 * Usage: if (matchFilenamePattern(filename, pattern)) ...
 * -------------------------------------------------------
 * Determines whether the filename matches the specified pattern.  The
 * pattern string is interpreted in much the same way that a Unix shell
 * expands filenames and supports the following wildcard options:
 *
 *<pre>
 *    ?      Matches any single character
 *    *      Matches any sequence of characters
 *    [...]  Matches any of the specified characters
 *    [^...] Matches any character <i>except</i> the specified ones
 *</pre>
 *
 * The last two options allow a range of characters to be specified in the
 * form <code>a-z</code>.
 */
bool matchFilenamePattern(const std::string& filename, const std::string& pattern);

/*
 * Function: openFile
 * Usage: if (openFile(stream, filename)) ...
 * ------------------------------------------
 * Opens the filestream <code>stream</code> using the specified
 * filename.  This function is similar to the <code>open</code>
 * method of the stream classes, but uses a C++ <code>string</code>
 * object instead of the older C-style string.  If the operation
 * succeeds, <code>openFile</code> returns <code>true</code>;
 * if it fails, <code>openFile</code> sets the failure flag in the
 * stream and returns <code>false</code>.
 */
bool openFile(std::ifstream& stream, const std::string& filename);
bool openFile(std::ofstream& stream, const std::string& filename);

/*
 * Function: openFileDialog
 * Usage: string filename = openFileDialog(stream);
 *        string filename = openFileDialog(stream, title);
 *        string filename = openFileDialog(stream, title, path);
 * -------------------------------------------------------------
 * Opens a dialog that allows the user to choose the file.  The
 * <code>title</code> parameter is displayed in the dialog title.
 * The <code>path</code> parameter is used to set the working directory;
 * if <code>path</code> does not appear, <code>openFileDialog</code>
 * uses the current directory.
 */
std::string openFileDialog(std::ifstream& stream);
std::string openFileDialog(std::ifstream& stream,
                           const std::string& title);
std::string openFileDialog(std::ifstream& stream,
                           const std::string& title,
                           const std::string& path);
std::string openFileDialog(std::ofstream& stream);
std::string openFileDialog(std::ofstream& stream,
                           const std::string& title);
std::string openFileDialog(std::ofstream& stream,
                           const std::string& title,
                           const std::string& path);

/*
 * Function: openOnPath
 * Usage: string pathname = openOnPath(stream, path, filename);
 * ------------------------------------------------------------
 * Opens a file using a search path.  If <code>openOnPath</code>
 * is successful, it returns the first path name on the search path
 * for which <code>stream.open</code> succeeds.  The <code>path</code>
 * argument consists of a list of directories that are prepended to the
 * filename, unless <code>filename</code> begins with an absolute
 * directory marker, such as <code>/</code> or <code>~</code>.
 * The directories in the search path may be separated either
 * by colons (Unix or Mac OS) or semicolons (Windows).  If the file
 * cannot be opened, the failure bit is set in the <code>stream</code>
 * parameter, and the <code>openOnPath</code> function returns the
 * empty string.
 */
std::string openOnPath(std::ifstream& stream,
                       const std::string& path,
                       const std::string& filename);
std::string openOnPath(std::ofstream& stream,
                       const std::string& path,
                       const std::string& filename);

/*
 * Function: promptUserForFile
 * Usage: string filename = promptUserForFile(stream, prompt);
 * -----------------------------------------------------------
 * Asks the user for the name of a file.  The file is opened using
 * the reference parameter <code>stream</code>, and the function
 * returns the name of the file.  If the requested file cannot be
 * opened, the user is given additional chances to enter a valid file.
 *
 * The optional <code>prompt</code> argument provides an input prompt
 * for the user.
 *
 * The also optional <code>reprompt</code> argument provides an output message
 * displayed each time if the user types a file that is not found.
 * If no value is passed, defaults to, "Unable to open that file.  Try again.".
 */
std::string promptUserForFile(std::ifstream& stream,
                              const std::string& prompt = "",
                              const std::string& reprompt = "");
std::string promptUserForFile(std::ofstream& stream,
                              const std::string& prompt = "",
                              const std::string& reprompt = "");

/*
 * Function: readEntireFile
 * Usage: readEntireFile(is, lines);
 * ---------------------------------
 * Reads the entire contents of the specified input stream into the
 * string vector <code>lines</code>.  The client is responsible for
 * opening and closing the stream.  The vector can be either an STL
 * <code>vector</code> or a <code>Vector</code> as defined in the
 * Stanford C++ libraries.
 */
void readEntireFile(std::istream& is, Vector<std::string>& lines);
void readEntireFile(std::istream& is, std::vector<std::string>& lines);

/*
 * An overload of readEntireFile that just returns the whole file as a very
 * long single string, rather than a vector of lines.
 *
 * The first version returns the entire file's text.
 * It throws an error() if the file is not found or cannot be read.
 *
 * The second version fills an output reference with the text read.
 * Returns true if the read was successful and false if the file was not found
 * or unable to be opened for reading.
 */
std::string readEntireFile(const std::string& filename);
bool readEntireFile(const std::string& filename, std::string& out);

/*
 * Function: renameFile
 * Usage: renameFile(oldname, newname);
 * ------------------------------------
 * Renames a file.  Errors are reported by calling
 * <code>error</code> in the implementation.
 */
void renameFile(const std::string& oldname, const std::string& newname);

/*
 * Moves the given input stream back to its beginning, so that it can
 * be read again from start to finish.
 */
void rewindStream(std::istream& input);

/*
 * Function: setCurrentDirectory
 * Usage: setCurrentDirectory(filename);
 * -------------------------------------
 * Changes the current directory to the specified path.
 */
void setCurrentDirectory(const std::string& path);

/*
 * Opens the given file and writes the given text into it.
 * Normally this function replaces any previous contents of the file, but
 * if the optional 'append' parameter is passed, the given text is added
 * at the end of the file, retaining any previous contents.
 * Returns true if the write was successful and false if the file was unable
 * to be opened for writing.
 */
bool writeEntireFile(const std::string& filename,
                     const std::string& text,
                     bool append = false);

#include "private/main.h"

#endif
