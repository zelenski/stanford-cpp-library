/*
 * File: loadobj.c
 * Last modified on Fri Sep 26 09:34:55 2014 by eroberts
 * -----------------------------------------------------
 * This file implements the loadobj.h interface.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include "cslib.h"
#include "filelib.h"
#include "loadobj.h"
#include "strlib.h"

#ifndef MAX_PATH_NAME
#  define MAX_PATH_NAME 1000
#endif

typedef struct Cell {
   void *handle;
   struct Cell *link;
} *HandleList;

HandleList handles = NULL;

void loadObject(string pathname) {
   HandleList hl;
   static char fullPath[MAX_PATH_NAME];

   hl = newBlock(HandleList);
   switch (pathname[0]) {
     case '/':
       strcpy(fullPath, pathname);
       break;
     case '~':
       strcpy(fullPath, expandPathname(pathname));
       break;
     default:
       getcwd(fullPath, MAX_PATH_NAME);
       strcat(fullPath, "/");
       strcat(fullPath, pathname);
       break;
   }
   hl->handle = dlopen(fullPath, RTLD_NOW);
   if (hl->handle == NULL) {
      freeBlock(hl);
      error("loadObject: %s", dlerror());
   }
   hl->link = handles;
   handles = hl;
}

proc findFunction(string fnname) {
   HandleList hl;
   void *fn;

   for (hl = handles; hl != NULL; hl = hl->link) {
      fn = dlsym(hl->handle, fnname);
      if (fn != NULL) return (proc) (long) fn;
   }
   error("findFunction: %s", dlerror());
   return NULL;
}

void loadSymbols(string progname) {
   /* Empty */
}
