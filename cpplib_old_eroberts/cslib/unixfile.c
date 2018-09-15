/*
 * File: unixfile.c
 * Last modified on Fri Sep 26 09:34:55 2014 by eroberts
 * -----------------------------------------------------
 * Implements the filelib.h interface for unix or unix-like systems
 * (including Mac OS X running standalone applications).  The entire
 * file compiles only on such systems and is empty on other platforms.
 */

#ifdef __unix__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "cslib.h"
#include "filelib.h"
#include "strlib.h"
#include "vector.h"

static string SKIP_FILES[] = {
   ".",
   "..",
   ".DS_Store",
   NULL
};

static int alphaCompare(const void *p1, const void *p2) {
   return strcmp(*(string *) p1, *(string *) p2);
}

string getDirectoryPathSeparator(void) {
   return "/";
}

string getSearchPathSeparator(void) {
   return ":";
}

bool fileExists(string path) {
   struct stat sblk;
   return stat(path, &sblk) == 0;
}

bool isFile(string path) {
   struct stat sblk;
   if (lstat(path, &sblk) != 0) return false;
   return (sblk.st_mode & S_IFMT) == S_IFREG;
}

bool isSymbolicLink(string path) {
   struct stat sblk;
   if (lstat(path, &sblk) != 0) return false;
   return (sblk.st_mode & S_IFMT) == S_IFLNK;
}

bool isDirectory(string path) {
   struct stat sblk;
   if (lstat(path, &sblk) != 0) return false;
   return (sblk.st_mode & S_IFMT) == S_IFDIR;
}

void createDirectory(string path) {
   if (endsWith(path, "/")) {
      path = substring(path, 0, stringLength(path) - 2);
   }
   if (mkdir(path, 0777) != 0) {
      if (errno == EEXIST && isDirectory(path)) return;
      error("createDirectory: %s", strerror(errno));
   }
}

void deleteFile(string filename) {
   if (unlink(filename) != 0) {
      if (errno == ENOENT) return;
      error("deleteFile: %s", strerror(errno));
   }
}

void renameFile(string oldname, string newname) {
   if (rename(oldname, newname) != 0) {
      error("renameFile: %s", strerror(errno));
   }
}

string *listDirectory(string path) {
   string *result, name;
   struct dirent *dp;
   DIR *dir;
   Vector list;
   int i, n;

   if (path == NULL) path = ".";
   path = expandPathname(path);
   dir = opendir(path);
   if (dir == NULL) error("listDirectory: Can't open directory %s", path);
   list = newVector();
   while (true) {
      dp = readdir(dir);
      if (dp == NULL) break;
      name = dp->d_name;
      if (searchStringArray(name, SKIP_FILES) == -1) {
         add(list, copyString(name));
      }
   }
   closedir(dir);
   n = size(list);
   result = newArray(n + 1, string);
   for (i = 0; i < n; i++) {
      result[i] = get(list, i);
   }
   result[n] = NULL;
   freeVector(list);
   qsort(result, n, sizeof(string), alphaCompare);
   return result;
}

void setCurrentDirectory(string path) {
   chdir(path);
}

string getCurrentDirectory(void) {
   string cwd, result;

   cwd = getcwd(NULL, 0);
   result = copyString(cwd);
   free(cwd);
   return result;
}

string expandPathname(string filename) {
   string homedir;
   char *spos;
   struct passwd *pw;

   if (*filename == '~') {
      spos = filename;
      while ((*spos != '/') && (*spos != '\\') && (*spos != '\0')) {
         spos++;
      }
      if (spos - filename == 1) {
         homedir = getenv("HOME");
         if (homedir == NULL) homedir = getpwuid(getuid())->pw_dir;
      } else {
         homedir = substring(filename, 1, spos - filename - 1);
         pw = getpwnam(homedir);
         if (pw == NULL) error("expandPathname: No user %s", homedir);
         homedir = pw->pw_dir;
      }
      filename = concat(homedir, spos);
   } else {
      filename = copyString(filename);
   }
   for (spos = filename; *spos != '\0'; spos++) {
      if (*spos == '\\') *spos = '/';
   }
   return filename;
}

#endif
