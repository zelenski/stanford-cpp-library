/*
 * File: simpio.cpp
 * ----------------
 * This file implements the simpio.h interface.
 * 
 * @version 2015/07/05
 * - increased visibility of appendSpace function used by various IO
 *   prompting functions (no longer static)
 * @version 2014/10/19
 * - alphabetized functions
 * - converted many funcs to take const string& rather than string for efficiency
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include "simpio.h"
#include "strlib.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static const std::string GETINTEGER_DEFAULT_PROMPT = "Enter an integer: ";
static const std::string GETINTEGER_DEFAULT_REPROMPT = "Illegal integer format. Try again.";
static const std::string GETREAL_DEFAULT_PROMPT = "Enter a number: ";
static const std::string GETREAL_DEFAULT_REPROMPT = "Illegal numeric format. Try again.";
static const std::string GETYESORNO_DEFAULT_PROMPT = "Try again: ";
static const std::string GETYESORNO_DEFAULT_REPROMPT = "Please type a word that starts with 'Y' or 'N'.";

/*
 * Implementation notes: getInteger, getReal
 * -----------------------------------------
 * Each of these functions reads a complete input line and then uses the
 * <sstream> library to parse that line into a value of the desired type.
 * If that fails, the implementation asks the user for a new value.
 */

int getInteger(const std::string& prompt,
               const std::string& reprompt) {
    std::string promptCopy = prompt;
    appendSpace(promptCopy);
    int value;
    while (true) {
        std::cout << promptCopy;
        std::string line;
        getline(std::cin, line);
        trimInPlace(line);
        std::istringstream stream(line);
        stream >> value;
        if (!stream.fail() && stream.eof()) {
            break;
        }
        std::cout << (reprompt.empty() ? GETINTEGER_DEFAULT_REPROMPT : reprompt) << std::endl;
        if (promptCopy.empty()) {
            promptCopy = GETINTEGER_DEFAULT_PROMPT;
        }
    }
    return value;
}

/*
 * Implementation notes: getLine
 * -----------------------------
 * The getLine function simply combines the process of displaying a
 * prompt and reading an input line into a single call.  The primary
 * reason for including this function in the library is to ensure
 * that the process of reading integers, floating-point numbers, and
 * strings remains as consistent as possible.
 */
std::string getLine(const std::string& prompt) {
    std::string line;
    getLine(prompt, line);
    return line;
}

void getLine(const std::string& prompt,
             std::string& out) {
    std::string promptCopy = prompt;
    appendSpace(promptCopy);
    std::cout << promptCopy;
    getline(std::cin, out);
}

void getLine(std::istream& input,
             std::string& out) {
    getline(input, out);
}

double getReal(const std::string& prompt,
               const std::string& reprompt) {
    std::string promptCopy = prompt;
    appendSpace(promptCopy);
    double value;
    while (true) {
        std::cout << promptCopy;
        std::string line;
        getline(std::cin, line);
        trimInPlace(line);
        std::istringstream stream(line);
        stream >> value;
        if (!stream.fail() && stream.eof()) {
            break;
        }
        std::cout << (reprompt.empty() ? GETREAL_DEFAULT_REPROMPT : reprompt) << std::endl;
        if (promptCopy.empty()) {
            promptCopy = GETREAL_DEFAULT_PROMPT;
        }
    }
    return value;
}

bool getYesOrNo(const std::string& prompt,
                const std::string& reprompt,
                const std::string& defaultValue) {
    std::string promptCopy = prompt;
    appendSpace(promptCopy);
    bool value;
    while (true) {
        std::cout << promptCopy;
        std::string line;
        getline(std::cin, line);
        if (line.empty()) {
            line = defaultValue;
        }
        if ((int) line.length() > 0) {
            char first = tolower(line[0]);
            if (first == 'y') {
                value = true;
                break;
            } else if (first == 'n') {
                value = false;
                break;
            }
        }
        std::cout << (reprompt.empty() ? GETYESORNO_DEFAULT_REPROMPT : reprompt) << std::endl;
        if (promptCopy.empty()) {
            promptCopy = GETYESORNO_DEFAULT_PROMPT;
        }
    }
    return value;
}

void appendSpace(std::string& prompt) {
    if (!prompt.empty() && !isspace(prompt[prompt.length() - 1])) {
        prompt += ' ';
    }
}
