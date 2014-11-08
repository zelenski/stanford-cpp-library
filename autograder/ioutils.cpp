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

namespace ioutils {
static std::stringstream bufferOut;
static std::streambuf* oldOut;
static std::stringstream bufferErr;
static std::streambuf* oldErr;
static std::stringstream bufferIn;
static std::streambuf* oldIn;
static bool consoleEchoUserInput = false;
static int consoleOutputLimit = 0;

/*
 * An input stream buffer that echoes characters as they are read.
 * Used to echo cin console input for capturing for test case diffing
 * when fixed input has been fed to cin by autograders.
 * 
 * inspired by: http://gabisoft.free.fr/articles/fltrsbf1.html
 */
class EchoInputStreambuf : public std::streambuf {
public:
    EchoInputStreambuf(std::streambuf* source) : m_source(source), m_buffer(0) {
        // empty
    }
    
    virtual ~EchoInputStreambuf() {
        sync();
    }
    
    virtual int overflow(int) {
        return EOF;
    }

    /*
     * This is the crucial function; called to read a character from the
     * underlying stream buffer (cin).  We capture it in a one-char m_buffer
     * so we can return it later.
     */
    virtual int underflow() {
        int result(EOF);
        if (gptr() < egptr()) {
            result = *gptr();
        } else {
            result = m_source->sbumpc();
            if (result != EOF) {
                m_buffer = result;
                setg(&m_buffer, &m_buffer, &m_buffer + 1);
                
                // echo the character to stdout
                std::cout.put(result);
                std::cout.flush();
            }
        }
        return result;
    }

    virtual int sync() {
        int result(0);
        if (gptr() < egptr()) {
            result = m_source->sputbackc(*gptr());
            setg(NULL, NULL, NULL);
        }
        if (m_source->pubsync() == EOF) {
            result = EOF;
        }
        return result;
    }

    virtual std::streambuf* setbuf(char* p, std::streamsize len) {
        return m_source->pubsetbuf(p, len);
    }

private:
    std::streambuf* m_source;
    char m_buffer;
};


/*
 * An output stream buffer that counts the number of characters written to it.
 * Used to limit cout console output by autograders for naughty students who
 * print too much output.
 * 
 * inspired by: http://gabisoft.free.fr/articles/fltrsbf1.html
 */
class LimitOutputStreambuf : public std::streambuf {
public:
    LimitOutputStreambuf(std::streambuf* source, int max) : m_source(source), m_count(0), m_max(max) {
        // empty
    }
    
    virtual ~LimitOutputStreambuf() {
        sync();
    }
    
    /*
     * This is the crucial function; called to write a character to the
     * underlying stream buffer (cout).  We count them so we can throw
     * an error if too many are printed.
     */
    virtual int overflow(int ch) {
        if (m_count >= 0) {
            m_count++;
            if (m_count > m_max) {
                m_count = -1;   // disable checking on further calls so I can print again
                std::ostringstream os;
                os << std::endl;
                os << "*** ERROR: Excessive output produced! (over " << m_max << " chars)" << std::endl;
                os << "***        Halting program." << std::endl;
                std::string str = os.str();
                m_source->sputn(str.c_str(), (int) str.length());
                throw std::exception();
            }
        }
        
        return m_source->sputc(ch);
    }

    virtual int underflow() {
        return EOF;
    }

    virtual int sync() {
        return m_source->pubsync();
    }

    virtual std::streambuf* setbuf(char* p, std::streamsize len) {
        return m_source->pubsetbuf(p, len);
    }

private:
    std::streambuf* m_source;
    int m_count;
    int m_max;
};


void captureStderrBegin() {
    bufferErr.str(std::string());
    std::streambuf* newBuf;
    int limit = getConsoleOutputLimit();
    if (limit > 0) {
        newBuf = new LimitOutputStreambuf(bufferErr.rdbuf(), limit);
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
        newBuf = new LimitOutputStreambuf(bufferOut.rdbuf(), limit);
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
        newBuf = new EchoInputStreambuf(bufferIn.rdbuf());
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
