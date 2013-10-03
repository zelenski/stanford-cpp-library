/*
 * File: tokenscanner.cpp
 * ----------------------
 * Implementation for the TokenScanner class.
 */

#include <cctype>
#include <iostream>
#include "error.h"
#include "tokenscanner.h"
#include "strlib.h"
#include "stack.h"
using namespace std;

TokenScanner::TokenScanner() {
   initScanner();
   setInput("");
}

TokenScanner::TokenScanner(string str) {
   initScanner();
   setInput(str);
}

TokenScanner::TokenScanner(istream & infile) {
   initScanner();
   setInput(infile);
}

TokenScanner::~TokenScanner() {
   if (stringInputFlag) delete isp;
}

void TokenScanner::setInput(string str) {
   stringInputFlag = true;
   buffer = str;
   isp = new istringstream(buffer);
   savedTokens = NULL;
}

void TokenScanner::setInput(istream & infile) {
   stringInputFlag = false;
   isp = &infile;
   savedTokens = NULL;
}

bool TokenScanner::hasMoreTokens() {
   string token = nextToken();
   saveToken(token);
   return (token != "");
}

string TokenScanner::nextToken() {
   if (savedTokens != NULL) {
      StringCell *cp = savedTokens;
      string token = cp->str;
      savedTokens = cp->link;
      delete cp;
      return token;
   }
   while (true) {
      if (ignoreWhitespaceFlag) skipSpaces();
      int ch = isp->get();
      if (ch == '/' && ignoreCommentsFlag) {
         ch = isp->get();
         if (ch == '/') {
            while (true) {
               ch = isp->get();
               if (ch == '\n' || ch == '\r' || ch == EOF) break;
            }
            continue;
         } else if (ch == '*') {
            int prev = EOF;
            while (true) {
               ch = isp->get();
               if (ch == EOF || (prev == '*' && ch == '/')) break;
               prev = ch;
            }
            continue;
         }
         if (ch != EOF) isp->unget();
         ch = '/';
      }
      if (ch == EOF) return "";
      if ((ch == '"' || ch == '\'') && scanStringsFlag) {
         isp->unget();
         return scanString();
      }
      if (isdigit(ch) && scanNumbersFlag) {
         isp->unget();
         return scanNumber();
      }
      if (isWordCharacter(ch)) {
         isp->unget();
         return scanWord();
      }
      string op = string(1, ch);
      while (isOperatorPrefix(op)) {
         ch = isp->get();
         if (ch == EOF) break;
         op += ch;
      }
      while (op.length() > 1 && !isOperator(op)) {
         isp->unget();
         op.erase(op.length() - 1, 1);
      }
      return op;
   }
}

void TokenScanner::saveToken(string token) {
   StringCell *cp = new StringCell;
   cp->str = token;
   cp->link = savedTokens;
   savedTokens = cp;
}

void TokenScanner::ignoreWhitespace() {
   ignoreWhitespaceFlag = true;
}

void TokenScanner::ignoreComments() {
   ignoreCommentsFlag = true;
}

void TokenScanner::scanNumbers() {
   scanNumbersFlag = true;
}

void TokenScanner::scanStrings() {
   scanStringsFlag = true;
}

void TokenScanner::addWordCharacters(string str) {
   wordChars += str;
}

void TokenScanner::addOperator(string op) {
   StringCell *cp = new StringCell;
   cp->str = op;
   cp->link = operators;
   operators = cp;
}

int TokenScanner::getPosition() const {
   if (savedTokens == NULL) {
      return int(isp->tellg());
   } else {
      return int(isp->tellg()) - savedTokens->str.length();
   }
   return -1;
}

bool TokenScanner::isWordCharacter(char ch) const {
   return isalnum(ch) || wordChars.find(ch) != string::npos;
};

void TokenScanner::verifyToken(string expected) {
   string token = nextToken();
   if (token != expected) {
      string msg = "Found \"" + token + "\"" +
                   " when expecting \"" + expected + "\"";
      error(msg);
   }
};

TokenType TokenScanner::getTokenType(string token) const {
   if (token == "") return TokenType(EOF);
   char ch = token[0];
   if (isspace(ch)) return SEPARATOR;
   if (ch == '"' || (ch == '\'' && token.length() > 1)) return STRING;
   if (isdigit(ch)) return NUMBER;
   if (isWordCharacter(ch)) return WORD;
   return OPERATOR;
};

string TokenScanner::getStringValue(string token) const {
   string str = "";
   int start = 0;
   int finish = token.length();
   if (finish > 1 && (token[0] == '"' || token[0] == '\'')) {
      start = 1;
      finish--;
   }
   for (int i = start; i < finish; i++) {
      char ch = token[i];
      if (ch == '\\') {
         ch = token[++i];
         if (isdigit(ch) || ch == 'x') {
            int base = 8;
            if (ch == 'x') {
               base = 16;
               i++;
            }
            int result = 0;
            int digit = 0;
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
            ch = char(result);
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
      str += ch;
   }
   return str;
}

int TokenScanner::getChar() {
   return isp->get();
}

void TokenScanner::ungetChar(int) {
   isp->unget();
}

/* Private methods */

void TokenScanner::initScanner() {
   ignoreWhitespaceFlag = false;
   ignoreCommentsFlag = false;
   scanNumbersFlag = false;
   scanStringsFlag = false;
   operators = NULL;
}

/*
 * Implementation notes: skipSpaces
 * --------------------------------
 * Advances the position of the scanner until the current character is
 * not a whitespace character.
 */

void TokenScanner::skipSpaces() {
   while (true) {
      int ch = isp->get();
      if (ch == EOF) return;
      if (!isspace(ch)) {
         isp->unget();
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

string TokenScanner::scanWord() {
   string token = "";
   while (true) {
      int ch = isp->get();
      if (ch == EOF) break;
      if (!isWordCharacter(ch)) {
         isp->unget();
         break;
      }
      token += char(ch);
   }
   return token;
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

string TokenScanner::scanNumber() {
   string token = "";
   NumberScannerState state = INITIAL_STATE;
   while (state != FINAL_STATE) {
      int ch = isp->get();
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
         } else if (!isdigit(ch)) {
            if (ch != EOF) isp->unget();
            state = FINAL_STATE;
         }
         break;
       case AFTER_DECIMAL_POINT:
         if (ch == 'E' || ch == 'e') {
            state = STARTING_EXPONENT;
         } else if (!isdigit(ch)) {
            if (ch != EOF) isp->unget();
            state = FINAL_STATE;
         }
         break;
       case STARTING_EXPONENT:
         if (ch == '+' || ch == '-') {
            state = FOUND_EXPONENT_SIGN;
         } else if (isdigit(ch)) {
            state = SCANNING_EXPONENT;
         } else {
            if (ch != EOF) isp->unget();
            isp->unget();
            state = FINAL_STATE;
         }
         break;
       case FOUND_EXPONENT_SIGN:
         if (isdigit(ch)) {
            state = SCANNING_EXPONENT;
         } else {
            if (ch != EOF) isp->unget();
            isp->unget();
            isp->unget();
            state = FINAL_STATE;
         }
         break;
       case SCANNING_EXPONENT:
         if (!isdigit(ch)) {
            if (ch != EOF) isp->unget();
            state = FINAL_STATE;
         }
         break;
       default:
         state = FINAL_STATE;
         break;
      }
      if (state != FINAL_STATE) {
         token += char(ch);
      }
   }
   return token;
}

/*
 * Implementation notes: scanString
 * --------------------------------
 * Reads and returns a quoted string from the scanner, continuing until
 * it scans the matching delimiter.  The scanner generates an error if
 * there is no closing quotation mark before the end of the input.
 */

string TokenScanner::scanString() {
   string token = "";
   char delim = isp->get();
   token += delim;
   bool escape = false;
   while (true) {
      int ch = isp->get();
      if (ch == EOF) error("TokenScanner found unterminated string");
      if (ch == delim && !escape) break;
      escape = (ch == '\\') && !escape;
      token += ch;
   }
   return token + delim;
}

/*
 * Implementation notes: isOperator, isOperatorPrefix
 * --------------------------------------------------
 * These methods search the list of operators and return true if the
 * specified operator is either in the list or a prefix of an operator
 * in the list, respectively.  This code could be made considerably more
 * efficient by implementing operators as a trie.
 */

bool TokenScanner::isOperator(string op) {
   for (StringCell *cp = operators; cp != NULL; cp = cp->link) {
      if (op == cp->str) return true;
   }
   return false;
}

bool TokenScanner::isOperatorPrefix(string op) {
   for (StringCell *cp = operators; cp != NULL; cp = cp->link) {
      if (startsWith(cp->str, op)) return true;
   }
   return false;
}
