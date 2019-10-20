/*
 * File: unittest.c
 * ----------------
 * This file implements the unittest.h interface.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "cslib.h"
#include "generic.h"
#include "strlib.h"
#include "unittest.h"

/* Constants */

#define MAX_HEX 20

/* Private function prototypes */

static void displayModuleHeader(void);
static void reportTest(bool test, string exp, string value, string fmt, ...);
static char classifyValueType(string value);
static bool startsWithTypeCast(string value);

/* Private variables */

static int errorCount;
static int indentation;
static bool verbose;
static string moduleName;
static string lastModule = NULL;
static bool first = true;

/* Exported entries */

void uTTest(string exp, string value, ...) {
   va_list args;
   bool b1, b2;
   double d1, d2;
   int i1, i2;
   string s1, s2;
   void *v1, *v2;
   char hex1[MAX_HEX], hex2[MAX_HEX];

   va_start(args, value);
   switch (classifyValueType(value)) {
     case 'B':
       b1 = va_arg(args, bool);
       b2 = va_arg(args, bool);
       reportTest(b1 == b2, exp, value, b1 ? "true" : "false");
       break;
     case 'C':
       i1 = va_arg(args, int);
       i2 = va_arg(args, int);
       reportTest(i1 == i2, exp, value, "'%c'", i1);
       break;
     case 'D':
       d1 = va_arg(args, double);
       d2 = va_arg(args, double);
       reportTest(d1 == d2, exp, value, "%g", d1);
       break;
     case 'I':
       i1 = va_arg(args, int);
       i2 = va_arg(args, int);
       reportTest(i1 == i2, exp, value, "%d", i1);
       break;
     case 'N':
       s1 = va_arg(args, string);
       reportTest((void *) s1 == NULL, exp, "NULL", "%s", s1);
       break;
     case 'S':
       s1 = va_arg(args, string);
       s2 = va_arg(args, string);
       value = quoteString(s2);
       reportTest(stringEqual(s1, s2), exp, value, "%s", quoteString(s1));
       break;
     case 'V':
       v1 = va_arg(args, void *);
       v2 = va_arg(args, void *);
       sprintf(hex1, " 0x%lX", (unsigned long) v1);
       sprintf(hex2, " 0x%lX", (unsigned long) v2);
       value = substring(value, 0, findChar(')', value, 0));
       s1 = concat(value, hex1);
       s2 = concat(value, hex2);
       reportTest(v1 == v2, exp, s2, "%s", s1);
       break;
   }
   va_end(args);
}

int testModule(string name, proc fn) {
   string plural;

   moduleName = startsWith(name, "test") ? name + 4 : name;
   indentation = 0;
   errorCount = 0;
   fn();
   plural = (errorCount == 1) ? "" : "s";
   printf("Module %s: %d error%s\n", moduleName, errorCount, plural);
   return errorCount;
}

void setVerboseTestingFlag(bool flag) {
   verbose = flag;
}

bool getVerboseTestingFlag(void) {
   return verbose;
}

void reportError(string msg, ...) {
   va_list args;

   if (msg != NULL) {
      va_start(args, msg);
      displayModuleHeader();
      printf("?? %*s", indentation, "");
      vprintf(msg, args);
      printf(" ??\n");
      va_end(args);
   }
   errorCount++;
}

void reportMessage(string msg, ...) {
   va_list args;

   if (getVerboseTestingFlag()) {
      va_start(args, msg);
      displayModuleHeader();
      printf("   %*s", indentation, "");
      vprintf(msg, args);
      printf("\n");
      va_end(args);
   }
}

void setReportIndentation(int indent) {
   indentation = indent;
}

int getReportIndentation(void) {
   return indentation;
}

void adjustReportIndentation(int delta) {
   indentation += delta;
}

/* Private methods */

static void displayModuleHeader(void) {
   if (moduleName != lastModule) {
      if (!first) printf("\n");
      printf("Testing %s:\n", moduleName);
      lastModule = moduleName;
      first = false;
   }
}

static void reportTest(bool test, string exp, string value, string fmt, ...) {
   va_list args;

   if (test) {
      if (verbose) {
         displayModuleHeader();
         printf("   %*s%s", indentation, "", exp);
         if (value != NULL) {
            printf(" -> %s", value);
         }
         printf("\n");
      }
   } else {
      displayModuleHeader();
      printf("?? %*s%s", indentation, "", exp);
      if (value != NULL) {
         printf(" -> ");
         va_start(args, fmt);
         vprintf(fmt, args);
         va_end(args);
         printf(" (should be %s)", value);
      }
      printf(" ??\n");
      errorCount++;
   }
}

static char classifyValueType(string value) {
   char type, ch;
   int i;

   if (stringEqual(value, "NULL")) return 'N';
   if (stringEqual(value, "true") || stringEqual(value, "false")) return 'B';
   if (startsWith(value, "(bool)")) return 'B';
   if (startsWith(value, "'") || startsWith(value, "(char)")) return 'C';
   if (startsWith(value, "(int)")) return 'I';
   if (startsWith(value, "(double)")) return 'D';
   if (startsWith(value, "(string)")) return 'S';
   if (startsWithTypeCast(value)) return 'P';
   type = 'I';
   for (i = 0; (ch = value[i]) != '\0'; i++) {
      if (ch == '.' || ch == 'E' || ch == 'e') {
         type = 'D';
      } else if (!isdigit(ch) && ch != '+' && ch != '-') {
         return 'S';
      }
   }
   return type;
}

static bool startsWithTypeCast(string value) {
   char ch;
   int i;

   if (value[0] != '(') return false;
   for (i = 1; (ch = value[i]) != '\0'; i++) {
      if (ch == ')') return (i > 1) && (value[i + 1] != '\0');
      if (!isalpha(ch)) return false;
   }
   return false;
}
