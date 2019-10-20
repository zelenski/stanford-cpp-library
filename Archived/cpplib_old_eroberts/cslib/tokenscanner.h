/*
 * File: tokenscanner.h
 * --------------------
 * This interface exports a scanner facility that divides a string into
 * individual tokens.
 */

#ifndef _tokenscanner_h
#define _tokenscanner_h

#include "cslib.h"
#include "private/tokenpatch.h"

/*
 * Type: TokenType
 * ---------------
 * This enumerated type defines the values of the
 * <code>getTokenType</code> method.
 */

typedef enum { SEPARATOR, WORD, NUMBER, STRING, OPERATOR } TokenType;

/*
 * Type: TokenScanner
 * ------------------
 * This abstract type divides a string into individual tokens.  The
 * typical use of the <code>TokenScanner</code> ADT is illustrated by
 * the following pattern, which reads the tokens in the string variable
 * <code>input</code>:
 *
 *<pre>
 *    string token;
 *    TokenScanner scanner;
 *
 *    scanner = newTokenScanner();
 *    setInputString(scanner, input);
 *    while (hasMoreTokens(scanner)) {
 *       token = nextToken(scanner);
 *       . . . process the token . . .
 *       freeBlock(token);
 *    }
 *    freeTokenScanner(scanner);
 *</pre>
 *
 * The <code>TokenScanner</code> ADT exports several additional methods
 * that give clients more control over its behavior.  Those methods are
 * described individually in the documentation.
 */

typedef struct TokenScannerCDT *TokenScanner;

/*
 * Function: newTokenScanner
 * Usage: scanner = newTokenScanner();
 * -----------------------------------
 * Creates a new <code>TokenScanner</code> with an empty token stream.
 * Before using the scanner, an input stream must be set by calling
 * either <code>setInputString</code> or <code>setInputFile</code>.
 */

TokenScanner newTokenScanner(void);

/*
 * Function: freeTokenScanner
 * Usage: freeTokenScanner(scanner);
 * ---------------------------------
 * Frees the storage associated with the <code>TokenScanner</code>.
 */

void freeTokenScanner();

/*
 * Function: setInputString
 * Usage: setInputString(scanner, str);
 * ------------------------------------
 * Sets the token stream for this scanner to the specified string.
 */

void setInputString(TokenScanner scanner, string str);

/*
 * Function: setInputFile
 * Usage: setInputFile(scanner, infile);
 * -------------------------------------
 * Sets the token stream for this scanner to the specified file,
 * which must be open for input.
 */

void setInputFile(TokenScanner scanner, FILE *infile);

/*
 * Function: hasMoreTokens
 * Usage: if (hasMoreTokens(scanner)) . . .
 * ----------------------------------------
 * Returns <code>true</code> if there are additional tokens for this
 * scanner to read.
 */

bool hasMoreTokens(TokenScanner scanner);

/*
 * Function: nextToken
 * Usage: token = nextToken(scanner);
 * ----------------------------------
 * Returns the next token from this   If <code>nextToken</code>
 * is called when no tokens are available, it returns the empty string.
 */

string nextToken(TokenScanner scanner);

/*
 * Function: saveToken
 * Usage: saveToken(scanner, token);
 * ---------------------------------
 * Pushes the specified token back into this scanner's input stream.
 * On the next call to <code>nextToken</code>, the scanner will return
 * the saved token without reading any additional characters from the
 * token stream.
 */

void saveToken(TokenScanner scanner, string token);

/*
 * Function: getPosition
 * Usage: pos = getPosition(scanner);
 * ----------------------------------
 * Returns the current position of the scanner in the input stream.
 * If <code>saveToken</code> has been called, this position corresponds
 * to the beginning of the saved token.  If <code>saveToken</code> is
 * called more than once, <code>getPosition</code> returns -1.
 */

int getPosition(TokenScanner scanner);

/*
 * Function: ignoreWhitespace
 * Usage: ignoreWhitespace(scanner);
 * ---------------------------------
 * Tells the scanner to ignore whitespace characters.  By default,
 * the <code>nextToken</code> method treats whitespace characters
 * (typically spaces and tabs) just like any other punctuation mark
 * and returns them as single-character tokens.
 * Calling
 *
 *<pre>
 *    ignoreWhitespace(scanner);
 *</pre>
 *
 * changes this behavior so that the scanner ignore whitespace characters.
 */

void ignoreWhitespace(TokenScanner scanner);

/*
 * Function: ignoreComments
 * Usage: ignoreComments(scanner);
 * -------------------------------
 * Tells the scanner to ignore comments.  The scanner package recognizes
 * both the slash-star and slash-slash comment format from the C-based
 * family of languages.  Calling
 *
 *<pre>
 *    ignoreComments(scanner);
 *</pre>
 *
 * sets the parser to ignore comments.
 */

void ignoreComments(TokenScanner scanner);

/*
 * Function: scanNumbers
 * Usage: scanNumbers(scanner);
 * ----------------------------
 * Controls how the scanner treats tokens that begin with a digit.  By
 * default, the <code>nextToken</code> method treats numbers and letters
 * identically and therefore does not provide any special processing for
 * numbers.  Calling
 *
 *<pre>
 *    scanNumbers(scanner);
 *</pre>
 *
 * changes this behavior so that <code>nextToken</code> returns the
 * longest substring that can be interpreted as a real number.
 */

void scanNumbers(TokenScanner scanner);

/*
 * Function: scanStrings
 * Usage: scanStrings(scanner);
 * ----------------------------
 * Controls how the scanner treats tokens enclosed in quotation marks.  By
 * default, quotation marks (either single or double) are treated just like
 * any other punctuation character.  Calling
 *
 *<pre>
 *    scanStrings(scanner);
 *</pre>
 *
 * changes this assumption so that <code>nextToken</code> returns a single
 * token consisting of all characters through the matching quotation mark.
 * The quotation marks are returned as part of the scanned token so that
 * clients can differentiate strings from other token types.
 */

void scanStrings(TokenScanner scanner);

/*
 * Function: addWordCharacters
 * Usage: addWordCharacters(scanner, str);
 * ---------------------------------------
 * Adds the characters in <code>str</code> to the set of characters
 * legal in a <code>WORD</code> token.  For example, calling
 * <code>addWordCharacters("_")</code> adds the underscore to the
 * set of characters that are accepted as part of a word.
 */

void addWordCharacters(TokenScanner scanner, string str);

/*
 * Function: isWordCharacter
 * Usage: if (isWordCharacter(scanner, ch)) . . .
 * ----------------------------------------------
 * Returns <code>true</code> if the character is valid in a word.
 */

bool isWordCharacter(TokenScanner scanner, char ch);

/*
 * Function: addOperator
 * Usage: addOperator(scanner, op);
 * --------------------------------
 * Defines a new multicharacter operator.  Whenever you call
 * <code>nextToken</code> when the input stream contains operator
 * characters, the scanner returns the longest possible operator
 * string that can be read at that point.
 */

void addOperator(TokenScanner scanner, string op);

/*
 * Function: verifyToken
 * Usage: verifyToken(scanner, expected);
 * --------------------------------------
 * Reads the next token and makes sure it matches the string
 * <code>expected</code>.  If it does not, <code>verifyToken</code>
 * throws an error.
 */

void verifyToken(TokenScanner scanner, string expected);

/*
 * Function: getTokenType
 * Usage: type = getTokenType(scanner, token);
 * -------------------------------------------
 * Returns the type of this token.  This type will match one of the
 * following enumerated type constants: <code>EOF</code>,
 * <code>SEPARATOR</code>, <code>WORD</code>, <code>NUMBER</code>,
 * <code>STRING</code>, or <code>OPERATOR</code>.
 */

TokenType getTokenType(TokenScanner scanner, string token);

/*
 * Method: getChar
 * Usage: int ch = getChar(scanner);
 * ---------------------------------
 * Reads the next character from the scanner input stream.
 */

int getChar(TokenScanner scanner);

/*
 * Method: ungetChar
 * Usage: ungetChar(scanner, ch);
 * ------------------------------
 * Pushes the character <code>ch</code> back into the scanner stream.
 * The character must match the one that was read.
 */

void ungetChar(TokenScanner scanner, int ch);

/*
 * Function: getStringValue
 * Usage: str = getStringValue(token);
 * -----------------------------------
 * Returns the string value of a token.  This value is formed by removing
 * any surrounding quotation marks and replacing escape sequences by the
 * appropriate characters.
 */

string getStringValue(string token);

#endif
