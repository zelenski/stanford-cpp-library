/*
 * File: appsection.c
 * ------------------
 * This program numbers sections in the appendices.  All it does is
 * compute the ordinal position of this directory in its parent.
 */

#include <stdio.h>
#include "cslib.h"
#include "filelib.h"
#include "strlib.h"

main() {
   string cwd, *list;
   int i, count;

   cwd = getTail(getCurrentDirectory());
   list = listDirectory("..");
   count = 1;
   for (i = 0; list[i] != NULL; i++) {
      if (stringEqual(cwd, list[i])) {
         printf("%d\n", count);
         return;
      }
      if (isDirectory(concat("../", list[i]))) count++;
   }
   printf("x\n");
}
