/*
 * File: cppdoc.c
 * --------------
 * A quick-and-dirty HTML documentation system for StanfordCPPLib.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "cslib.h"
#include "filelib.h"
#include "foreach.h"
#include "hashmap.h"
#include "options.h"
#include "tokenscanner.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"

/* Constants */

#define DEFAULT_TITLE "C++ Library Documentation"

/* Option table */

static string OPTION_SPEC[] = {
   "-help",
   "-list",
   "-include <header>",
   "-module <name>",
   "-title <str>",
   "-icon <filename>",
   "-description <filename>",
   "-out <directory>",
   "-stylesheet <file>",
   "-elide <cumulative>",
   "-sortindex",
   "-classonly",
   "-generic",
   "-noconst",
   "-nostd",
   "-novirtual",
   "-notemplate",
   NULL
};

/* Types */

typedef struct {
   string name;
   string kind;
   string module;
   string type;
   string superclass;
   string *usage;
   string *html;
   string *code;
} *entryT;

/* Private variables */

static FILE *outfile;
static HashMap options;
static HashMap moduleTable;
static string stylesheet;
static bool noConstFlag;
static bool noStdFlag;
static bool noTemplateFlag;
static bool noVirtualFlag;
static bool classOnlyFlag;

/* Private function prototypes */

static void initStructures();
static void readDirectory(string dirname);
static void scanHeaderFile(string dirname, string filename);
static void dumpHTML(string dirname);
static void dumpHTMLIndex(void);
static void dumpModule(string module);
static void dumpClasses(string module);
static void dumpClass(entryT entry, Vector entries);
static void dumpHeader(string module, string href);
static void dumpTrailer(string module);
static void dumpBodyHeader(void);
static void dumpComment(entryT entry);
static void dumpIndexHeader(void);
static void dumpIndexTrailer(void);
static void dumpIndex(Vector entries, string kind, string type);
static void dumpIndexEntry(entryT entry);
static void dumpDetail(Vector entries, string kind, string type);
static void dumpDetailEntry(entryT entry);
static string getIndexKey(entryT entry);
static string getFirstSentence(entryT entry);
static int findSentenceEnd(string line);
static entryT findEntryOfKind(Vector entries, string kind, string type);
static int countEntriesOfKind(Vector entries, string kind, string type);
static void sortEntryVector(Vector entries);
static void listModules(void);
static void listEntry(entryT entry);
static void includeFile(Vector html, string filename);
static string makePlural(string word);
static bool isVowel(char ch);
static bool takesArguments(string kind);
static bool isStatementMacro(string kind);
static bool isEndOfCode(string line);
static string blueComments(string str);
static string quoteSpaces(string str);
static string replaceAll(string str, string pattern, string replacement);

/* Main entry point */

main(int argc, string argv[]) {
   string *files;
   int nFiles;

   options = parseOptions(argv + 1, OPTION_SPEC);
   files = getArgList(options);
   nFiles = stringArrayLength(files);
   stylesheet = getOption(options, "-stylesheet", "cppdoc.css");
   noStdFlag = containsKey(options, "-nostd");
   noConstFlag = containsKey(options, "-noconst");
   noTemplateFlag = containsKey(options, "-notemplate");
   noVirtualFlag = containsKey(options, "-novirtual");
   classOnlyFlag = containsKey(options, "-classonly");
   if (containsKey(options, "-help") || nFiles != 1) {
      showUsage("cppdoc <options> <dir>", OPTION_SPEC);
   } else {
      initStructures();
      readDirectory(files[0]);
      if (containsKey(options, "-list")) {
         listModules();
      } else {
         dumpHTML(files[0]);
         dumpHTMLIndex();
      }
   }
}

static void initStructures() {
   moduleTable = newHashMap();
}

static void readDirectory(string dirname) {
   string *list, name, key, elide;
   int i;

   list = listDirectory(dirname);
   elide = concat("+", concat(getOption(options, "-elide", ""), "+"));
   for (i = 0; list[i] != NULL; i++) {
      name = list[i];
      key = concat("+", concat(name, "+"));
      if (endsWith(name, ".h") && findString(key, elide, 0) == -1) {
         scanHeaderFile(dirname, list[i]);
      }
   }
}

#define INITIAL_STATE 0
#define COMMENT_START 1
#define USAGE_START   2
#define IN_USAGE      3
#define IN_HTML       4
#define IN_CODE       5

static void scanHeaderFile(string dirname, string filename) {
   FILE *infile;
   entryT entry;
   TokenScanner scanner;
   Vector entries, usage, html, code;
   string type, kind, name, line, superclass;
   int state, colon, start, finish, level, n;

   infile = fopen(concat(dirname, concat("/", filename)), "r");
   if (infile == NULL) error("Can't open file %s", filename);
   state = INITIAL_STATE;
   entries = newVector();
   usage = NULL;
   html = NULL;
   code = NULL;
   type = "";
   while (true) {
      line = readLine(infile);
      if (line == NULL) break;
      switch (state) {
       case INITIAL_STATE:
         if (stringEqual(line, "/*")) state = COMMENT_START;
         break;
       case COMMENT_START:
         colon = findString(": ", line, 0);
         if (colon < 0 || !startsWith(line, " * ")) {
            state = INITIAL_STATE;
         } else {
            name = substring(line, colon + 2, strlen(line));
            kind = substring(line, 3, colon - 1);
            if (stringEqual(kind, "Class") || stringEqual(kind, "Type")) {
               type = name;
            }
            entry = newBlock(entryT);
            entry->name = name;
            entry->kind = kind;
            entry->type = type;
            entry->module = filename;
            entry->superclass = NULL;
            add(entries, entry);
            state = USAGE_START;
         }
         break;
       case USAGE_START:
         if (startsWith(line, " */")) {
            state = INITIAL_STATE;
         } else if (startsWith(line, " * -----")) {
            html = newVector();
            state = IN_HTML;
         } else if (startsWith(line, " * Usage: ")) {
            usage = newVector();
            add(usage, trim(substring(line, 10, strlen(line))));
            state = IN_USAGE;
         }
         break;
       case IN_USAGE:
         if (startsWith(line, " * -----")) {
            html = newVector();
            state = IN_HTML;
         } else if (startsWith(line, " */")) {
            entry->usage = (string *) vectorToArray(usage);
            state = INITIAL_STATE;
         } else {
            add(usage, trim(substring(line, 10, strlen(line))));
         }
         break;
       case IN_HTML:
         if (startsWith(line, " */")) {
            entry->usage = (string *) vectorToArray(usage);
            entry->html = (string *) vectorToArray(html);
            code = newVector();
            state = IN_CODE;
            level = 0;
         } else if (findString("<include src=\"", line, 0) != -1) {
            start = findChar('"', line, 0) + 1;
            finish = findChar('"', line, start + 1) - 1;
            includeFile(html, substring(line, start, finish));
         } else {
            start = (line[2] == ' ') ? 3 : 2;
            add(html, substring(line, start, strlen(line)));
         }
         break;
       case IN_CODE:
         if (isEndOfCode(line) || (startsWith(line, "}") && level == 0)) {
            entry->code = (string *) vectorToArray(code);
            state = startsWith(line, "/*") ? COMMENT_START : INITIAL_STATE;
         } else if (startsWith(line, "class ")) {
            scanner = newTokenScanner();
            setInputString(scanner, line);
            ignoreWhitespace(scanner);
            nextToken(scanner);
            name = nextToken(scanner);
            if (stringEqual(nextToken(scanner), ":")) {
               if (stringEqual(nextToken(scanner), "public")) {
                  superclass = nextToken(scanner);
                  if (stringEqual(nextToken(scanner), "{")) {
                     entry->superclass = superclass;
                  }
               }
            }
            freeTokenScanner(scanner);
         } else {
            n = size(code);
            if (n > 0 && startsWith(line, "                ")) {
               line = concat(" ", trim(line));
               set(code, n - 1, concat(get(code, n - 1), line));
            } else {
               add(code, line);
            }
            if (endsWith(line, "{")) level++;
            if (startsWith(trim(line), "}")) level--;
         }
         break;
      }
   }
   put(moduleTable, filename, entries);
}

static void dumpHTML(string dirname) {
   string out, module, requestedModule, filename, cmd;

   out = getOption(options, "-out", "doc");
   createDirectoryPath(out);
   requestedModule = getOption(options, "-module", NULL);
   foreach (module in moduleTable) {
      if (requestedModule == NULL || stringEqual(module, requestedModule)) {
         filename = concat(getRoot(module), "-h.html");
         filename = concat(out, concat("/", filename));
         cmd = concat("fillcomments -html -out ", filename);
         if (containsKey(options, "-stylesheet")) {
            cmd = concat(cmd, concat(" -stylesheet ", stylesheet));
         }
         if (!classOnlyFlag) {
            filename = concat(dirname, concat("/", module));
            cmd = concat(cmd, concat(" ", filename));
            system(cmd);
            dumpModule(module);
         }
         dumpClasses(module);
      }
   }
}

static void dumpHTMLIndex() {
   int ch;
   string out, name, filename, description, title;
   Vector classes, modules, entries;
   FILE *infile;
   entryT entry;

   out = getOption(options, "-out", "doc");
   filename = concat(out, "/index.html");
   outfile = fopen(filename, "w");
   if (outfile == NULL) error("Can't open %s", getTail(filename));
   title = getOption(options, "-title", DEFAULT_TITLE);
   fprintf(outfile, "<html>\n");
   fprintf(outfile, "<head>\n");
   fprintf(outfile, "<title>%s</title>\n", title);
   if (stylesheet != NULL) {
      fprintf(outfile, "<link rel=\"stylesheet\" type=\"text/css\"");
      fprintf(outfile, " href=\"%s\" />\n", stylesheet);
   }
   fprintf(outfile, "</head>\n");
   fprintf(outfile, "<body>\n");
   dumpBodyHeader();
   description = getOption(options, "-description", "");
   if (stringLength(description) != 0) {
      infile = fopen(description, "r");
      if (infile == NULL) error("Can't find file %s", description);
      while ((ch = getc(infile)) != EOF) {
         putc(ch, outfile);
      }
      fclose(infile);
   }
   classes = newVector();
   modules = newVector();
   foreach (name in moduleTable) {
      entries = (Vector) get(moduleTable, name);
      entry = findEntryOfKind(entries, "File", NULL);
      if (entry != NULL) add(modules, entry);
      foreach (entry in entries) {
         if (stringEqual(entry->kind, "Class")) {
            add(classes, entry);
         }
      }
   }
   sortEntryVector(classes);
   sortEntryVector(modules);
   dumpIndexHeader();
   dumpIndex(classes, "Class", NULL);
   dumpIndexTrailer();
   dumpIndexHeader();
   dumpIndex(modules, "File", NULL);
   dumpIndexTrailer();
   fprintf(outfile, "</body>\n");
   fprintf(outfile, "</html>\n");
   fclose(outfile);
}

static void dumpModule(string module) {
   string out, filename;
   Vector entries;

   out = getOption(options, "-out", "doc");
   filename = defaultExtension(module, "*.html");
   filename = concat(out, concat("/", filename));
   outfile = fopen(filename, "w");
   if (outfile == NULL) error("Can't open %s", getTail(filename));
   entries = get(moduleTable, module);
   filename = concat(getRoot(module), "-h.html");
   dumpHeader(module, filename);
   dumpComment(findEntryOfKind(entries, "File", NULL));
   dumpIndexHeader();
   dumpIndex(entries, "Class", NULL);
   dumpIndex(entries, "Type", NULL);
   dumpIndex(entries, "Constant", NULL);
   dumpIndex(entries, "Statement", NULL);
   dumpIndex(entries, "Function", NULL);
   dumpIndexTrailer();
   dumpDetail(entries, "Type", NULL);
   dumpDetail(entries, "Statement", NULL);
   dumpDetail(entries, "Function", NULL);
   dumpTrailer(module);
   fclose(outfile);
}

static void dumpClasses(string module) {
   Vector entries;
   entryT entry;

   entries = get(moduleTable, module);
   foreach (entry in entries) {
      if (stringEqual(entry->kind, "Class")) {
         dumpClass(entry, entries);
      }
   }
}

static void dumpClass(entryT entry, Vector entries) {
   string className, out, filename, module, href;
   int angle;

   className = entry->name;
   out = getOption(options, "-out", "doc");
   angle = findChar('<', entry->name, 0);
   filename = (angle == -1) ? className : substring(className, 0, angle - 1);
   filename = concat(out, concat("/", concat(filename, "-class.html")));
   outfile = fopen(filename, "w");
   if (outfile == NULL) error("Can't open %s", getTail(filename));
   filename = concat(getRoot(entry->module), "-h.html");
   module = concat("class ", className);
   if (entry->superclass != NULL) {
      href = concat("<a href=\"", entry->superclass);
      href = concat(href, "-class.html\">");
      href = concat(href, entry->superclass);
      href = concat(href, "</a>");
      module = concat(module, concat(" : public ", href));
   }
   dumpHeader(module, filename);
   dumpComment(findEntryOfKind(entries, "Class", className));
   dumpIndexHeader();
   dumpIndex(entries, "Constructor", className);
   dumpIndex(entries, "Method", className);
   dumpIndex(entries, "Operator", className);
   dumpIndex(entries, "Function", className);
   dumpIndex(entries, "Statement", className);
   dumpIndex(entries, "Macro", className);
   dumpIndexTrailer();
   dumpDetail(entries, "Constructor", className);
   dumpDetail(entries, "Method", className);
   dumpDetail(entries, "Operator", className);
   dumpDetail(entries, "Function", className);
   dumpDetail(entries, "Statement", className);
   dumpDetail(entries, "Macro", className);
   dumpTrailer(className);
   fclose(outfile);
}

static void dumpHeader(string module, string href) {
   string header, title;

   if (findChar(':', module, 0) == -1) module = quoteHTML(module);
   title = module;
   if (startsWith(title, "class ")) title = title + 6;
   fprintf(outfile, "<html>\n");
   fprintf(outfile, "<head>\n");
   fprintf(outfile, "<title>%s</title>\n", title);
   if (stylesheet != NULL) {
      fprintf(outfile, "<link rel=\"stylesheet\" type=\"text/css\"");
      fprintf(outfile, " href=\"%s\" />\n", stylesheet);
   }
   fprintf(outfile, "</head>\n");
   fprintf(outfile, "<body>\n");
   dumpBodyHeader();
   if (startsWith(module, "class ")) {
      header = concat(substring(href, 0, strlen(href) - 8), ".h");
      href = concat(substring(header, 0, strlen(header) - 3), ".html");
      if (containsKey(options, "-include")) {
         header = getOption(options, "-include", NULL);
         href = concat(getRoot(header), ".html");
      }
      fprintf(outfile, "<div class=include><a href=\"%s\">", href);
      fprintf(outfile, "#include \"%s\"</a></div>\n", header);
      fprintf(outfile, "<h1 class=header><code>%s</code></h1>\n", module);
   } else {
      fprintf(outfile, "<h1 class=header><a href=\"%s\">", href);
      fprintf(outfile, "<code>%s</code></a></h1>\n", module);
   }
}

static void dumpTrailer(string module) {
   fprintf(outfile, "</body>\n");
   fprintf(outfile, "</html>\n");
}

static void dumpBodyHeader() {
   string icon, title;

   fprintf(outfile, "<table class=banner>\n");
   fprintf(outfile, "<tr>\n");
   icon = getOption(options, "-icon", "");
   title = getOption(options, "-title", DEFAULT_TITLE);
   if (stringLength(icon) > 0) {
      fprintf(outfile, "<td valign=middle>");
      fprintf(outfile, "<image src=\"%s\" height=32>", icon);
      fprintf(outfile, "</td>\n");
   }
   fprintf(outfile, "<td valign=middle><div class=bannerText>\n");
   fprintf(outfile, "%s\n", title);
   fprintf(outfile, "</div></td>\n");
   fprintf(outfile, "</tr></table>\n");
   fprintf(outfile, "<hr>\n");
}

static void dumpComment(entryT entry) {
   string *html;
   int i;

   if (entry == NULL) return;
   html = entry->html;
   for (i = 0; html[i] != NULL; i++) {
      fprintf(outfile, "%s\n", html[i]);
   }
}

static void dumpIndexHeader(void) {
   fprintf(outfile, "<table class=index width=100%%>\n");
}

static void dumpIndexTrailer(void) {
   fprintf(outfile, "</table>\n");
}

static void dumpIndex(Vector entries, string kind, string type) {
   Vector vec;
   entryT entry;
   string label;
   int nItems;


   nItems = countEntriesOfKind(entries, kind, type);
   if (nItems == 0) return;
   if (stringEqual(kind, "File")) {
      label = "Interface files";
   } else {
      label = (nItems == 1) ? kind : makePlural(kind);
   }
   fprintf(outfile, "<tr><td class=indexHead colspan=2>%s</td></tr>\n", label);
   vec = entries;
   if (containsKey(options, "-sortindex")) {
      vec = clone(entries);
      sortEntryVector(vec);
   }
   foreach (entry in vec) {
      if (stringEqual(kind, entry->kind)) {
         if (type == NULL || stringEqual(entry->type, type)) {
            dumpIndexEntry(entry);
         }
      }
   }
}

static void dumpIndexEntry(entryT entry) {
   string tag, synopsis;

   tag = getIndexKey(entry);
   synopsis = getFirstSentence(entry);
   fprintf(outfile, "<tr><td class=indexKey><nobr>%s&nbsp;</nobr></td>", tag);
   fprintf(outfile, "<td class=indexSynopsis width=100%%>");
   fprintf(outfile, "%s</td></tr>\n", synopsis);
}

static void dumpDetail(Vector entries, string kind, string type) {
   entryT entry;
   int nItems;

   nItems = countEntriesOfKind(entries, kind, type);
   if (nItems == 0) return;
   fprintf(outfile, "<h2>%s detail</h2>\n", kind);
   fprintf(outfile, "<hr>\n");
   foreach (entry in entries) {
      if (stringEqual(kind, entry->kind)) {
         if (type == NULL || stringEqual(entry->type, type)) {
            dumpDetailEntry(entry);
            fprintf(outfile, "<hr>\n");
         }
      }
   }
   fprintf(outfile, "</table>\n");
}

static void dumpDetailEntry(entryT entry) {
   string line;
   int i, start, finish, indent, pos, args, paren, cp;

   fprintf(outfile, "<a name=\"%s:%s\"></a>\n", entry->kind, entry->name);
   fprintf(outfile, "<pre class=detailCode>\n");
   if (isStatementMacro(entry->kind)) {
      for (i = 0; entry->usage[i] != NULL; i++) {
         fprintf(outfile, "%s\n", quoteHTML(entry->usage[i]));
      }
   } else {
      finish = stringArrayLength(entry->code) - 1;
      while (finish >= 0 && strlen(entry->code[finish]) == 0) {
         finish--;
      }
      start = 0;
      while (start <= finish && strlen(entry->code[start]) == 0) {
         start++;
      }
      if (start <= finish) {
         indent = 0;
         while (entry->code[start][indent] == ' ') {
            indent++;
         }
         for (i = start; i <= finish; i++) {
            line = entry->code[i];
            line = substring(line, indent, strlen(line));
            paren = findChar('(', line, 0);
            if (i == start && paren > 0 && containsKey(options, "-generic")) {
               for (cp = paren - 1; cp >= 0 && isalnum(line[cp]); cp--) {
                  /* Empty */
               }
               line = concat(substring(line, 0, cp),
                             concat(entry->name,
                                    substring(line, paren, 9999)));
            }
            if (noTemplateFlag) {
               if (strlen(line) == 0 || startsWith(line, "template ")) {
                  continue;
               }
            }
            line = replaceAll(line, "explicit ", "");
            if (noStdFlag) {
               line = replaceAll(line, "std::", "");
            }
            if (noVirtualFlag) {
               line = replaceAll(line, "virtual ", "");
               line = replaceAll(line, " = 0", "");
            }
            if (noConstFlag) {
               if (endsWith(line, " const;")) {
                  line = concat(substring(line, 0, strlen(line) - 8), ";");
               }
               args = (startsWith(line, "    ")) ? 4 : findChar('(', line, 0);
               while (start > 0) {
                  pos = findString("const ", line, start);
                  if (pos == -1) break;
                  line = concat(substring(line, 0, pos - 1),
                                substring(line, pos + 6, 9999));
                  pos = findChar('&', line, pos);
                  if (pos == -1) error("Expected &");
                  if (line[pos - 1] == ' ' && line[pos + 1] == ' ') {
                     line = concat(substring(line, 0, pos - 1),
                                   substring(line, pos + 2, 9999));
                  } else {
                     line = concat(substring(line, 0, pos - 1),
                                   substring(line, pos + 1, 9999));
                  }
               }
            }
            fprintf(outfile, "%s\n", blueComments(quoteHTML(line)));
         }
      }
   }
   fprintf(outfile, "</pre>\n");
   fprintf(outfile, "<div class=detailHTML>\n");
   for (i = 0; entry->html[i] != NULL; i++) {
      fprintf(outfile, "%s\n", entry->html[i]);
   }
   if (entry->usage == NULL || isStatementMacro(entry->kind)) {
      fprintf(outfile, "</div>\n");
   } else {
      fprintf(outfile, "<p>Usage:<br>\n");
      fprintf(outfile, "</div>\n");
      fprintf(outfile, "<pre class=usageCode>\n");
      for (i = 0; entry->usage[i] != NULL; i++) {
         fprintf(outfile, "%s\n", quoteHTML(entry->usage[i]));
      }
      fprintf(outfile, "</pre>\n");
   }
}

static string getIndexKey(entryT entry) {
   string name, key, line, href;
   int i, dot, start, finish;

   name = quoteHTML(entry->name);
   if (stringEqual(entry->kind, "Class")) {
      finish = findChar('<', entry->name, 0);
      if (finish == -1) finish = strlen(entry->name);
      href = concat("<a href=\"", substring(entry->name, 0, finish - 1));
      href = concat(href, "-class.html\">");
   } else if (stringEqual(entry->kind, "Constant")) {
      href = NULL;
   } else if (stringEqual(entry->kind, "File")) {
      dot = findChar('.', entry->name, 0);
      href = concat("<a href=\"", substring(entry->name, 0, dot - 1));
      href = concat(href, ".html\">");
   } else {
      href = concat("<a href=\"#", entry->kind);
      href = concat(href, concat(":", concat(name, "\">")));
   }
   if (takesArguments(entry->kind) && entry->usage != NULL) {
      key = "";
      for (i = 0; entry->usage[i] != NULL; i++) {
         line = entry->usage[i];
         start = findString(entry->name, line, 0);
         if (start == -1) {
            printf("No %s in usage line: %s\n", entry->name, line);
         }
         start = findChar('(', line, start);
         if (start == -1) {
            start = strlen(line);
            line = concat(line, "()");
         }
         finish = findChar(')', line, start);
         if (finish == -1) error("Illegal usage line: %s", line);
         if (strlen(key) > 0) key = concat(key, "<br>");
         key = concat(key, href);
         key = concat(key, name);
         key = concat(key, quoteSpaces(substring(line, start, finish)));
         key = concat(key, "</a>");
      }
      return key;
   }
   if (stringEqual(entry->kind, "Operator")) {
      name = entry->usage[0];
   }
   return (href == NULL) ? name : concat(href, concat(name, "</a>"));
}

static string getFirstSentence(entryT entry) {
   string str, line;
   int i, end;

   str = "";
   for (i = 0; entry->html[i] != NULL; i++) {
      line = entry->html[i];
      end = findSentenceEnd(line);
      if (end >= 0) return concat(str, substring(line, 0, end));
      str = concat(str, concat(line, " "));
   }
   return str;
}

static int findSentenceEnd(string line) {
   int i;

   for (i = 0; line[i] != '\0'; i++) {
      switch (line[i]) {
       case '.': case '!': case '?':
         if (line[i + 1] == '\0' || isspace(line[i + 1])) return i;
      }
   }
   return -1;
}

static entryT findEntryOfKind(Vector entries, string kind, string type) {
   entryT entry;

   foreach (entry in entries) {
      if (stringEqual(entry->kind, kind)) {
         if (type == NULL || stringEqual(entry->type, type)) {
            return entry;
         }
      }
   }
   return NULL;
}

static int countEntriesOfKind(Vector entries, string kind, string type) {
   entryT entry;
   int count;

   count = 0;
   foreach (entry in entries) {
      if (stringEqual(entry->kind, kind)) {
         if (type == NULL || stringEqual(entry->type, type)) {
            count++;
         }
      }
   }
   return count;
}

static void sortEntryVector(Vector entries) {
   int lh, rh, n, i;
   string s1, s2;
   entryT tmp;

   n = size(entries);
   for (lh = 0; lh < n; lh++) {
      rh = lh;
      for (i = lh + 1; i < n; i++) {
         s1 = toUpperCase(((entryT) get(entries, i))->name);
         s2 = toUpperCase(((entryT) get(entries, rh))->name);
         if (stringCompare(s1, s2) < 0) rh = i;
      }
      tmp = (entryT) get(entries, lh);
      set(entries, lh, (entryT) get(entries, rh));
      set(entries, rh, tmp);
   }
}

static void listModules(void) {
   string str, module;
   Vector entries;
   entryT entry;

   module = getOption(options, "-module", NULL);
   foreach (str in moduleTable) {
      if (module == NULL || stringEqual(str, module)) {
         printf("%s: \n", str);
         entries = (Vector) get(moduleTable, str);
         foreach (entry in entries) {
            listEntry(entry);
         }
         printf("\n");
      }
   }
}

static void listEntry(entryT entry) {
   printf("  %s: %s\n", entry->kind, entry->name);
}

static void includeFile(Vector html, string filename) {
   FILE *infile;
   string line;

   infile = fopen(filename, "r");
   if (infile == NULL) error("Can't open %s", filename);
   while (true) {
      line = readLine(infile);
      if (line == NULL) break;
      add(html, line);
   }
   fclose(infile);
}

static string makePlural(string word) {
   int len;

   len = strlen(word);
   if (endsWith(word, "s") || endsWith(word, "x")
               || endsWith(word, "z") || endsWith(word, "ch")
               || endsWith(word, "sh")) {
      return concat(word, "es");
   } else if (endsWith(word, "y") && len > 1
               && !isVowel(word[len - 2])) {
      return concat(substring(word, 0, len - 2), "ies");
   } else {
      return concat(word, "s");
   }
}

static bool isVowel(char ch) {
   switch (ch) {
    case 'A': case 'E': case 'I': case 'O': case 'U':
    case 'a': case 'e': case 'i': case 'o': case 'u':
      return true;
    default:
      return false;
   }
}

static bool takesArguments(string kind) {
   if (stringEqual(kind, "Function")) return true;
   if (stringEqual(kind, "Method")) return true;
   if (stringEqual(kind, "Macro")) return true;
   if (stringEqual(kind, "Statement")) return true;
   if (stringEqual(kind, "Constructor")) return true;
   if (stringEqual(kind, "Destructor")) return true;
   return false;
}

static bool isStatementMacro(string kind) {
   if (stringEqual(kind, "Statement")) return true;
   if (stringEqual(kind, "Macro")) return true;
   return false;
}

static bool isEndOfCode(string line) {
   if (startsWith(line, "#")) return true;
   if (startsWith(line, "public:")) return true;
   if (startsWith(line, "private:")) return true;
   if (startsWith(line, "/*")) return true;
   return false;
}

static string quoteSpaces(string str) {
   string result;
   int start, space;

   start = 0;
   result = "";
   while (true) {
      space = findChar(' ', str, start);
      if (space == -1) break;
      result = concat(result, substring(str, start, space - 1));
      result = concat(result, "&nbsp;");
      start = space + 1;
   }
   return concat(result, substring(str, start, strlen(str) - 1));
}

static string blueComments(string str) {
   string result;
   int start, index, finish;

   start = 0;
   result = "";
   while (true) {
      index = findString("/" "*", str, start);
      if (index == -1) break;
      finish = findString("*" "/", str, index);
      if (finish == -1) error("Illegal internal comment: %s", str);
      result = concat(result, substring(str, start, index - 1));
      result = concat(result, "<span class=comment>");
      result = concat(result, substring(str, index, finish + 1));
      result = concat(result, "</span>");
      start = finish + 2;
   }
   return concat(result, substring(str, start, strlen(str) - 1));
}

static string replaceAll(string str, string pattern, string replacement) {
   int pos;

   pos = 0;
   while (true) {
      pos = findString(pattern, str, pos);
      if (pos == -1) break;
      str = concat(substring(str, 0, pos - 1),
                   concat(replacement,
                          substring(str, pos + stringLength(pattern), 9999)));
      pos += stringLength(replacement);
   }
   return str;
}
