/*
 * File: ioutils.cpp
 * ---------------
 * This file contains implementations of functions to help capture, redirect,
 * and feed input to cin/cout/err.
 * See ioutils.h for documentation of each function.
 * 
 * @author Marty Stepp
 * @version 2014/10/14
 * @since 2014/03/01
 */

#include "ioutils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "console.h"
#include "error.h"
#include "private/echoinputstreambuf.h"
#include "private/limitoutputstreambuf.h"

namespace ioutils {
static std::stringstream bufferOut;
static std::streambuf* oldOut;
static std::stringstream bufferErr;
static std::streambuf* oldErr;
static std::stringstream bufferIn;
static std::streambuf* oldIn;
static bool consoleEchoUserInput = false;
static int consoleOutputLimit = 0;



void captureStderrBegin() {
    bufferErr.str(std::string());
    std::streambuf* newBuf;
    int limit = getConsoleOutputLimit();
    if (limit > 0) {
        newBuf = new stanfordcpplib::LimitOutputStreambuf(bufferErr.rdbuf(), limit);
    } else {
        newBuf = bufferErr.rdbuf();
    }
    oldErr = std::cerr.rdbuf(newBuf);
}

std::string captureStderrEnd() {
    if (oldErr != NULL) {
        std::cerr.rdbuf(oldErr);
        oldErr = NULL;
    }
    return bufferErr.str();
}

void captureStdoutBegin(bool alsoStderr) {
    bufferOut.str(std::string());
    std::streambuf* newBuf;
    int limit = getConsoleOutputLimit();
    if (limit > 0) {
        newBuf = new stanfordcpplib::LimitOutputStreambuf(bufferOut.rdbuf(), limit);
    } else {
        newBuf = bufferOut.rdbuf();
    }
    
    oldOut = std::cout.rdbuf(newBuf);
    if (alsoStderr) {
        bufferErr.str(std::string());
        oldErr = std::cerr.rdbuf(newBuf);
    }
}

std::string captureStdoutEnd() {
    if (oldOut != NULL) {
        std::cout.rdbuf(oldOut);
        oldOut = NULL;
    }
    if (oldErr != NULL) {
        std::cerr.rdbuf(oldErr);
        oldErr = NULL;
    }
    return bufferOut.str();
}

bool getConsoleEchoUserInput() {
    return consoleEchoUserInput;
}

int getConsoleOutputLimit() {
    return consoleOutputLimit;
}
        
void redirectStdinBegin(std::string userInput) {
    bufferIn.str(std::string());
    std::streambuf* newBuf;
    if (getConsoleEchoUserInput()) {
        newBuf = new stanfordcpplib::EchoInputStreambuf(bufferIn.rdbuf());
    } else {
        newBuf = bufferIn.rdbuf();
    }
    oldIn = std::cin.rdbuf(newBuf);
    redirectStdinFeedInput(userInput);
}

void redirectStdinFeedInput(std::string userInput) {
    if (!userInput.empty()) {
        bufferIn << userInput << std::endl;
    }
}

void redirectStdinEnd() {
    if (oldIn != NULL) {
        std::cin.rdbuf(oldIn);
        oldIn = NULL;
    }
}

void setConsoleEchoUserInput(bool echo) {
    consoleEchoUserInput = echo;
}

void setConsoleOutputLimit(int limit) {
    consoleOutputLimit = limit;
}
} // namespace ioutils
