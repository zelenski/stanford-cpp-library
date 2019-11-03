/*
 * File: tokenscanner.cpp
 * ----------------------
 * Implementation for the TokenScanner class.
 * 
 * @version 2016/11/26
 * - added getInput method
 * - replaced occurrences of string with const string& for efficiency
 * - alphabetized method ordering
 * - added operator << for printing a scanner
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include "tokenscanner.h"
#include <cctype>
#include <iostream>
#include "error.h"
#include "strlib.h"
#include "stack.h"

TokenScanner::TokenScanner() {
    initScanner();
    setInput("");
}

TokenScanner::TokenScanner(std::istream& infile) {
    initScanner();
    setInput(infile);
}

TokenScanner::TokenScanner(const std::string& str) {
    initScanner();
    setInput(str);
}

TokenScanner::~TokenScanner() {
    if (stringInputFlag) {
        delete isp;
    }
}

void TokenScanner::addOperator(const std::string& op) {
    StringCell* cp = new StringCell;
    cp->str = op;
    cp->link = operators;
    operators = cp;
}

void TokenScanner::addWordCharacters(const std::string& str) {
    wordChars += str;
}

int TokenScanner::getChar() {
    return isp->get();
}

std::string TokenScanner::getInput() const {
    return buffer;
}

int TokenScanner::getPosition() const {
    if (!savedTokens) {
        return int(isp->tellg());
    } else {
        return int(isp->tellg()) - savedTokens->str.length();
    }
}

std::string TokenScanner::getStringValue(const std::string& token) const {
    std::string str = "";
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
                    if (digit >= base) {
                        break;
                    }
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

TokenScanner::TokenType TokenScanner::getTokenType(const std::string& token) const {
    if (token.empty()) {
        return TokenType(EOF);
    }

    char ch = token[0];
    if (isspace(ch)) {
        return SEPARATOR;
    } else if (ch == '"' || (ch == '\'' && token.length() > 1)) {
        return STRING;
    } else if (isdigit(ch)) {
        return NUMBER;
    } else if (isWordCharacter(ch)) {
        return WORD;
    } else {
        return OPERATOR;
    }
}

bool TokenScanner::hasMoreTokens() {
    std::string token = nextToken();
    saveToken(token);
    return !token.empty();
}

void TokenScanner::ignoreComments() {
    ignoreCommentsFlag = true;
}

void TokenScanner::ignoreWhitespace() {
    ignoreWhitespaceFlag = true;
}

bool TokenScanner::isWordCharacter(char ch) const {
    return isalnum(ch) || wordChars.find(ch) != std::string::npos;
}

std::string TokenScanner::nextToken() {
    if (savedTokens) {
        StringCell* cp = savedTokens;
        std::string token = cp->str;
        savedTokens = cp->link;
        delete cp;
        return token;
    }

    while (true) {
        if (ignoreWhitespaceFlag) {
            skipSpaces();
        }
        int ch = isp->get();
        if (ch == '/' && ignoreCommentsFlag) {
            ch = isp->get();
            if (ch == '/') {
                while (true) {
                    ch = isp->get();
                    if (ch == '\n' || ch == '\r' || ch == EOF) {
                        break;
                    }
                }
                continue;
            } else if (ch == '*') {
                int prev = EOF;
                while (true) {
                    ch = isp->get();
                    if (ch == EOF || (prev == '*' && ch == '/')) {
                        break;
                    }
                    prev = ch;
                }
                continue;
            }
            if (ch != EOF) {
                isp->unget();
            }
            ch = '/';
        }
        if (ch == EOF) {
            return "";
        }
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
        std::string op = std::string(1, ch);
        while (isOperatorPrefix(op)) {
            ch = isp->get();
            if (ch == EOF) {
                break;
            }
            op += ch;
        }
        while (op.length() > 1 && !isOperator(op)) {
            isp->unget();
            op.erase(op.length() - 1, 1);
        }
        return op;
    }
}

void TokenScanner::saveToken(const std::string& token) {
    StringCell* cp = new StringCell;
    cp->str = token;
    cp->link = savedTokens;
    savedTokens = cp;
}

void TokenScanner::scanNumbers() {
    scanNumbersFlag = true;
}

void TokenScanner::scanStrings() {
    scanStringsFlag = true;
}

void TokenScanner::setInput(std::istream& infile) {
    stringInputFlag = false;
    isp = &infile;
    savedTokens = nullptr;
}

void TokenScanner::setInput(const std::string& str) {
    stringInputFlag = true;
    buffer = str;
    isp = new std::istringstream(buffer);
    savedTokens = nullptr;
}

void TokenScanner::ungetChar(int) {
    isp->unget();
}

void TokenScanner::verifyToken(const std::string& expected) {
    std::string token = nextToken();
    if (token != expected) {
        std::string msg = "TokenScanner::verifyToken: Found \"" + token + "\""
                + " when expecting \"" + expected + "\"";
        if (!buffer.empty()) {
            msg += "\ninput = \"" + buffer + "\"";
        }
        error(msg);
    }
}

/* Private methods */

void TokenScanner::initScanner() {
    ignoreWhitespaceFlag = false;
    ignoreCommentsFlag = false;
    scanNumbersFlag = false;
    scanStringsFlag = false;
    operators = nullptr;
}

/*
 * Implementation notes: isOperator, isOperatorPrefix
 * --------------------------------------------------
 * These methods search the list of operators and return true if the
 * specified operator is either in the list or a prefix of an operator
 * in the list, respectively.  This code could be made considerably more
 * efficient by implementing operators as a trie.
 */
bool TokenScanner::isOperator(const std::string& op) {
    for (StringCell *cp = operators; cp != nullptr; cp = cp->link) {
        if (op == cp->str) {
            return true;
        }
    }
    return false;
}

bool TokenScanner::isOperatorPrefix(const std::string& op) {
    for (StringCell* cp = operators; cp != nullptr; cp = cp->link) {
        if (startsWith(cp->str, op)) {
            return true;
        }
    }
    return false;
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
std::string TokenScanner::scanNumber() {
    std::string token = "";
    NumberScannerState state = INITIAL_STATE;
    while (state != FINAL_STATE) {
        int ch = isp->get();
        switch (state) {
        case INITIAL_STATE:
            if (!isdigit(ch)) {
                error("TokenScanner::scanNumber: internal error: illegal call");
            }
            state = BEFORE_DECIMAL_POINT;
            break;
        case BEFORE_DECIMAL_POINT:
            if (ch == '.') {
                state = AFTER_DECIMAL_POINT;
            } else if (ch == 'E' || ch == 'e') {
                state = STARTING_EXPONENT;
            } else if (!isdigit(ch)) {
                if (ch != EOF) {
                    isp->unget();
                }
                state = FINAL_STATE;
            }
            break;
        case AFTER_DECIMAL_POINT:
            if (ch == 'E' || ch == 'e') {
                state = STARTING_EXPONENT;
            } else if (!isdigit(ch)) {
                if (ch != EOF) {
                    isp->unget();
                }
                state = FINAL_STATE;
            }
            break;
        case STARTING_EXPONENT:
            if (ch == '+' || ch == '-') {
                state = FOUND_EXPONENT_SIGN;
            } else if (isdigit(ch)) {
                state = SCANNING_EXPONENT;
            } else {
                if (ch != EOF) {
                    isp->unget();
                }
                isp->unget();
                state = FINAL_STATE;
            }
            break;
        case FOUND_EXPONENT_SIGN:
            if (isdigit(ch)) {
                state = SCANNING_EXPONENT;
            } else {
                if (ch != EOF) {
                    isp->unget();
                }
                isp->unget();
                isp->unget();
                state = FINAL_STATE;
            }
            break;
        case SCANNING_EXPONENT:
            if (!isdigit(ch)) {
                if (ch != EOF) {
                    isp->unget();
                }
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
std::string TokenScanner::scanString() {
    std::string token = "";
    char delim = isp->get();
    token += delim;
    bool escape = false;
    while (true) {
        int ch = isp->get();
        if (ch == EOF) {
            error("TokenScanner::scanString: found unterminated string");
        }
        if (ch == delim && !escape) {
            break;
        }
        escape = (ch == '\\') && !escape;
        token += ch;
    }
    return token + delim;
}

/*
 * Implementation notes: scanWord
 * ------------------------------
 * Reads characters until the scanner reaches the end of a sequence
 * of word characters.
 */
std::string TokenScanner::scanWord() {
    std::string token = "";
    while (true) {
        int ch = isp->get();
        if (ch == EOF) {
            break;
        }
        if (!isWordCharacter(ch)) {
            isp->unget();
            break;
        }
        token += char(ch);
    }
    return token;
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
        if (ch == EOF) {
            return;
        }
        if (!isspace(ch)) {
            isp->unget();
            return;
        }
    }
}

std::ostream& operator <<(std::ostream& out, const TokenScanner& scanner) {
    out << "TokenScanner{";
    bool first = true;
    if (!scanner.buffer.empty()) {
        out << "input=\"" << scanner.buffer << "\"";
        first = false;
    }
    out << (first ? "" : ",") << "position=" << scanner.getPosition();
    first = false;

    if (scanner.scanNumbersFlag) {
        out << (first ? "" : ",") << "scanNumbers";
        first = false;
    }
    if (scanner.scanStringsFlag) {
        out << (first ? "" : ",") << "scanStrings";
        first = false;
    }
    if (!scanner.wordChars.empty()) {
        out << (first ? "" : ",") << "wordChars=[" << scanner.wordChars << "]";
        first = false;
    }
    if (scanner.ignoreWhitespaceFlag) {
        out << (first ? "" : ",") << "ignoreWhitespace";
        first = false;
    }
    if (scanner.ignoreCommentsFlag) {
        out << (first ? "" : ",") << "ignoreComments";
        // first = false;
    }
    out << "}";
    return out;
}
