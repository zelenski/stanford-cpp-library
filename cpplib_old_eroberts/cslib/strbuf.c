/*
 * File: strbuf.c
 * --------------
 * This file implements the strbuf.h interface.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "exception.h"
#include "cslib.h"
#include "strbuf.h"
#include "strlib.h"
#include "unittest.h"

/* Constants */

#define INITIAL_CAPACITY 100
#define MAX_NUMBER_DIGITS 30

/*
 * Type: StringBufferCDT
 * ---------------------
 * This type is the concrete type for the StringBuffer.
 */

struct StringBufferCDT {
   int capacity;
   int count;
   char *buffer;
};

/* Private function prototypes */

static void ensureCapacity(StringBuffer sb, int capacity);

/* Exported entries */

StringBuffer newStringBuffer() {
   StringBuffer sb;

   sb = newBlock(StringBuffer);
   sb->capacity = INITIAL_CAPACITY;
   sb->count = 0;
   sb->buffer = newArray(sb->capacity, char);
   return sb;
}

void freeStringBuffer(StringBuffer sb) {
   freeBlock(sb->buffer);
   freeBlock(sb);
}

void pushChar(StringBuffer sb, char ch) {
   ensureCapacity(sb, sb->count + 1);
   sb->buffer[sb->count++] = ch;
}

char popChar(StringBuffer sb) {
   if (sb->count == 0) error("popChar: StringBuffer is empty");
   return sb->buffer[--sb->count];
}

void appendString(StringBuffer sb, string str) {
   int n;

   n = stringLength(str);
   ensureCapacity(sb, sb->count + n);
   memcpy(&sb->buffer[sb->count], str, n);
   sb->count += n;
}

void sbprintf(StringBuffer sb, string format, ...) {
   va_list args;
   int capacity;

   va_start(args, format);
   capacity = printfCapacity(format, args);
   va_end(args);
   va_start(args, format);
   sbFormat(sb, capacity, format, args);
   va_end(args);
}

void sbFormat(StringBuffer sb, int capacity, string format, va_list args) {
   int n;

   ensureCapacity(sb, sb->count + capacity + 1);
   n = vsprintf(&sb->buffer[sb->count], format, args);
   va_end(args);
   sb->count += n;
}

bool isEmptyStringBuffer(StringBuffer sb) {
   return sb->count == 0;
}

int sizeStringBuffer(StringBuffer sb) {
   return sb->count;
}

void clearStringBuffer(StringBuffer sb) {
   sb->count = 0;
}

string getString(StringBuffer sb) {
   ensureCapacity(sb, sb->count + 1);
   sb->buffer[sb->count] = '\0';
   return sb->buffer;
}

/* Friend functions */

int printfCapacity(string format, va_list args) {
   int i, capacity, width, precision, len, lcount;
   bool inFormat, dotSeen;
   string str;
   char ch;

   capacity = 0;
   inFormat = false;
   for (i = 0; format[i] != '\0'; i++) {
      ch = format[i];
      if (!inFormat) {
         if (ch == '%') {
            inFormat = true;
            dotSeen = false;
            width = -1;
            precision = -1;
            lcount = 0;
         } else {
            capacity++;
         }
      } else {
         switch (ch) {
          case '0': case '1': case '2': case '3': case '4':
          case '5': case '6': case '7': case '8': case '9':
            if (dotSeen) {
               if (precision < 0) precision = 0;
               precision = 10 * precision + (ch - '0');
            } else {
               if (width < 0) width = 0;
               width = 10 * width + (ch - '0');
            }
            break;
          case '*':
            if (dotSeen) {
               precision = va_arg(args, int);
            } else {
               width = va_arg(args, int);
            }
            break;
          case '.':
            dotSeen = true;
            precision = 0;
            break;
          case '+': case '-': case '#': case ' ':
          case 'h': case 'z': case 'j': case 't':
            break;
          case 'l':
            lcount++;
            break;
          case '%':
            capacity++;
            inFormat = false;
            break;
          case 'd': case 'i': case 'o': case 'u': case 'x': case 'X':
            if (lcount == 0) {
               va_arg(args, int);
            } else {
               va_arg(args, long);
            }
            len = MAX_NUMBER_DIGITS;
            if (width > len) len = width;
            capacity += len;
            inFormat = false;
            break;
          case 'a': case 'e': case 'f': case 'g':
          case 'A': case 'E': case 'F': case 'G':
            if (lcount == 0) {
               va_arg(args, double);
            } else if (lcount == 1) {
               va_arg(args, long double);
            }
            len = MAX_NUMBER_DIGITS;
            if (width > len) len = width;
            capacity += len;
            inFormat = false;
            break;
          case 'c':
            va_arg(args, int);
            capacity += 1 + lcount;
            inFormat = false;
            break;
          case 's':
            str = va_arg(args, string);
            if (precision >= 0) {
               capacity += precision;
            } else {
               len = stringLength(str);
               if (width > len) len = width;
               if (lcount > 0) len = 2 * len;
               capacity += len;
            }
            inFormat = false;
            break;
         }
      }
   }
   return capacity;
}

/*
 * Private function: ensureCapacity
 * Usage: ensureCapacity(sb, capacity);
 * ------------------------------------
 * Makes sure that the capacity of the StringBuffer is at least as large
 * as the specified capacity.
 */

void ensureCapacity(StringBuffer sb, int capacity) {
   string old;
   int cap;

   if (sb->capacity >= capacity) return;
   cap = sb->capacity;
   if (cap < INITIAL_CAPACITY) cap = INITIAL_CAPACITY;
   while (cap < capacity) {
      cap *= 2;
   }
   old = sb->buffer;
   sb->capacity = cap;
   sb->buffer = newArray(cap, char);
   memcpy(sb->buffer, old, sb->count);
   freeBlock(old);
}

/**********************************************************************/
/* Unit test for the strbuf module                                    */
/**********************************************************************/

#ifndef _NOTEST_

/* Private function prototypes */

static void testStringBufferAppend(void);
static void testStringBufferFormat(void);
static int testCapacity(string format, ...);

/* Unit test */

void testStrbufModule(void) {
   string alphabet;

   testStringBufferAppend();
   testStringBufferFormat();
   trace(alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
   test(testCapacity("ABCDE"), 5);
   test(testCapacity("%%"), 1);
   test(testCapacity("%d", 42), (int) MAX_NUMBER_DIGITS);
   test(testCapacity("%50d", 42), 50);
   test(testCapacity("%*d", 50, 42), 50);
   test(testCapacity("%s", alphabet), 26);
   test(testCapacity("%10.10s", alphabet), 10);
   test(testCapacity("%50s", alphabet), 50);
}

static void testStringBufferAppend(void) {
   StringBuffer sb;
   string str;
   int i;

   sb = newStringBuffer();
   pushChar(sb, '(');
   str = "(";
   for (i = 0; i < 999; i++) {
      appendString(sb, integerToString(i));
      str = concat(str, integerToString(i));
   }
   str = concat(str, ")");
   pushChar(sb, ')');
   test(stringEqual(getString(sb), str), true);
   test(popChar(sb), ')');
   trace(clear(sb));
   test(getString(sb), "");
}

static void testStringBufferFormat(void) {
   StringBuffer sb;

   sb = newStringBuffer();
   trace(sbprintf(sb, "The answer is %d", 42));
   test(getString(sb), "The answer is 42");
}

static int testCapacity(string format, ...) {
   va_list args;
   int capacity;

   va_start(args, format);
   capacity = printfCapacity(format, args);
   va_end(args);
   return capacity;
}

#endif
