/*
 * File: cmpfn.c
 * Last modified on Wed Apr  9 07:29:52 2008 by eroberts
 * -----------------------------------------------------
 * This file implements the comparison functions exported by
 * the cmpfn.h interface.  The reason to centralize them in
 * a single module is to allow the same functions to be
 * shared among many different modules.
 */

#include <stdio.h>
#include "cmpfn.h"
#include "cslib.h"
#include "strlib.h"

/* Exported entries */

int intCmpFn(const void *p1, const void *p2) {
   int v1, v2;

   v1 = *((int *) p1);
   v2 = *((int *) p2);
   if (v1 == v2) return 0;
   return (v1 < v2) ? -1 : +1;
}

int shortCmpFn(const void *p1, const void *p2) {
   short v1, v2;

   v1 = *((short *) p1);
   v2 = *((short *) p2);
   if (v1 == v2) return 0;
   return (v1 < v2) ? -1 : +1;
}

int longCmpFn(const void *p1, const void *p2) {
   long v1, v2;

   v1 = *((long *) p1);
   v2 = *((long *) p2);
   if (v1 == v2) return 0;
   return (v1 < v2) ? -1 : +1;
}

int charCmpFn(const void *p1, const void *p2) {
   unsigned char v1, v2;

   v1 = *((unsigned char *) p1);
   v2 = *((unsigned char *) p2);
   if (v1 == v2) return 0;
   return (v1 < v2) ? -1 : +1;
}

int floatCmpFn(const void *p1, const void *p2) {
   float v1, v2;

   v1 = *((float *) p1);
   v2 = *((float *) p2);
   if (v1 == v2) return 0;
   return (v1 < v2) ? -1 : +1;
}

int doubleCmpFn(const void *p1, const void *p2) {
   double v1, v2;

   v1 = *((double *) p1);
   v2 = *((double *) p2);
   if (v1 == v2) return 0;
   return (v1 < v2) ? -1 : +1;
}

int unsignedCmpFn(const void *p1, const void *p2) {
   unsigned v1, v2;

   v1 = *((unsigned *) p1);
   v2 = *((unsigned *) p2);
   if (v1 == v2) return 0;
   return (v1 < v2) ? -1 : +1;
}

int unsignedShortCmpFn(const void *p1, const void *p2) {
   unsigned short v1, v2;

   v1 = *((unsigned short *) p1);
   v2 = *((unsigned short *) p2);
   if (v1 == v2) return 0;
   return (v1 < v2) ? -1 : +1;
}

int unsignedLongCmpFn(const void *p1, const void *p2) {
   unsigned long v1, v2;

   v1 = *((unsigned long *) p1);
   v2 = *((unsigned long *) p2);
   if (v1 == v2) return 0;
   return (v1 < v2) ? -1 : +1;
}

int unsignedCharCmpFn(const void *p1, const void *p2) {
   unsigned char v1, v2;

   v1 = *((unsigned char *) p1);
   v2 = *((unsigned char *) p2);
   if (v1 == v2) return 0;
   return (v1 < v2) ? -1 : +1;
}

int stringCmpFn(const void *p1, const void *p2) {
   return stringCompare(*((string *) p1), *((string *) p2));
}

int pointerCmpFn(const void *p1, const void *p2) {
   void *v1, *v2;

   v1 = *((void **) p1);
   v2 = *((void **) p2);
   if (v1 == v2) return 0;
   return (v1 < v2) ? -1 : +1;
}
