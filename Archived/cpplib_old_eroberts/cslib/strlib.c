/*
 * File: strlib.c
 * --------------
 * This file implements the strlib.h interface.
 */

/*
 * General implementation notes:
 * -----------------------------
 * This module implements the strlib library by mapping all
 * functions into the appropriate calls to the ANSI <string.h>
 * interface.  The implementations of the individual functions
 * are all quite simple and do not require individual comments.
 * For descriptions of the behavior of each function, see the
 * interface.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "exception.h"
#include "cslib.h"
#include "strlib.h"
#include "unittest.h"

/* Constants */

#define MAX_NUMBER_DIGITS 30

/* Private function prototypes */

static string createString(int len);

/* Section 1 -- Basic string operations */

string concat(string s1, string s2) {
   string s;
   int len1, len2;

   if (s1 == NULL || s2 == NULL) {
      error("concat: NULL string passed as an argument");
   }
   len1 = strlen(s1);
   len2 = strlen(s2);
   s = createString(len1 + len2);
   strcpy(s, s1);
   strcpy(s + len1, s2);
   return s;
}

char charAt(string s, int i) {
   int len;

   if (s == NULL) error("charAt: String value is NULL");
   len = strlen(s);
   if (i < 0 || i > len) {
      error("charAt: Index is out of range");
   }
   return s[i];
}

string substring(string s, int p1, int p2) {
   int len;
   string result;

   if (s == NULL) error("substring: String value is NULL");
   len = strlen(s);
   if (p1 < 0) p1 = 0;
   if (p2 >= len) p2 = len - 1;
   len = p2 - p1 + 1;
   if (len < 0) len = 0;
   result = createString(len);
   strncpy(result, s + p1, len);
   result[len] = '\0';
   return result;
}

string charToString(char ch) {
   string result;

   result = createString(1);
   result[0] = ch;
   result[1] = '\0';
   return result;
}

int stringLength(string s) {
   if (s == NULL) error("stringLength: String value is NULL");
   return strlen(s);
}

string copyString(string s) {
   string newstr;

   if (s == NULL) error("copyString: String value is NULL");
   newstr = createString(strlen(s));
   strcpy(newstr, s);
   return newstr;
}

/* Section 2 -- String comparison functions */

bool stringEqual(string s1, string s2) {
   if (s1 == NULL || s2 == NULL) {
      error("stringEqual: String value is NULL");
   }
   return strcmp(s1, s2) == 0;
}

bool stringEqualIgnoreCase(string s1, string s2) {
   int i;

   if (s1 == NULL || s2 == NULL) {
      error("stringEqualIgnoreCase: String value is NULL");
   }
   for (i = 0; s1[i] != '\0'; i++) {
      if (tolower(s1[i]) != tolower(s2[i])) return false;
   }
   return s2[i] == '\0';
}

int stringCompare(string s1, string s2) {
   int cmp;

   if (s1 == NULL || s2 == NULL) {
      error("stringCompare: String value is NULL");
   }
   cmp = strcmp(s1, s2);
   if (cmp == 0) return 0;
   return (cmp < 0) ? -1 : +1;
}

bool startsWith(string s1, string s2) {
   return strncmp(s1, s2, strlen(s2)) == 0;
}

bool endsWith(string s1, string s2) {
   int n1, n2;

   n1 = strlen(s1);
   n2 = strlen(s2);
   if (n2 > n1) return false;
   return strcmp(s1 + n1 - n2, s2) == 0;
}

/* Section 3 -- Search functions */

int findChar(char ch, string text, int start) {
   char *cptr;

   if (text == NULL) error("findChar: String value is NULL");
   if (start < 0) start = 0;
   if (start > strlen(text)) return -1;
   cptr = strchr(text + start, ch);
   if (cptr == NULL) return -1;
   return (int) (cptr - text);
}

int findString(string str, string text, int start) {
   char *cptr;

   if (str == NULL) error("findString: String value is NULL");
   if (text == NULL) error("findString: String value is NULL");
   if (start < 0) start = 0;
   if (start > strlen(text)) return -1;
   cptr = strstr(text + start, str);
   if (cptr == NULL) return -1;
   return (int) (cptr - text);
}

int findLastChar(char ch, string text) {
   char *cptr;

   if (text == NULL) error("findLastChar: String value is NULL");
   cptr = strrchr(text, ch);
   if (cptr == NULL) return -1;
   return (int) (cptr - text);
}

int findLastString(string str, string text) {
   int i, nc;

   if (str == NULL) error("findLastString: String value is NULL");
   if (text == NULL) error("findLastString: String value is NULL");
   nc = strlen(str);
   for (i = strlen(text) - nc; i >= 0; i--) {
      if (strncmp(str, text + i, nc) == 0) return i;
   }
   return -1;
}

/* Section 4 -- Conversion functions */

string toLowerCase(string s) {
   string result;
   int i;

   if (s == NULL) {
      error("toLowerCase: String value is NULL");
   }
   result = createString(strlen(s));
   for (i = 0; s[i] != '\0'; i++) {
      result[i] = tolower(s[i]);
   }
   result[i] = '\0';
   return result;
}

string toUpperCase(string s) {
   string result;
   int i;

   if (s == NULL) {
      error("toUpperCase: String value is NULL");
   }
   result = createString(strlen(s));
   for (i = 0; s[i] != '\0'; i++) {
      result[i] = toupper(s[i]);
   }
   result[i] = '\0';
   return result;
}

string integerToString(int n) {
   char buffer[MAX_NUMBER_DIGITS];

   sprintf(buffer, "%d", n);
   return copyString(buffer);
}

int stringToInteger(string s) {
   int result;
   char dummy;

   if (s == NULL) {
      error("stringToInteger: String value is NULL");
   }
   if (sscanf(s, " %d %c", &result, &dummy) != 1) {
      error("stringToInteger: Illegal number %s", s);
   }
   return result;
}

/*
 * Implementation notes: realToString
 * ----------------------------------
 * Some implementations of printf pad the exponent field to three
 * digits, but all seem to pad it to at least two.  The string
 * manipulation code in the function definition standardizes on
 * the two-digit form.
 */

string realToString(double d) {
   char buffer[MAX_NUMBER_DIGITS];
   int len;

   sprintf(buffer, "%G", d);
   len = strlen(buffer);
   if (len > 5 && buffer[len - 5] == 'E' && buffer[len - 3] == '0') {
      buffer[len - 3] = buffer[len - 2];
      buffer[len - 2] = buffer[len - 1];
      buffer[len - 1] = '\0';
   }
   return copyString(buffer);
}

double stringToReal(string s) {
   double result;
   char dummy;

   if (s == NULL) error("stringToReal: String value is NULL");
   if (sscanf(s, " %lg %c", &result, &dummy) != 1) {
      error("stringToReal: Illegal number %s", s);
   }
   return result;
}

string trim(string str) {
   int start, finish;

   start = 0;
   while (str[start] != '\0' && isspace(str[start])) {
      start++;
   }
   finish = strlen(str) - 1;
   while (finish >= start && isspace(str[finish])) {
      finish--;
   }
   return substring(str, start, finish);
}

string quoteString(string str) {
   int i, n;
   string result;
   char ch, *cp;

   n = 2;
   for (i = 0; (ch = str[i]) != '\0'; i++) {
      switch (ch) {
       case '\a': case '\b': case '\f': case '\n': case '\r':
       case '\t': case '\v': case '"': case '\\':
         n += 2;
         break;
       default:
         n += (ch < 32 || ch >= 127) ? 4 : 1;
         break;
      }
   }
   result = newArray(n + 1, char);
   cp = result;
   *cp++ = '"';
   for (i = 0; (ch = str[i]) != '\0'; i++) {
      switch (ch) {
       case '\a': *cp++ = '\\'; *cp++ = 'a'; break;
       case '\b': *cp++ = '\\'; *cp++ = 'b'; break;
       case '\f': *cp++ = '\\'; *cp++ = 'f'; break;
       case '\n': *cp++ = '\\'; *cp++ = 'n'; break;
       case '\r': *cp++ = '\\'; *cp++ = 'r'; break;
       case '\t': *cp++ = '\\'; *cp++ = 't'; break;
       case '\v': *cp++ = '\\'; *cp++ = 'v'; break;
       case '"': *cp++ = '\\'; *cp++ = '"'; break;
       case '\\': *cp++ = '\\'; *cp++ = '\\'; break;
       default:
         if (ch < 32 || ch >= 127) {
            sprintf(cp, "\\%03o", ch);
            cp += 4;
         } else {
            *cp++ = ch;
         }
         break;
      }
   }
   *cp++ = '"';
   *cp = '\0';
   return result;
}

string quoteHTML(string str) {
   int i, n;
   string result;
   char ch, *cp;

   n = 0;
   for (i = 0; (ch = str[i]) != '\0'; i++) {
      switch (ch) {
       case '&': n += 5; break;
       case '<': n += 4; break;
       case '>': n += 4; break;
       default: n++; break;
      }
   }
   result = newArray(n + 1, char);
   cp = result;
   for (i = 0; (ch = str[i]) != '\0'; i++) {
      switch (ch) {
       case '&': strcpy(cp, "&amp;"); cp += 5; break;
       case '<': strcpy(cp, "&lt;"); cp += 4; break;
       case '>': strcpy(cp, "&gt;"); cp += 4; break;
       default: *cp++ = ch; break;
      }
   }
   *cp = '\0';
   return result;
}

int stringArrayLength(string array[]) {
   int i;

   if (array == NULL) return 0;
   for (i = 0; array[i] != NULL; i++) {
      /* Empty */
   }
   return i;
}

int searchStringArray(string str, string array[]) {
   int i;

   if (array == NULL) return -1;
   for (i = 0; array[i] != NULL; i++) {
      if (stringEqual(str, array[i])) return i;
   }
   return -1;
}

/*
 * Private function: createString
 * Usage: s = createString(len);
 * -----------------------------
 * This function dynamically allocates space for a string of
 * len characters, leaving room for the null character at the
 * end.
 */

static string createString(int len) {
   return (string) getBlock(len + 1);
}

/**********************************************************************/
/* Unit test for the strlib module                                    */
/**********************************************************************/

#ifndef _NOTEST_

/* Private function prototypes */

static void testConcat(void);
static void testIthChar(void);
static void testSubString(void);
static void testCharToString(void);
static void testStringLength(void);
static void testCopyString(void);
static void testStringEqual(void);
static void testStringEqualIgnoreCase(void);
static void testStringCompare(void);
static void testStartsWith(void);
static void testEndsWith(void);
static void testFindChar(void);
static void testFindString(void);
static void testFindLastChar(void);
static void testFindLastString(void);
static void testConvertToLowerCase(void);
static void testConvertToUpperCase(void);
static void testIntegerToString(void);
static void testStringToInteger(void);
static void testRealToString(void);
static void testStringToReal(void);
static void testTrim(void);
static void testQuoteString(void);
static void testQuoteHTML(void);
static void testStringArrayLength(void);
static void testSearchStringArray(void);

/* Unit test */

void testStrlibModule(void) {
   testConcat();
   testIthChar();
   testSubString();
   testCharToString();
   testStringLength();
   testCopyString();
   testStringEqual();
   testStringEqualIgnoreCase();
   testStringCompare();
   testStartsWith();
   testEndsWith();
   testFindChar();
   testFindString();
   testFindLastChar();
   testFindLastString();
   testConvertToLowerCase();
   testConvertToUpperCase();
   testIntegerToString();
   testStringToInteger();
   testRealToString();
   testStringToReal();
   testTrim();
   testQuoteString();
   testQuoteHTML();
   testStringArrayLength();
   testSearchStringArray();
}

/* Private functions */

static void testConcat(void) {
   test(concat("hello", "world"), "helloworld");
   test(concat("hello", ""), "hello");
   test(concat("", "world"), "world");
}

static void testIthChar(void) {
   test(charAt("abcde", 0), 'a');
   test(charAt("abcde", 4), 'e');
   test(charAt("abcde", 5), '\0');
   testError(charAt("abcde", 6));
}

static void testSubString(void) {
   test(substring("abcde", 0, 1), "ab");
   test(substring("abcde", 0, 4), "abcde");
   test(substring("abcde", -1, 5), "abcde");
   test(substring("abcde", 3, 2), "");
}

static void testCharToString(void) {
   test(charToString('x'), "x");
}

static void testStringLength(void) {
   test(stringLength("abcde"), 5);
   test(stringLength(""), 0);
}

static void testCopyString(void) {
   test(copyString("abcde"), "abcde");
}

static void testStringEqual(void) {
   test(stringEqual("cat", "dog"), false);
   test(stringEqual("abcde", "abcde"), true);
   test(stringEqual("ABCDE", "abcde"), false);
   test(stringEqual("abc", "abcde"), false);
   test(stringEqual("abcde", "abc"), false);
   test(stringEqual("", ""), true);
}

static void testStringEqualIgnoreCase(void) {
   test(stringEqualIgnoreCase("cat", "dog"), false);
   test(stringEqualIgnoreCase("abcde", "abcde"), true);
   test(stringEqualIgnoreCase("ABCDE", "abcde"), true);
   test(stringEqualIgnoreCase("abc", "abcde"), false);
   test(stringEqualIgnoreCase("abcde", "abc"), false);
   test(stringEqualIgnoreCase("", ""), true);
}

static void testStringCompare(void) {
   test(stringCompare("cat", "cat"), 0);
   test(stringCompare("cat", "dog"), -1);
   test(stringCompare("dog", "cat"), +1);
   test(stringCompare("", "xxx"), -1);
   test(stringCompare("xxx", ""), +1);
}

static void testStartsWith(void) {
   test(startsWith("start", ""), true);
   test(startsWith("start", "star"), true);
   test(startsWith("start", "start"), true);
   test(startsWith("start", "startx"), false);
}

static void testEndsWith(void) {
   test(endsWith("start", ""), true);
   test(endsWith("start", "tart"), true);
   test(endsWith("start", "start"), true);
   test(endsWith("start", "xstart"), false);
}

static void testFindChar(void) {
   test(findChar('c', "abcabc", 0), 2);
   test(findChar('c', "abcabc", 3), 5);
   test(findChar('a', "abcabc", 5), -1);
}

static void testFindString(void) {
   test(findString("cad", "abracadabra", 0), 4);
   test(findString("cad", "abracadabra", 6), -1);
   test(findString("abr", "abracadabra", 6), 7);
}

static void testFindLastChar(void) {
   test(findLastChar('c', "abcabc"), 5);
   test(findLastChar('d', "abcabc"), -1);
}

static void testFindLastString(void) {
   test(findLastString("abr", "abracadabra"), 7);
   test(findLastString("xxx", "abracadabra"), -1);
}

static void testConvertToLowerCase(void) {
   test(toLowerCase("ABC"), "abc");
   test(toLowerCase("#A1"), "#a1");
}

static void testConvertToUpperCase(void) {
   test(toUpperCase("abc"), "ABC");
   test(toUpperCase("#a1"), "#A1");
}

static void testIntegerToString(void) {
   test(integerToString(0), "0");
   test(integerToString(1234567), "1234567");
   test(integerToString(-17), "-17");
}

static void testStringToInteger(void) {
   test(stringToInteger("1234"), 1234);
   test(stringToInteger("-42"), -42);
   testError(stringToInteger("X"));
}

static void testRealToString(void) {
   test(realToString(0), "0");
   test(realToString(3.1416), "3.1416");
   test(realToString(-2.3), "-2.3");
   test(realToString(1.75E+15), "1.75E+15");
}

static void testStringToReal(void) {
   test(stringToReal("1234"), 1234.0);
   test(stringToReal("-4.2"), -4.2);
   test(stringToReal("1.28E-15"), 1.28E-15);
   testError(stringToReal("X"));
}

static void testQuoteString(void) {
   test(quoteString("abcde"), "\"abcde\"");
   test(quoteString("ab\t\"\5cd"), "\"ab\\t\\\"\\005cd\"");
}

static void testQuoteHTML(void) {
   test(quoteHTML("a < 4 && b > 0"), "a &lt; 4 &amp;&amp; b &gt; 0");
}

static void testTrim(void) {
   test(trim("abcde"), "abcde");
   test(trim("    abcde"), "abcde");
   test(trim("abcde    "), "abcde");
   test(trim("\tabcde  "), "abcde");
   test(trim(""), "");
}

static void testStringArrayLength(void) {
   string primaryColors[4];
   primaryColors[0] = "red";
   primaryColors[1] = "green";
   primaryColors[2] = "blue";
   primaryColors[3] = NULL;
   test(stringArrayLength(primaryColors), 3);
}

static void testSearchStringArray(void) {
   string primaryColors[4];
   primaryColors[0] = "red";
   primaryColors[1] = "green";
   primaryColors[2] = "blue";
   primaryColors[3] = NULL;
   test(searchStringArray("red", primaryColors), 0);
   test(searchStringArray("green", primaryColors), 1);
   test(searchStringArray("blue", primaryColors), 2);
   test(searchStringArray("yellow", primaryColors), -1);
}

#endif
