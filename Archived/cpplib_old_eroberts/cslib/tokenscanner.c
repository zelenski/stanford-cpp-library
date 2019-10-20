/*
 * File: tokenscanner.c
 * --------------------
 * Implementation for the TokenScanner class.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "cslib.h"
#include "strbuf.h"
#include "strlib.h"
#include "tokenscanner.h"
#include "unittest.h"

/*
 * Type: StringCell
 * ----------------
 * This type is used to construct linked lists of cells, which are used
 * to represent both the stack of saved tokens and the set of defined
 * operators.
 */

typedef struct StringCell {
   string str;
   struct StringCell *link;
} StringCell;

/*
 * Type: NumberScannerState
 * ------------------------
 * This type is used to enumerate the finite state machine used to read
 * numbers.
 */

typedef enum {
   INITIAL_STATE,
   BEFORE_DECIMAL_POINT,
   AFTER_DECIMAL_POINT,
   STARTING_EXPONENT,
   FOUND_EXPONENT_SIGN,
   SCANNING_EXPONENT,
   FINAL_STATE
} NumberScannerState;

/*
 * Type: TokenScannerCDT
 * ---------------------
 * This type represents the concrete structure of a scanner.
 */

struct TokenScannerCDT {
   string buffer;                   /* Source for string input      */
   int len;                         /* Length of buffer             */
   int cp;                          /* Current buffer position      */
   FILE *infile;                    /* Source for file input        */
   bool charIsSaved;                /* Char has been saved to file  */
   bool ignoreWhitespaceFlag;       /* Scanner ignores whitespace   */
   bool ignoreCommentsFlag;         /* Scanner ignores comments     */
   bool scanNumbersFlag;            /* Scanner parses numbers       */
   bool scanStringsFlag;            /* Scanner parses strings       */
   StringCell *savedTokens;         /* Stack of saved tokens        */
   StringCell *operators;           /* List of multichar operators  */
   StringBuffer wordChars;          /* Additional word characters   */
   StringBuffer savedChars;         /* Stack of saved characters    */
   StringBuffer sb;                 /* Buffer for creating strings  */
};

/* Private method prototypes */

static void initScanner(TokenScanner scanner);
static void skipSpaces(TokenScanner scanner);
static string scanWord(TokenScanner scanner);
static string scanNumber(TokenScanner scanner);
static string scanString(TokenScanner scanner);
static bool isOperator(TokenScanner scanner, string op);
static bool isOperatorPrefix(TokenScanner scanner, string op);
static void freeStringList(StringCell *cp);

/* Exported entries */

TokenScanner newTokenScanner(void) {
   TokenScanner scanner;

   scanner = newBlock(TokenScanner);
   initScanner(scanner);
   setInputString(scanner, "");
   return scanner;
}

void freeTokenScanner(TokenScanner scanner) {
   freeStringList(scanner->savedTokens);
   freeStringList(scanner->operators);
   freeStringBuffer(scanner->sb);
   freeStringBuffer(scanner->savedChars);
   freeStringBuffer(scanner->wordChars);
   freeBlock(scanner);
}

void setInputString(TokenScanner scanner, string str) {
   scanner->buffer = str;
   scanner->len = stringLength(str);
   scanner->cp = 0;
   scanner->infile = NULL;
   freeStringList(scanner->savedTokens);
   scanner->savedTokens = NULL;
}

void setInputFile(TokenScanner scanner, FILE *infile) {
   scanner->infile = infile;
   scanner->charIsSaved = false;
   freeStringList(scanner->savedTokens);
   scanner->savedTokens = NULL;
}

bool hasMoreTokens(TokenScanner scanner) {
   string token;

   token = nextToken(scanner);
   saveToken(scanner, token);
   return stringLength(token) != 0;
}

string nextToken(TokenScanner scanner) {
   StringCell *cp;
   string token;
   int ch, prev;

   if (scanner->savedTokens != NULL) {
      cp = scanner->savedTokens;
      token = cp->str;
      scanner->savedTokens = cp->link;
      freeBlock(cp);
      return token;
   }
   while (true) {
      if (scanner->ignoreWhitespaceFlag) skipSpaces(scanner);
      ch = getChar(scanner);
      if (ch == '/' && scanner->ignoreCommentsFlag) {
         ch = getChar(scanner);
         if (ch == '/') {
            while (true) {
               ch = getChar(scanner);
               if (ch == '\n' || ch == '\r' || ch == EOF) break;
            }
            continue;
         } else if (ch == '*') {
            prev = EOF;
            while (true) {
               ch = getChar(scanner);
               if (ch == EOF || (prev == '*' && ch == '/')) break;
               prev = ch;
            }
            continue;
         }
         ungetChar(scanner, ch);
         ch = '/';
      }
      if (ch == EOF) return copyString("");
      if ((ch == '"' || ch == '\'') && scanner->scanStringsFlag) {
         ungetChar(scanner, ch);
         return scanString(scanner);
      }
      if (isdigit(ch) && scanner->scanNumbersFlag) {
         ungetChar(scanner, ch);
         return scanNumber(scanner);
      }
      if (isWordCharacter(scanner, ch)) {
         ungetChar(scanner, ch);
         return scanWord(scanner);
      }
      clear(scanner->sb);
      pushChar(scanner->sb, ch);
      while (isOperatorPrefix(scanner, getString(scanner->sb))) {
         ch = getChar(scanner);
         if (ch == EOF) break;
         pushChar(scanner->sb, ch);
      }
      while (size(scanner->sb) > 1
             && !isOperator(scanner, getString(scanner->sb))) {
         ungetChar(scanner, ch);
         popChar(scanner->sb);
      }
      return copyString(getString(scanner->sb));
   }
}

void saveToken(TokenScanner scanner, string token) {
   StringCell *cp;

   cp = newBlock(StringCell *);
   cp->str = token;
   cp->link = scanner->savedTokens;
   scanner->savedTokens = cp;
}

void ignoreWhitespace(TokenScanner scanner) {
   scanner->ignoreWhitespaceFlag = true;
}

void ignoreComments(TokenScanner scanner) {
   scanner->ignoreCommentsFlag = true;
}

void scanNumbers(TokenScanner scanner) {
   scanner->scanNumbersFlag = true;
}

void scanStrings(TokenScanner scanner) {
   scanner->scanStringsFlag = true;
}

void addWordCharacters(TokenScanner scanner, string str) {
   appendString(scanner->wordChars, str);
}

void addOperator(TokenScanner scanner, string op) {
   StringCell *cp;

   cp = newBlock(StringCell *);
   cp->str = op;
   cp->link = scanner->operators;
   scanner->operators = cp;
}

int getPosition(TokenScanner scanner) {
   int pos;

   if (scanner->infile == NULL) {
      pos = scanner->cp;
   } else {
      pos = (int) ftell(scanner->infile);
   }
   if (scanner->savedTokens != NULL) {
      if (scanner->savedTokens->link != NULL) return -1;
      pos -= stringLength(scanner->savedTokens->str);
   }
   return pos;
}

bool isWordCharacter(TokenScanner scanner, char ch) {
   return isalnum(ch) || findChar(ch, getString(scanner->wordChars), 0) != -1;
}

void verifyToken(TokenScanner scanner, string expected) {
   string token;

   token = nextToken(scanner);
   if (!stringEqual(token, expected)) {
      if (stringEqual(token, "\n")) token = "end of line";
      if (stringEqual(expected, "\n")) expected = "end of line";
      error("Found \"%s\" when expecting \"%s\"", token, expected);
   }
   freeBlock(token);
}

TokenType getTokenType(TokenScanner scanner, string token) {
   char ch;

   if (stringLength(token) == 0) return (TokenType) EOF;
   ch = token[0];
   if (isspace(ch)) return SEPARATOR;
   if ((ch == '"' || ch == '\'') && stringLength(token) > 1) return STRING;
   if (isdigit(ch)) return NUMBER;
   if (isWordCharacter(scanner, ch)) return WORD;
   return OPERATOR;
}


int getChar(TokenScanner scanner) {
   if (!isEmpty(scanner->savedChars)) return popChar(scanner->savedChars);
   if (scanner->infile == NULL) {
      if (scanner->cp >= scanner->len) {
         return EOF;
      } else {
         return scanner->buffer[scanner->cp++];
      }
   } else {
      scanner->charIsSaved = false;
      return getc(scanner->infile);
   }
}

void ungetChar(TokenScanner scanner, int ch) {
   if (scanner->infile == NULL) {
      if (scanner->cp == 0) error("Internal error: pushback before start");
      scanner->cp--;
   } else {
      if (scanner->charIsSaved) {
         pushChar(scanner->savedChars, (char) ch);
      } else {
         ungetc(ch, scanner->infile);
         scanner->charIsSaved = true;
      }
   }
}

string getStringValue(string token) {
   StringBuffer sb;
   int i, base, start, finish, result, digit;
   string str;
   char ch;

   sb = newStringBuffer();
   start = 0;
   finish = stringLength(token);
   if (finish > 1 && (token[0] == '"' || token[0] == '\'')) {
      start = 1;
      finish--;
   }
   for (i = start; i < finish; i++) {
      ch = token[i];
      if (ch == '\\') {
         ch = token[++i];
         if (isdigit(ch) || ch == 'x') {
            base = 8;
            if (ch == 'x') {
               base = 16;
               i++;
            }
            result = 0;
            digit = 0;
            while (i < finish) {
               ch = token[i];
               if (isdigit(ch)) {
                  digit = ch - '0';
               } else if (isalpha(ch)) {
                  digit = toupper(ch) - 'A' + 10;
               } else {
                  digit = base;
               }
               if (digit >= base) break;
               result = base * result + digit;
               i++;
            }
            ch = (char) result;
            i--;
         } else {
            switch (ch) {
             case 'a': ch = '\a'; break;
             case 'b': ch = '\b'; break;
             case 'f': ch = '\f'; break;
             case 'n': ch = '\n'; break;
             case 'r': ch = '\r'; break;
             case 't': ch = '\t'; break;
             case 'v': ch = '\v'; break;
             case '"': ch = '"'; break;
             case '\'': ch = '\''; break;
             case '\\': ch = '\\'; break;
            }
         }
      }
      pushChar(sb, ch);
   }
   str = copyString(getString(sb));
   freeStringBuffer(sb);
   return str;
}

/* Private methods */

static void initScanner(TokenScanner scanner) {
   scanner->ignoreWhitespaceFlag = false;
   scanner->ignoreCommentsFlag = false;
   scanner->scanNumbersFlag = false;
   scanner->scanStringsFlag = false;
   scanner->operators = NULL;
   scanner->savedTokens = NULL;
   scanner->sb = newStringBuffer();
   scanner->savedChars = newStringBuffer();
   scanner->wordChars = newStringBuffer();
}

/*
 * Implementation notes: skipSpaces
 * --------------------------------
 * Advances the position of the scanner until the current character is
 * not a whitespace character.  The string str is used to check that
 * a whitespace character is not included explicitly as an operator.
 */

static void skipSpaces(TokenScanner scanner) {
   char str[2];
   int ch;

   while (true) {
      ch = getChar(scanner);
      if (ch == EOF) return;
      str[0] = ch;
      str[1] = '\0';
      if (!(isspace(ch) && !isOperator(scanner, str))) {
         ungetChar(scanner, ch);
         return;
      }
   }
}

/*
 * Implementation notes: scanWord
 * ------------------------------
 * Reads characters until the scanner reaches the end of a sequence
 * of word characters.
 */

static string scanWord(TokenScanner scanner) {
   int ch;

   clear(scanner->sb);
   while (true) {
      ch = getChar(scanner);
      if (ch == EOF) break;
      if (!isWordCharacter(scanner, ch)) {
         ungetChar(scanner, ch);
         break;
      }
      pushChar(scanner->sb, (char) ch);
   }
   return copyString(getString(scanner->sb));
}

/*
 * Implementation notes: scanNumber
 * --------------------------------
 * Reads characters until the scanner reaches the end of a legal number.
 * The function operates by simulating what computer scientists
 * call a finite-state machine.  The program uses the variable
 * <code>state</code> to record the history of the process and
 * determine what characters would be legal at this point in time.
 */

static string scanNumber(TokenScanner scanner) {
   NumberScannerState state;
   int ch, xch;

   clear(scanner->sb);
   state = INITIAL_STATE;
   while (state != FINAL_STATE) {
      ch = getChar(scanner);
      xch = 'e';
      switch (state) {
       case INITIAL_STATE:
         if (!isdigit(ch)) {
            error("Internal error: illegal call to scanNumber");
         }
         state = BEFORE_DECIMAL_POINT;
         break;
       case BEFORE_DECIMAL_POINT:
         if (ch == '.') {
            state = AFTER_DECIMAL_POINT;
         } else if (ch == 'E' || ch == 'e') {
            state = STARTING_EXPONENT;
            xch = ch;
         } else if (!isdigit(ch)) {
            if (ch != EOF) ungetChar(scanner, ch);
            state = FINAL_STATE;
         }
         break;
       case AFTER_DECIMAL_POINT:
         if (ch == 'E' || ch == 'e') {
            state = STARTING_EXPONENT;
            xch = ch;
         } else if (!isdigit(ch)) {
            if (ch != EOF) ungetChar(scanner, ch);
            state = FINAL_STATE;
         }
         break;
       case STARTING_EXPONENT:
         if (ch == '+' || ch == '-') {
            state = FOUND_EXPONENT_SIGN;
         } else if (isdigit(ch)) {
            state = SCANNING_EXPONENT;
         } else {
            if (ch != EOF) ungetChar(scanner, ch);
            ungetChar(scanner, ch);
            state = FINAL_STATE;
         }
         break;
       case FOUND_EXPONENT_SIGN:
         if (isdigit(ch)) {
            state = SCANNING_EXPONENT;
         } else {
            if (ch != EOF) ungetChar(scanner, ch);
            ungetChar(scanner, ch);
            ungetChar(scanner, ch);
            state = FINAL_STATE;
         }
         break;
       case SCANNING_EXPONENT:
         if (!isdigit(ch)) {
            if (ch != EOF) ungetChar(scanner, ch);
            state = FINAL_STATE;
         }
         break;
       default:
         state = FINAL_STATE;
         break;
      }
      if (state != FINAL_STATE) {
         pushChar(scanner->sb, (char) ch);
      }
   }
   return copyString(getString(scanner->sb));
}

/*
 * Implementation notes: scanString
 * --------------------------------
 * Reads and returns a quoted string from the scanner, continuing until
 * it scans the matching delimiter.  The scanner generates an error if
 * there is no closing quotation mark before the end of the input.
 */

static string scanString(TokenScanner scanner) {
   bool escape;
   char delim;
   int ch;

   clear(scanner->sb);
   delim = getChar(scanner);
   pushChar(scanner->sb, delim);
   escape = false;
   while (true) {
      ch = getChar(scanner);
      if (ch == EOF) error("TokenScanner found unterminated string");
      if (ch == delim && !escape) break;
      escape = (ch == '\\') && !escape;
      pushChar(scanner->sb, (char) ch);
   }
   pushChar(scanner->sb, delim);
   return copyString(getString(scanner->sb));
}

/*
 * Implementation notes: isOperator, isOperatorPrefix
 * --------------------------------------------------
 * These methods search the list of operators and return true if the
 * specified operator is either in the list or a prefix of an operator
 * in the list, respectively.  This code could be made considerably more
 * efficient by implementing operators as a trie.
 */

static bool isOperator(TokenScanner scanner, string op) {
   StringCell *cp;

   for (cp = scanner->operators; cp != NULL; cp = cp->link) {
      if (stringEqual(op, cp->str)) return true;
   }
   return false;
}

static bool isOperatorPrefix(TokenScanner scanner, string op) {
   StringCell *cp;

   for (cp = scanner->operators; cp != NULL; cp = cp->link) {
      if (startsWith(cp->str, op)) return true;
   }
   return false;
}

static void freeStringList(StringCell *cp) {
   StringCell *next;
   while (cp != NULL) {
      next = cp->link;
      freeBlock(cp);
      cp = next;
   }
}

/**********************************************************************/
/* Unit test for the tokenscanner module                              */
/**********************************************************************/

#ifndef _NOTEST_

/* Unit test */

static void testStringScanner(void);
static void testStreamScanner(void);
static void testScannerNoOptions(void);
static void testScannerLanguageOptions(void);
static void testScanNumbers(void);
static void testScanEscapeSequences(void);

void testTokenScannerModule(void) {
   testStringScanner();
   testStreamScanner();
   testScanEscapeSequences();
}

static void testStringScanner(void) {
   testScannerNoOptions();
   testScannerLanguageOptions();
   testScanNumbers();
}

static void testStreamScanner(void) {
   TokenScanner scanner;
   FILE *infile;

   trace(scanner = newTokenScanner());
   trace(infile = fopen("tokenscanner.h", "r"));
   test(infile == NULL, false);
   if (infile == NULL) return;
   trace(setInputFile(scanner, infile));
   test(hasMoreTokens(scanner), true);
   test(nextToken(scanner), "/");
   test(nextToken(scanner), "*");
   test(hasMoreTokens(scanner), true);
   test(nextToken(scanner), "\n");
   test(hasMoreTokens(scanner), true);
   test(nextToken(scanner), " ");
   test(nextToken(scanner), "*");
   test(nextToken(scanner), " ");
   test(nextToken(scanner), "File");
   trace(ignoreWhitespace(scanner));
   trace(scanStrings(scanner));
   test(nextToken(scanner), ":");
   test(nextToken(scanner), "tokenscanner");
   test(nextToken(scanner), ".");
   test(nextToken(scanner), "h");
   test(nextToken(scanner), "*");
   test(nextToken(scanner), "-");
   trace(fclose(infile));
}

static void testScannerNoOptions(void) {
   TokenScanner scanner;
   string token;

   trace(scanner = newTokenScanner());
   trace(setInputString(scanner, "cout << \"pi = \" << 3.14159265 << endl;"));
   test(hasMoreTokens(scanner), true);
   test(token = nextToken(scanner), "cout");
   test(getTokenType(scanner, token), (int) WORD);
   test(token = nextToken(scanner), " ");
   test(getTokenType(scanner, token), (int) SEPARATOR);
   test(token = nextToken(scanner), "<");
   test(getTokenType(scanner, token), (int) OPERATOR);
   test(nextToken(scanner), "<");
   test(nextToken(scanner), " ");
   test(nextToken(scanner), "\"");
   test(nextToken(scanner), "pi");
   test(nextToken(scanner), " ");
   test(nextToken(scanner), "=");
   test(nextToken(scanner), " ");
   test(nextToken(scanner), "\"");
   test(nextToken(scanner), " ");
   test(nextToken(scanner), "<");
   test(nextToken(scanner), "<");
   test(nextToken(scanner), " ");
   test(token = nextToken(scanner), "3");
   test(getTokenType(scanner, token), (int) NUMBER);
   test(nextToken(scanner), ".");
   test(nextToken(scanner), "14159265");
   test(nextToken(scanner), " ");
   test(nextToken(scanner), "<");
   test(nextToken(scanner), "<");
   test(nextToken(scanner), " ");
   test(nextToken(scanner), "endl");
   test(nextToken(scanner), ";");
   test(hasMoreTokens(scanner), false);
   test(token = nextToken(scanner), "");
   test(getTokenType(scanner, token), (int) EOF);
}

static void testScannerLanguageOptions(void) {
   TokenScanner scanner;
   string token;

   trace(scanner = newTokenScanner());
   trace(ignoreWhitespace(scanner));
   trace(scanStrings(scanner));
   trace(scanNumbers(scanner));
   trace(addWordCharacters(scanner, "_"));
   trace(addOperator(scanner, "=="));
   trace(addOperator(scanner, "!="));
   trace(addOperator(scanner, "<="));
   trace(addOperator(scanner, ">="));
   trace(addOperator(scanner, "<<"));
   trace(addOperator(scanner, ">>"));
   trace(addOperator(scanner, "++"));
   trace(addOperator(scanner, "--"));
   trace(addOperator(scanner, "->"));
   trace(addOperator(scanner, "&&"));
   trace(addOperator(scanner, "||"));
   trace(setInputString(scanner, "cout << \"pi = \" << 3.14159265 << endl;"));
   test(hasMoreTokens(scanner), true);
   test(nextToken(scanner), "cout");
   test(nextToken(scanner), "<<");
   test(token = nextToken(scanner), "\"pi = \"");
   test(getTokenType(scanner, token), (int) STRING);
   test(nextToken(scanner), "<<");
   test(token = nextToken(scanner), "3.14159265");
   test(getTokenType(scanner, token), (int) NUMBER);
   test(nextToken(scanner), "<<");
   test(nextToken(scanner), "endl");
   test(nextToken(scanner), ";");
   test(hasMoreTokens(scanner), false);
   test(nextToken(scanner), "");
   trace(setInputString(scanner, "if (a_1<=b+++_c &&&x!=4)"));
   test(nextToken(scanner), "if");
   test(nextToken(scanner), "(");
   test(token = nextToken(scanner), "a_1");
   test(getTokenType(scanner, token), (int) WORD);
   test(nextToken(scanner), "<=");
   test(nextToken(scanner), "b");
   test(nextToken(scanner), "++");
   test(nextToken(scanner), "+");
   test(token = nextToken(scanner), "_c");
   test(getTokenType(scanner, token), (int) WORD);
   test(nextToken(scanner), "&&");
   test(nextToken(scanner), "&");
   test(nextToken(scanner), "x");
   test(nextToken(scanner), "!=");
   test(nextToken(scanner), "4");
   test(nextToken(scanner), ")");
   test(hasMoreTokens(scanner), false);
   test(nextToken(scanner), "");
}

static void testScanNumbers(void) {
   TokenScanner scanner;

   trace(scanner = newTokenScanner());
   trace(ignoreWhitespace(scanner));
   trace(scanNumbers(scanner));
   trace(setInputString(scanner, "42 73x 3.14159265 6.28E-25"));
   test(getChar(scanner), '4');
   trace(ungetChar(scanner, '4'));
   test(nextToken(scanner), "42");
   test(nextToken(scanner), "73");
   test(nextToken(scanner), "x");
   test(nextToken(scanner), "3.14159265");
   test(nextToken(scanner), "6.28E-25");
   test(hasMoreTokens(scanner), false);
}

static void testScanEscapeSequences(void) {
   TokenScanner scanner;
   string token;

   trace(scanner = newTokenScanner());
   trace(scanStrings(scanner));
   trace(setInputString(scanner, "'A'"));
   trace(token = nextToken(scanner));
   test(token, "'A'");
   test(getStringValue(token), "A");
   trace(setInputString(scanner, "\"Quoted String\""));
   trace(token = nextToken(scanner));
   test(token, "\"Quoted String\"");
   test(getStringValue(token), "Quoted String");
   trace(setInputString(scanner, "\"\\\"Bother\\\" said Pooh.\""));
   trace(token = nextToken(scanner));
   test(token, "\"\\\"Bother\\\" said Pooh.\"");
   test(getStringValue(token), "\"Bother\" said Pooh.");
   trace(setInputString(scanner, "\"\\xFF\\007\\a\\t\""));
   trace(token = nextToken(scanner));
   test(token, "\"\\xFF\\007\\a\\t\"");
   test(getStringValue(token), "\377\007\a\t");
}

#endif
