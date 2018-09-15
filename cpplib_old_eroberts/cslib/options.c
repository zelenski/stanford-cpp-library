/*
 * File: options.c
 * Last modified on Fri Sep 26 09:34:55 2014 by eroberts
 * -----------------------------------------------------
 * This file implements the options.h interface.
 */

#include <stdio.h>
#include <ctype.h>
#include "cslib.h"
#include "generic.h"
#include "hashmap.h"
#include "options.h"
#include "strlib.h"
#include "unittest.h"
#include "vector.h"

/* Function prototypes */

static int findOptionSpec(string arg, string optionSpec[]);
static string scanOptionPattern(HashMap options, string key,
                                string arg, string pattern);

/* Exported entries */

HashMap parseOptions(string args[], string optionSpec[]) {
   HashMap options;
   Vector argvec;
   string arg, key, pattern, spec, value, str;
   int i, index, space;

   options = newHashMap();
   argvec = newVector();
   for (i = 0; args[i] != NULL; i++) {
      arg = args[i];
      if (arg[0] == '-' && arg[1] != '\0') {
         index = findOptionSpec(arg, optionSpec);
         spec = optionSpec[index];
         space = findChar(' ', spec, 0);
         if (space == -1) {
            key = spec;
            value = "true";
         } else {
            key = substring(spec, 0, space - 1);
            pattern = substring(spec, space + 1, stringLength(spec));
            if (args[++i] == NULL) {
               error("Missing value after %s", arg);
            }
            str = args[i];
            while (args[i + 1] != NULL && stringEqual(args[i + 1], "+")) {
               i += 2;
               if (args[i] == NULL) error("Missing value after +");
               str = concat(str, args[i]);
            }
            value = scanOptionPattern(options, arg, str, pattern);
         }
         put(options, key, value);
      } else {
         add(argvec, arg);
      }
   }
   put(options, "args", vectorToArray(argvec));
   return options;
}

string *parseShellArgs(string line) {
   Vector args;
   string arg;
   bool started;
   char ch, quote;
   int i;

   args = newVector();
   line = concat(line, " ");
   arg = "";
   quote = ' ';
   started = false;
   for (i = 0; line[i] != '\0'; i++) {
      ch = line[i];
      if (quote == '\\') {
         arg = concat(arg, charToString(ch));
         quote = ' ';
      } else if (quote == '"' || quote == '\'') {
         if (ch == quote) {
            quote = ' ';
         } else {
            arg = concat(arg, charToString(ch));
         }
      } else {
         if (ch == '\\' || ch == '"' || ch == '\'') {
            quote = ch;
            started = true;
         } else if (isspace(ch)) {
            if (started) add(args, arg);
            arg = "";
            started = false;
         } else {
            arg = concat(arg, charToString(ch));
            started = true;
         }
      }
   }
   return (string *) vectorToArray(args);
}

string *getArgList(HashMap options) {
   return (string *) get(options, "args");
}

string getOption(HashMap options, string key, string def) {
   string value;

   value = (string) get(options, key);
   return (value == NULL) ? def : value;
}

int getIntOption(HashMap options, string key, int def) {
   string value;

   value = (string) get(options, key);
   return (value == NULL) ? def : stringToInteger(value);
}

double getDoubleOption(HashMap options, string key, double def) {
   string value;

   value = (string) get(options, key);
   return (value == NULL) ? def : stringToReal(value);
}

int getCharOption(HashMap options, string key, char def) {
   string value;

   value = (string) get(options, key);
   return (value == NULL) ? def : value[0];
}

bool getBoolOption(HashMap options, string key, bool def) {
   string value;

   value = (string) get(options, key);
   return (value == NULL) ? def : stringEqual(value, "true");
}

string getColorOption(HashMap options, string key, string def) {
   string arg;

   arg = getOption(options, key, NULL);
   if (arg == NULL) return def;
   if (endsWith(arg, "%")) {
      arg = substring(arg, 0, stringLength(arg) - 2);
      return concat(arg, " 100 div colorscreen");
   } else if (startsWith(arg, "#")) {
      return concat("16", concat(arg, " sethexcolor"));
   } else if (startsWith(arg, "0x")) {
      arg = substring(arg, 2, stringLength(arg) - 1);
      return concat("16#", concat(arg, " sethexcolor"));
   } else if (isdigit(arg[0])) {
      return concat(arg, " setgray");
   }
   arg = toLowerCase(arg);
   if (stringEqual(arg, "black")) return "black";
   if (stringEqual(arg, "darkgray")) return "darkGray";
   if (stringEqual(arg, "gray")) return "gray";
   if (stringEqual(arg, "lightgray")) return "lightGray";
   if (stringEqual(arg, "white")) return "white";
   if (stringEqual(arg, "red")) return "red";
   if (stringEqual(arg, "green")) return "green";
   if (stringEqual(arg, "blue")) return "blue";
   if (stringEqual(arg, "yellow")) return "yellow";
   if (stringEqual(arg, "magenta")) return "magenta";
   if (stringEqual(arg, "cyan")) return "cyan";
   if (stringEqual(arg, "pink")) return "pink";
   if (stringEqual(arg, "orange")) return "orange";
   error("Unrecognized color %s", arg);
   return NULL;
}

double getUnitsOption(HashMap options, string key, double def) {
   double value;
   string arg;
   char units[3];
   int nscan;

   arg = getOption(options, key, NULL);
   if (arg == NULL) return def;
   nscan = sscanf(arg, "%lf%2s", &value, units);
   if (nscan == 1) return value;
   if (stringEqual(units, "pt")) return value;
   if (stringEqual(units, "px")) return value;
   if (stringEqual(units, "i")) return value * 72;
   if (stringEqual(units, "in")) return value * 72;
   if (stringEqual(units, "cm")) return value * 72 / 2.54;
   error("Unrecognized unit measure in %s", key);
   return 0;
}

void showUsage(string usage, string spec[]) {
   int i;

   printf("Usage: %s\n", usage);
   printf("  where <options> can be any of the following:\n");
   for (i = 0; spec[i] != NULL; i++) {
      printf("   %s\n", spec[i]);
   }
}

/* Private functions */

static int findOptionSpec(string arg, string optionSpec[]) {
   int i, index;

   index = -1;
   for (i = 0; optionSpec[i] != NULL; i++) {
      if (startsWith(optionSpec[i], arg)) {
         if (index == -1) {
            index = i;
         } else {
            error("Ambiguous option: %s", arg);
         }
      }
   }
   if (index == -1) error("Unrecognized option: %s", arg);
   return index;
}

static string scanOptionPattern(HashMap options, string key,
                                string arg, string pattern) {
   string text, match;
   char ch;

   if (stringEqual(pattern, "<char>")) {
      if (stringLength(arg) != 1) {
         error("Expected a single character after %s", key);
      }
      return arg;
   }
   if (stringEqual(pattern, "<int>")) {
      if (sscanf(concat(arg, "$"), "%*i%c", &ch) != 1 || ch != '$') {
         error("Expected an integer after %s", key);
      }
      return arg;
   }
   if (stringEqual(pattern, "<double>")) {
      if (sscanf(concat(arg, "$"), "%*f%c", &ch) != 1 || ch != '$') {
         error("Expected a number after %s", key);
      }
      return arg;
   }
   if (stringEqual(pattern, "<bool>")) {
      if (startsWith("true", toLowerCase(arg))) return "true";
      if (startsWith("false", toLowerCase(arg))) return "false";
      error("Expected a boolean value after %s", key);
   }
   if (stringEqual(pattern, "<cumulative>")) {
      if (!containsKey(options, key)) return arg;
      return concat(get(options, key), concat("+", arg));
   }
   if (startsWith(pattern, "<") && endsWith(pattern, ">")) return arg;
   text = concat("|", concat(pattern, "|"));
   match = concat("|", concat(arg, "|"));
   if (findString(match, text, 0) == -1) {
      error("Expected %s after %s", pattern, key);
   }
   return arg;
}

/**********************************************************************/
/* Unit test for the options module                                   */
/**********************************************************************/

#ifndef _NOTEST_

/* Unit test */

static string OPTIONS[] = {
   "-count <int>",
   "-size <double>",
   "-letter <char>",
   "-title <string>",
   "-units <units>",
   "-color <color>",
   "-trace on|off",
   "-skip <cumulative>",
   "-verbose",
   NULL
};

void testOptionsModule(void) {
   string *args;
   HashMap options;

   trace(args = parseShellArgs("-count 3 -size 1.5 -trace on -v a0 a1"));
   trace(options = parseOptions(args, OPTIONS));
   test(getIntOption(options, "-count", 0), 3);
   test(getDoubleOption(options, "-size", 0), 1.5);
   test(getOption(options, "-title", "none"), "none");
   test(getOption(options, "-trace", "off"), "on");
   test(getBoolOption(options, "-verbose", false), true);
   trace(args = getArgList(options));
   test(args[0], "a0");
   test(args[1], "a1");
   trace(args = parseShellArgs("-letter x"));
   trace(options = parseOptions(args, OPTIONS));
   test(get(options, "-letter"), "x");
   trace(args = parseShellArgs("-size"));
   testError(parseOptions(args, OPTIONS));
   trace(args = parseShellArgs("-size x"));
   testError(parseOptions(args, OPTIONS));
   trace(args = parseShellArgs("-trace true"));
   testError(parseOptions(args, OPTIONS));
   trace(args = parseShellArgs("-let xyz"));
   testError(parseOptions(args, OPTIONS));
   trace(args = parseShellArgs("-title 'Harry Potter'"));
   trace(options = parseOptions(args, OPTIONS));
   test(get(options, "-title"), "Harry Potter");
   trace(args = parseShellArgs("-units 1"));
   trace(options = parseOptions(args, OPTIONS));
   test(getUnitsOption(options, "-units", 0.0), 1.0);
   trace(args = parseShellArgs("-units 1in"));
   trace(options = parseOptions(args, OPTIONS));
   test(getUnitsOption(options, "-units", 0.0), 72.0);
   trace(args = parseShellArgs("-skip a -skip b -skip c"));
   trace(options = parseOptions(args, OPTIONS));
   test(get(options, "-skip"), "a+b+c");
}

#endif
