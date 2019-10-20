/*
 * File: filelib.c
 * Last modified on Thu Dec 13 08:42:06 2012 by eroberts
 * -----------------------------------------------------
 * Implements the common parts of the filelib.h interface.
 * Platform-specific components of the implementation are
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "cmpfn.h"
#include "cslib.h"
#include "exception.h"
#include "filelib.h"
#include "generic.h"
#include "iterator.h"
#include "itertype.h"
#include "strlib.h"
#include "unittest.h"

/* Define a function type for the mapPath procedure. */

typedef void *(*PathMapFn)(string name, void *data);

/* Private function prototypes */

static void mapDirectoryTree(string path, Iterator iterator);
static void *mapPath(PathMapFn fn, string path, string filename, void *data);
static void *checkExists(string name, void *data);
static void *openFile(string name, void *data);
static bool isAbsolutePath(string pathname);
static bool recursiveMatch(string str, int sx, string pattern, int px);

/* Implementation section */

string getRoot(string filename) {
   register char *cp;
   char *dotpos;

   dotpos = NULL;
   for (cp = filename; *cp; cp++) {
      if (*cp == '.') dotpos = cp;
      if (*cp == '/' || *cp == '\\') dotpos = NULL;
   }
   if (dotpos == NULL) {
      return copyString(filename);
   } else {
      return substring(filename, 0, (dotpos - filename) - 1);
   }
}

string getExtension(string filename) {
   register char *cp;
   char *dotpos;

   dotpos = NULL;
   for (cp = filename; *cp; cp++) {
      if (*cp == '.') dotpos = cp;
      if (*cp == '/' || *cp == '\\') dotpos = NULL;
   }
   if (dotpos == NULL) {
      return "";
   } else {
      return copyString(dotpos);
   }
}

string getHead(string pathname) {
   register char *cp;
   char *slashpos;

   slashpos = NULL;
   for (cp = pathname; *cp; cp++) {
      if (*cp == '/' || *cp == '\\') slashpos = cp;
   }
   if (slashpos == NULL) {
      return "";
   } else if (slashpos == pathname) {
      return substring(pathname, 0, 0);
   } else {
      return substring(pathname, 0, (slashpos - pathname) - 1);
   }
}

string getTail(string pathname) {
   register char *cp;
   char *slashpos;

   slashpos = NULL;
   for (cp = pathname; *cp; cp++) {
      if (*cp == '/' || *cp == '\\') slashpos = cp;
   }
   if (slashpos == NULL) {
      return copyString(pathname);
   } else {
      return copyString(slashpos + 1);
   }
}

string defaultExtension(string filename, string ext) {
   register char *cp;
   char *dotpos;
   bool force;

   force = (ext[0] == '*');
   if (force) ext++;
   dotpos = NULL;
   for (cp = filename; *cp; cp++) {
      if (*cp == '.') dotpos = cp;
      if (*cp == '/' || *cp == '\\') dotpos = NULL;
   }
   if (dotpos == NULL) {
      force = true;
      dotpos = cp;
   }
   if (force) {
      return concat(substring(filename, 0, dotpos - filename - 1), ext);
   } else {
      return copyString(filename);
   }
}

FILE *openOnPath(string path, string filename, string mode) {
   return (FILE *) mapPath(openFile, path, filename, mode);
}

string findOnPath(string path, string filename) {
   return (string) mapPath(checkExists, path, filename, NULL);
}

void createDirectoryPath(string path) {
   string prefix;
   int cp;

   cp = 1;
   if (stringEqual(path, "")) return;
   while ((cp = findChar('/', path, cp + 1)) > 0) {
      prefix = substring(path, 0, cp - 1);
      createDirectory(prefix);
   }
   createDirectory(path);
}

Iterator newDirectoryIterator(string dir) {
   Iterator iterator;
   string *names;
   int i;

   iterator = newListIterator(sizeof (string), NULL);
   names = listDirectory(dir);
   for (i = 0; names[i] != NULL; i++) {
      addToIteratorList(iterator, &names[i]);
   }
   return iterator;
}

Iterator newDirectoryTreeIterator(string dir) {
   Iterator iterator;

   iterator = newListIterator(sizeof (string), NULL);
   mapDirectoryTree(dir, iterator);
   return iterator;
}

bool matchFilenamePattern(string filename, string pattern) {
   return recursiveMatch(filename, 0, pattern, 0);
}

/* Private functions */

static void mapDirectoryTree(string dir, Iterator iterator) {
   string *names, pathname;
   int i;

   names = listDirectory(dir);
   for (i = 0; names[i] != NULL; i++) {
      if (dir == NULL) {
         pathname = names[i];
      } else {
         pathname = dir;
         if (!endsWith(pathname, "/")) {
            pathname = concat(pathname, "/");
         }
         pathname = concat(pathname, names[i]);
      }
      if (isDirectory(pathname)) {
         mapDirectoryTree(pathname, iterator);
      } else {
         addToIteratorList(iterator, &pathname);
      }
   }
}

static void *mapPath(PathMapFn fn, string path, string filename, void *data) {
   char *p1, *p2;
   string localpath, tempname;
   void *result;

   if (filename == NULL) filename = "";
   if (path == NULL || path[0] == '\0' || isAbsolutePath(filename)) {
      return fn(expandPathname(filename), data);
   }
   localpath = concat(path, ":");
   result = NULL;
   p1 = localpath;
   while (result == NULL) {
      for (p2 = p1; (*p2 != '\0') && (*p2 != ':') && (*p2 != ';') ; p2++) {
         /* Empty */
      }
      if (*p2 == '\0') break;
      *p2 = '\0';
      if (p1 != p2 || strlen(localpath) == 1) {
         tempname = concat(p1, concat("/", filename));
         result = fn(expandPathname(tempname), data);
      }
      p1 = p2 + 1;
   }
   return result;
}

static void *checkExists(string name, void *data) {
   return (fileExists(name)) ? name : NULL;
}

static void *openFile(string name, void *data) {
   return fopen(name, (string) data);
}

static bool isAbsolutePath(string pathname) {
   if (*pathname == '/' || *pathname == '~') return true;
   if (strlen(pathname) > 1 && isalpha(pathname[0]) && pathname[1] == ':') {
      return true;
   }
   return false;
}

static bool recursiveMatch(string str, int sx, string pattern, int px) {
   int slen, plen, i;
   char pch, sch;
   bool match, invert;

   slen = stringLength(str);
   plen = stringLength(pattern);
   if (px == plen) return (sx == slen);
   pch = pattern[px];
   if (pch == '*') {
      for (i = sx; i <= slen; i++) {
         if (recursiveMatch(str, i, pattern, px + 1)) return true;
      }
      return false;
   }
   if (sx == slen) return false;
   sch = str[sx];
   if (pch == '[') {
      match = false;
      invert = false;
      px++;
      if (px == plen) {
         error("matchFilenamePattern: missing ]");
      }
      if (pattern[px] == '^') {
         px++;
         invert = true;
      }
      while (px < plen && pattern[px] != ']') {
         if (px + 2 < plen && pattern[px + 1] == '-') {
            match |= (sch >= pattern[px] && sch <= pattern[px + 2]);
            px += 3;
         } else {
            match |= (sch == pattern[px]);
            px++;
         }
      }
      if (px == plen) {
         error("matchFilenamePattern: missing ]");
      }
      if (match == invert) return false;
   } else if (pch != '?') {
      if (pch != sch) return false;
   }
   return recursiveMatch(str, sx + 1, pattern, px + 1);
}

/**********************************************************************/
/* Unit test for the filelib module                                   */
/**********************************************************************/

#ifndef _NOTEST_

/* Private function prototypes */

static void testGetRoot(void);
static void testGetExtension(void);
static void testGetHead(void);
static void testGetTail(void);
static void testDefaultExtension(void);
static void testFileExists(void);
static void testIsFile(void);
static void testIsDirectory(void);
static void testExpandPathname(void);
static void testMatchFilenamePattern(void);

/* Unit test */

void testFilelibModule(void) {
   testGetRoot();
   testGetExtension();
   testGetHead();
   testGetTail();
   testDefaultExtension();
   testFileExists();
   testIsFile();
   testIsDirectory();
   testExpandPathname();
   testMatchFilenamePattern();
}

/* Private functions */

static void testGetRoot(void) {
   test(getRoot("filename.ext"), "filename");
   test(getRoot("abc.def.ghi"), "abc.def");
   test(getRoot("/a/b/c/filename.ext"), "/a/b/c/filename");
   test(getRoot("A:a\\b\\c\\filename.ext"), "A:a\\b\\c\\filename");
}

static void testGetExtension(void) {
   test(getExtension("filename.ext"), ".ext");
   test(getExtension("abc.def.ghi"), ".ghi");
   test(getExtension("/a/b/c/filename.ext"), ".ext");
   test(getExtension("A:a\\b\\c\\filename.ext"), ".ext");
}

static void testGetHead(void) {
   test(getHead("/a/b/c/filename.ext"), "/a/b/c");
   test(getHead("A:a\\b\\c\\filename.ext"), "A:a\\b\\c");
   test(getHead("a/b"), "a");
   test(getHead("a"), "");
   test(getHead("/a"), "/");
   test(getHead("/"), "/");
}

static void testGetTail(void) {
   test(getTail("/a/b/c/filename.ext"), "filename.ext");
   test(getTail("A:a\\b\\c\\filename.ext"), "filename.ext");
   test(getTail("a/b"), "b");
   test(getTail("a"), "a");
   test(getTail("/a"), "a");
   test(getTail("/"), "");
}

static void testDefaultExtension(void) {
   test(defaultExtension("filename", ".ext"), "filename.ext");
   test(defaultExtension("filename.old", ".new"), "filename.old");
   test(defaultExtension("filename.old", "*.new"), "filename.new");
   test(defaultExtension("a.b.c", "*.x"), "a.b.x");
}

static void testFileExists(void) {
   test(fileExists("filelib.c"), true);
   test(fileExists("xyzzy.c"), false);
}

static void testIsFile(void) {
   test(isFile("filelib.c"), true);
   test(isFile("."), false);
   test(isFile("xyzzy.c"), false);
}

static void testIsDirectory(void) {
   test(isDirectory("filelib.c"), false);
   test(isDirectory("."), true);
   test(isDirectory("xyzzy.c"), false);
}

static void testExpandPathname(void) {
   if (stringEqual(getDirectoryPathSeparator(), "/")) {
      test(expandPathname("a/b"), "a/b");
      test(expandPathname("a\\b"), "a/b");
   } else {
      test(expandPathname("a/b"), "a\\b");
      test(expandPathname("a\\b"), "a\\b");
   }
}

static void testMatchFilenamePattern(void) {
   test(matchFilenamePattern("abc", "abc"), true);
   test(matchFilenamePattern("xxx", "abc"), false);
   test(matchFilenamePattern("ab", "abc"), false);
   test(matchFilenamePattern("abcd", "abc"), false);
   test(matchFilenamePattern("axc", "a?c"), true);
   test(matchFilenamePattern("ac", "a?c"), false);
   test(matchFilenamePattern("axxc", "a?c"), false);
   test(matchFilenamePattern("axxc", "a??c"), true);
   test(matchFilenamePattern("axc", "a*c"), true);
   test(matchFilenamePattern("ac", "a*c"), true);
   test(matchFilenamePattern("axxc", "a*c"), true);
   test(matchFilenamePattern("axxcd", "a*c"), false);
   test(matchFilenamePattern("axxbyyc", "a*b*c"), true);
   test(matchFilenamePattern("abc", "[abc]bc"), true);
   test(matchFilenamePattern("bbc", "[abc]bc"), true);
   test(matchFilenamePattern("xbc", "[abc]bc"), false);
   test(matchFilenamePattern("abc", "[^abc]bc"), false);
   test(matchFilenamePattern("bbc", "[^abc]bc"), false);
   test(matchFilenamePattern("xbc", "[^abc]bc"), true);
   test(matchFilenamePattern("abc", "[a-c]bc"), true);
   test(matchFilenamePattern("bbc", "[a-c]bc"), true);
   test(matchFilenamePattern("xbc", "[a-c]bc"), false);
   test(matchFilenamePattern("abc", "[^a-c]bc"), false);
   test(matchFilenamePattern("bbc", "[^a-c]bc"), false);
   test(matchFilenamePattern("xbc", "[^a-c]bc"), true);
}

#endif
