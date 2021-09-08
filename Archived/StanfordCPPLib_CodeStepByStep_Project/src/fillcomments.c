/*
 * File: fillcomments.c
 * Last modified on Mon Jun  9 18:57:29 2014 by eroberts
 * -----------------------------------------------------
 * This program removes HTML tags from javadoc/cppdoc comment lines but
 * refills those lines to ensure that lines don't end up looking choppy.
 * It also implements the -html option, which writes an HTML file
 * containing the text of the file.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "cslib.h"
#include "hashmap.h"
#include "options.h"
#include "simpio.h"
#include "strlib.h"

/* Constants */

#define MAX_COMMENT_LINE 75
#define NBSP '\001'

#define NORMAL 0
#define TAGMODE 1
#define ANDMODE 2
#define FIRST_COMMENT 1
#define IN_COMMENT 2
#define IN_EXTENDED_COMMENT 3
#define IN_PREFORMATTED_BLOCK 4

/* Option specifications */

static string OPTION_SPEC[] = {
   "-help",
   "-html",
   "-output <name>",
   "-stylesheet <file>",
   "-hide <file>",
   NULL
};

/* Private variables */

string buffer;
string stylesheet;
HashMap options;
bool sentenceBreak;
bool html;

/* Private function prototypes */

static void convertStream(FILE *infile, FILE *outfile);
static void flushBuffer(FILE *outfile);
static void commentLine(string line, FILE *outfile);
static string standardLine(string line);
static bool dumpChar(int ch, FILE *outfile);
static void dumpHTMLHeader(string hName, FILE *outfile);
static void dumpHTMLTrailer(FILE *outfile);
static bool isBreak(string line);
static bool isDeletedLine(string line);
static string fixHTML(string line);

/* Main program */

main(int argc, string argv[]) {
   FILE *infile, *outfile;
   string *files, outName, hName;
   int nFiles;

   options = parseOptions(argv + 1, OPTION_SPEC);
   files = getArgList(options);
   nFiles = stringArrayLength(files);
   if (containsKey(options, "-help") || nFiles > 1) {
      showUsage("fillcomments <options> <infile>", OPTION_SPEC);
      return;
   }
   html = containsKey(options, "-html");
   stylesheet = getOption(options, "-stylesheet", "cppdoc.css");
   if (nFiles == 0) {
      infile = stdin;
      hName = "-----.h";
   } else {
      infile = fopen(files[0], "r");
      hName = files[0];
      if (infile == NULL) error("Can't open %s", files[0]);
   }
   if (containsKey(options, "-output")) {
      outName = getOption(options, "-output", "");
      outfile = fopen(outName, "w");
      if (outfile == NULL) error("Can't open %s", outName);
   } else {
      outfile = stdout;
   }
   if (html) dumpHTMLHeader(hName, outfile);
   convertStream(infile, outfile);
   if (html) dumpHTMLTrailer(outfile);
   if (infile != stdin) fclose(infile);
   if (outfile != stdout) fclose(outfile);
   exit(0);
}

/* Private functions */

static void convertStream(FILE *infile, FILE *outfile) {
   string line, kind, name;
   int state, colon;
   bool afterBlank, skipBlank;

   buffer = NULL;
   sentenceBreak = false;
   state = NORMAL;
   afterBlank = false;
   skipBlank = false;
   while (true) {
      line = readLine(infile);
      if (line == NULL) break;
      if (isDeletedLine(line)) {
         skipBlank = afterBlank;
         continue;
      }
      switch (state) {
       case NORMAL:
         if (stringEqual(line, "/*")) {
            state = FIRST_COMMENT;
         } else if (stringEqual(line, "/**")) {
            state = IN_EXTENDED_COMMENT;
            fprintf(outfile, "%s\n", line);
         } else {
            if (strlen(line) == 0) {
               if (skipBlank) {
                  skipBlank = false;
               } else {
                  fprintf(outfile, "\n");
                  afterBlank = true;
               }
            } else {
               fprintf(outfile, "%s\n", standardLine(line));
               afterBlank = skipBlank = false;
            }
         }
         break;
       case FIRST_COMMENT:
         if (html) {
            colon = findString(": ", line, 0);
            if (colon > 0) {
               kind = substring(line, 3, colon - 1);
               if (!stringEqual(kind, "File")) {
                  name = substring(line, colon + 2, strlen(line));
                  fprintf(outfile, "<a name=\"%s:%s\">", kind, name);
               }
            }
            if (stringEqual(stylesheet, "none")) {
               fprintf(outfile, "<scan style=\"color:blue;\">");
            } else {
               fprintf(outfile, "<scan class=comment>");
            }
         }
         fprintf(outfile, "/*\n");
         fprintf(outfile, "%s\n", fixHTML(line));
         state = IN_COMMENT;
         break;
       case IN_COMMENT:
         if (startsWith(line, " * -----")) state = IN_EXTENDED_COMMENT;
         if (startsWith(line, " */")) state = NORMAL;
         if (state == NORMAL && html) {
            fprintf(outfile, "%s</scan>\n", fixHTML(line));
         } else {
            fprintf(outfile, "%s\n", fixHTML(line));
         }
         skipBlank = afterBlank = false;
         break;
       case IN_EXTENDED_COMMENT:
         if (startsWith(line, " *<pre>")) {
            flushBuffer(outfile);
            state = IN_PREFORMATTED_BLOCK;
         } else {
            if (startsWith(line, " * ") && !isBreak(line)) {
               if (buffer == NULL) {
                  buffer = " * ";
               } else {
                  buffer = concat(buffer, (sentenceBreak) ? "  " : " ");
                  sentenceBreak = false;
               }
               commentLine(line + 3, outfile);
               sentenceBreak = endsWith(line, ".");
            } else {
               flushBuffer(outfile);
               buffer = "";
               commentLine(line, outfile);
               flushBuffer(outfile);
            }
            if (stringEqual(line, " */")) {
               flushBuffer(outfile);
               if (html) {
                  fprintf(outfile, "</scan>");
               }
               skipBlank = afterBlank = false;
               state = NORMAL;
            }
         }
         break;
       case IN_PREFORMATTED_BLOCK:
         if (startsWith(line, " *</pre>")) {
            state = IN_EXTENDED_COMMENT;
         } else {
            commentLine(line, outfile);
            flushBuffer(outfile);
            fprintf(outfile, "\n");
         }
         break;
      }
   }
   flushBuffer(outfile);
}

static void flushBuffer(FILE *outfile) {
   int i;

   if (buffer != NULL) {
      for (i = 0; buffer[i] != '\0'; i++) {
         if (buffer[i] == NBSP) buffer[i] = ' ';
      }
      fprintf(outfile, "%s\n", fixHTML(buffer));
      buffer = NULL;
   }
}

static void commentLine(string line, FILE *outfile) {
   int index, ch, state;
   string tag;

   state = NORMAL;
   for (index = 0; line[index] != '\0'; index++) {
      ch = line[index];
      switch (state) {
       case NORMAL:
         switch (ch) {
          case '<':
            if (startsWith(line, "#include")) {
               dumpChar(ch, outfile);
            } else {
               state = TAGMODE;
               tag = "";
            }
            break;
          case '&':
            state = ANDMODE;
            tag = "";
            break;
          default:
            if (dumpChar(ch, outfile)) {
               while (line[index + 1] == ' ') index++;
            }
            break;
         }
         break;
       case TAGMODE:
         if (ch == '>') {
            state = NORMAL;
         } else if (tag[0] == 0 && (ch == ' ' || ch == '>' || ch == '<')) {
            dumpChar('<', outfile);
            dumpChar(ch, outfile);
            state = NORMAL;
         } else {
            tag = concat(tag, charToString(ch));
         }
         break;
       case ANDMODE:
         if (ch == ' ' || ch == '&') {
            dumpChar('&', outfile);
            dumpChar(ch, outfile);
            state = NORMAL;
         } else if (ch == ';') {
            if (stringEqual(tag, "nbsp")) {
               dumpChar(NBSP, outfile);
            } else if (stringEqual(tag, "amp")) {
               dumpChar('&', outfile);
            } else if (stringEqual(tag, "gt")) {
               dumpChar('>', outfile);
            } else if (stringEqual(tag, "lt")) {
               dumpChar('<', outfile);
            }
            state = NORMAL;
         } else {
            tag = concat(tag, charToString(ch));
         }
         break;
      }
   }
}

static string standardLine(string line) {
   string str;
   int start, finish;

   if (!html) return line;
   line = quoteHTML(line);
   start = findString("/" "/", line, 0);
   if (start >= 0) {
      str = substring(line, 0, start - 1);
      if (stringEqual(stylesheet, "none")) {
         str = concat(str, "<scan style=\"color:blue;\">");
      } else {
         str = concat(str, "<scan class=comment>");
      }
      str = concat(str, substring(line, start, strlen(line)));
      str = concat(str, "</scan>");
      return str;
   }
   start = findString("/" "*", line, 0);
   if (start == -1) return line;
   finish = findString("*" "/", line, start);
   if (finish == -1) error("Multiline internal comment not supported");
   str = substring(line, 0, start - 1);
   if (stringEqual(stylesheet, "none")) {
      str = concat(str, "<scan style=\"color:blue;\">");
   } else {
      str = concat(str, "<scan class=comment>");
   }
   str = concat(str, substring(line, start, finish + 1));
   str = concat(str, "</scan>");
   str = concat(str, substring(line, finish + 2, strlen(line)));
   return str;
}

static bool dumpChar(int ch, FILE *outfile) {
   string str;
   int i, index;

   str = charToString(ch);
   switch (ch) {
     case 0x92:
       str = "'";
       break;
     case 0x93: case 0x94:
       str = "\"";
       break;
     case 0x95:
       str = "-";
       break;
     case 0x96:
       str = "--";
       break;
   }
   if (buffer != NULL) {
      buffer = concat(buffer, str);
      if (stringLength(buffer) > MAX_COMMENT_LINE) {
         index = stringLength(buffer) - 1;
         while (buffer[index] != ' ') index--;
         for (i = 0; i < index; i++) {
            if (buffer[i] == NBSP) buffer[i] = ' ';
         }
         fprintf(outfile, "%s\n", fixHTML(substring(buffer, 0, index - 1)));
         buffer = concat(" *", buffer + index);
         return stringLength(buffer) < 4;
      }
   } else {
      fprintf(outfile, "%s", fixHTML(str));
   }
   return false;
}

static void dumpHTMLHeader(string hName, FILE *outfile) {
   fprintf(outfile, "<html>\n");
   fprintf(outfile, "<head>\n");
   fprintf(outfile, "<title>%s</title>\n", hName);
   if (!stringEqual(stylesheet, "none")) {
      fprintf(outfile, "<link rel=\"stylesheet\" type=\"text/css\"");
      fprintf(outfile, " href=\"%s\" />\n", stylesheet);
   }
   fprintf(outfile, "</head>\n");
   fprintf(outfile, "<body>\n");
   fprintf(outfile, "<pre class=code>\n");
}

static void dumpHTMLTrailer(FILE *outfile) {
   fprintf(outfile, "</pre>\n");
   fprintf(outfile, "</body>\n");
   fprintf(outfile, "</html>\n");
}

static bool isBreak(string line) {
   return startsWith(line, " *  ")
       || startsWith(line, " * Macro: ")
       || startsWith(line, " * Statement: ")
       || startsWith(line, " * Function: ")
       || startsWith(line, " * Functions: ")
       || startsWith(line, " * Method: ")
       || startsWith(line, " * Methods: ")
       || startsWith(line, " * Type: ")
       || startsWith(line, " * Types: ")
       || startsWith(line, " * Operator: ")
       || startsWith(line, " * Operators: ")
       || startsWith(line, " * Constant: ")
       || startsWith(line, " * Usage: ")
       || startsWith(line, " * -----")
       || startsWith(line, " * @");
}

static bool isDeletedLine(string line) {
   return stringEqual(line, " * <ul>")
       || stringEqual(line, " * </ul>")
       || stringEqual(line, " * <ol>")
       || stringEqual(line, " * </ol>")
       || stringEqual(line, " * <table>")
       || stringEqual(line, " * </table>")
       || stringEqual(line, "#include \"foreach.h\"")
       || findString("<include src=\"", line, 0) != -1
       || startsWith(line, "#include \"private/");
};

static string fixHTML(string line) {
   return (html) ? quoteHTML(line) : line;
}
