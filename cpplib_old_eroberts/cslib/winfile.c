/*
 * File: winfile.c
 * Last modified on Mon Jul  8 19:31:36 2013 by eroberts
 * -----------------------------------------------------
 * Implements the machine-specific functions in the filelib.h
 * interface for WIN32 platforms.
 */

#ifdef __WIN32__

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "filelib.h"
#include "cslib.h"
#include "strlib.h"

string getDirectoryPathSeparator(void) {
   return "\\";
}

string getSearchPathSeparator(void) {
   return ";";
}

bool fileExists(string path) {
   int status;

   if (path == NULL) error("fileExists: NULL filename");
   status = _access(path, 0);
   if (status == 0) return true;
   if (errno == EINVAL) error("fileExists: Invalid parameter");
   return false;
}

bool isFile(string path) {
   struct _stat sblk;
   if (_stat(path, &sblk) != 0) return false;
   return (sblk.st_mode & _S_IFREG) == _S_IFREG;
}

bool isSymbolicLink(string path) {
   return false;
}

bool isDirectory(string path) {
   struct _stat sblk;
   if (_stat(path, &sblk) != 0) return false;
   return (sblk.st_mode & _S_IFDIR) == _S_IFDIR;
}

void createDirectory(string path) {
   if (endsWith(path, "/")) {
      path = substring(path, 0, stringLength(path) - 2);
   }
   if (_mkdir(path) != 0) {
      if (errno == EEXIST && isDirectory(path)) return;
      error("createDirectory: %s", strerror(errno));
   }
}

void deleteFile(string filename) {
   if (_unlink(filename) != 0) {
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
   struct _finddata_t data;
   intptr_t fp;
   string *result;
   int i, n;

   path = expandPathname(path);
   if (!isDirectory(path)) error("listDirectory: Path is not a directory");
   fp = _findfirst(concat(path, "\\*"), &data);
   if ((long) fp == -1L) {
      if (errno != ENOENT) {
         error("listDirectory: %s", strerror(errno));
      }
      result = newArray(1, string);
      result[0] = NULL;
      return result;
   }
   n = 1;
   while (_findnext(fp, &data) == 0) n++;
   _findclose(fp);
   result = newArray(n + 1, string);
   fp = _findfirst(concat(path, "\\*"), &data);
   result[0] = copyString(data.name);
   for (i = 1; i < n; i++) {
      _findnext(fp, &data);
      result[i] = copyString(data.name);
   }
   result[n] = NULL;
   return result;
}

void setCurrentDirectory(string path) {
   _chdir(path);
}

string getCurrentDirectory(void) {
   string cwd, result;

   cwd = _getcwd(NULL, 0);
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
         if (homedir == NULL) {
            error("expandPathname: No HOME environment variable");
         }
      } else {
         homedir = substring(filename, 1, spos - filename - 1);
         homedir = concat("\\Users\\", homedir);
      }
      filename = concat(homedir, spos);
   } else {
      filename = copyString(filename);
   }
   for (spos = filename; *spos != '\0'; spos++) {
      if (*spos == '/') *spos = '\\';
   }
   return filename;
}

#endif
