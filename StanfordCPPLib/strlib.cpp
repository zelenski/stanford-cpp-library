/*
 * File: strlib.cpp
 * ----------------
 * This file implements the strlib.h interface.
 */

#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "error.h"
#include "strlib.h"
using namespace std;

/* Function prototypes */

/*
 * Implementation notes: numeric conversion
 * ----------------------------------------
 * These functions use the <sstream> library to perform the conversion.
 */

string integerToString(int n) {
   ostringstream stream;
   stream << n;
   return stream.str();
}

int stringToInteger(string str) {
   istringstream stream(str);
   int value;
   stream >> value >> ws;
   if (stream.fail() || !stream.eof()) {
      error("stringToInteger: Illegal integer format (" + str + ")");
   }
   return value;
}

string realToString(double d) {
   ostringstream stream;
   stream << uppercase << d;
   return stream.str();
}

double stringToReal(string str) {
   istringstream stream(str);
   double value;
   stream >> value >> ws;
   if (stream.fail() || !stream.eof()) {
      error("stringToReal: Illegal floating-point format (" + str + ")");
   }
   return value;
}

/*
 * Implementation notes: case conversion
 * -------------------------------------
 * The functions toUpperCase and toLowerCase return a new string whose
 * characters appear in the desired case. These implementations rely on
 * the fact that the characters in the string are copied when the
 * argument is passed to the function, which makes it possible to change
 * the case of the copy without affecting the original.
 */

string toUpperCase(string str) {
   int nChars = str.length();
   for (int i = 0; i < nChars; i++) {
      str[i] = toupper(str[i]);
   }
   return str;
}

string toLowerCase(string str) {
   int nChars = str.length();
   for (int i = 0; i < nChars; i++) {
      str[i] = tolower(str[i]);
   }
   return str;
}

/*
 * Implementation notes: equalsIgnoreCase
 * --------------------------------------
 * This implementation uses a for loop to cycle through the characters in
 * each string.  Converting each string to uppercase and then comparing
 * the results makes for a shorter but less efficient implementation.
 */

bool equalsIgnoreCase(string s1, string s2) {
   if (s1.length() != s2.length()) return false;
   int nChars = s1.length();
   for (int i = 0; i < nChars; i++) {
      if (tolower(s1[i]) != tolower(s2[i])) return false;
   }
   return true;
}

/*
 * Implementation notes: startsWith, endsWith
 * ------------------------------------------
 * These implementations are overloaded to allow the second argument to
 * be either a string or a character.
 */

bool startsWith(string str, string prefix) {
   if (str.length() < prefix.length()) return false;
   int nChars = prefix.length();
   for (int i = 0; i < nChars; i++) {
      if (str[i] != prefix[i]) return false;
   }
   return true;
}

bool startsWith(string str, char prefix) {
   return str.length() > 0 && str[0] == prefix;
}

bool endsWith(string str, string suffix) {
   int nChars = suffix.length();
   int start = str.length() - nChars;
   if (start < 0) return false;
   for (int i = 0; i < nChars; i++) {
      if (str[start + i] != suffix[i]) return false;
   }
   return true;
}

bool endsWith(string str, char suffix) {
   return str.length() > 0 && str[str.length() - 1] == suffix;
}

string trim(string str) {
   int finish = str.length() - 1;
   while (finish >= 0 && isspace(str[finish])) {
      finish--;
   }
   int start = 0;
   while (start <= finish && isspace(str[start])) {
      start++;
   }
   return str.substr(start, finish - start + 1);
}

/*
 * Implementation notes: readQuotedString and writeQuotedString
 * ------------------------------------------------------------
 * Most of the work in these functions has to do with escape sequences.
 */

static const string STRING_DELIMITERS = ",:)}]\n";

bool stringNeedsQuoting(const string & str) {
   int n = str.length();
   for (int i = 0; i < n; i++) {
      char ch = str[i];
      if (isspace(ch)) return false;
      if (STRING_DELIMITERS.find(ch) != string::npos) return true;
   }
   return false;
}

void readQuotedString(istream & is, string & str) {
   str = "";
   char ch;
   while (is.get(ch) && isspace(ch)) {
      /* Empty */
   }
   if (is.fail()) return;
   if (ch == '\'' || ch == '"') {
      char delim = ch;
      while (is.get(ch) && ch != delim) {
         if (is.fail()) error("Unterminated string");
         if (ch == '\\') {
            if (!is.get(ch)) error("Unterminated string");
            if (isdigit(ch) || ch == 'x') {
               int maxDigits = 3;
               int base = 8;
               if (ch == 'x') {
                  base = 16;
                  maxDigits = 2;
               }
               int result = 0;
               int digit = 0;
               for (int i = 0; i < maxDigits && ch != delim; i++) {
                  if (isdigit(ch)) {
                     digit = ch - '0';
                  } else if (base == 16 && isxdigit(ch)) {
                     digit = toupper(ch) - 'A' + 10;
                  } else {
                     break;
                  }
                  result = base * result + digit;
                  if (!is.get(ch)) error("Unterminated string");
               }
               ch = char(result);
               is.unget();
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
   } else {
      str += ch;
      int endTrim = 0;
      while (is.get(ch) && STRING_DELIMITERS.find(ch) == string::npos) {
         str += ch;
         if (!isspace(ch)) endTrim = str.length();
      }
      if (is) is.unget();
      str = str.substr(0, endTrim);
   }
}

void writeQuotedString(ostream & os, const string & str, bool forceQuotes) {
   if (!forceQuotes && stringNeedsQuoting(str)) forceQuotes = true;
   if (forceQuotes) os << '"';
   int len = str.length();
   for (int i = 0; i < len; i++) {
      char ch = str.at(i);
      switch (ch) {
       case '\a': os << "\\a"; break;
       case '\b': os << "\\b"; break;
       case '\f': os << "\\f"; break;
       case '\n': os << "\\n"; break;
       case '\r': os << "\\r"; break;
       case '\t': os << "\\t"; break;
       case '\v': os << "\\v"; break;
       case '\\': os << "\\\\"; break;
       default:
         if (isprint(ch) && ch != '"') {
            os << ch;
         } else {
            ostringstream oss;
            oss << oct << setw(3) << setfill('0') << (int(ch) & 0xFF);
            os << "\\" << oss.str();
         }
      }
   }
   if (forceQuotes) os << '"';
}
