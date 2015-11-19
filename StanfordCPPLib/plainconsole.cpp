/*
 * File: plainconsole.cpp
 * ----------------------
 * This file defines the implementation of functions to add utility to the
 * C++ plain text console streams, cin/cout/cerr.
 * See plainconsole.h for documentation of each function.
 *
 * @author Marty Stepp
 * @version 2015/10/21
 * @since 2015/10/21
 */

#include "plainconsole.h"
#include <csignal>
#include <iostream>
#include <string>
#include "error.h"

namespace plainconsole {
/*
 * A stream buffer that just forwards everything to a delegate,
 * but echoes any user input read from it.
 * Used to (sometimes) echo console input when redirected in from a file.
 * http://www.cplusplus.com/reference/streambuf/streambuf/
 */
class EchoingStreambuf : public std::streambuf {
private:
    /* Constants */
    static const int BUFFER_SIZE = 4096;

    /* Instance variables */
    char inBuffer[BUFFER_SIZE];
    char outBuffer[BUFFER_SIZE];
    std::istream instream;
    std::ostream& outstream;
    int outputLimit;
    int outputPrinted;

public:
    EchoingStreambuf(std::streambuf& buf, std::ostream& out)
            : instream(&buf),
              outstream(out),
              outputLimit(0),
              outputPrinted(0) {
        // outstream.rdbuf(&buf);
        setg(inBuffer, inBuffer, inBuffer);
        setp(outBuffer, outBuffer + BUFFER_SIZE);
    }

    ~EchoingStreambuf() {
        /* Empty */
    }
    
    virtual void setOutputLimit(int limit) {
        outputLimit = limit;
    }

    virtual int underflow() {
        // 'return 0' handles end-of-input from stdin redirect
        std::string line;
        if (!getline(instream, line)) {
            return 0;
        }
        
        int n = line.length();
        if (n + 1 >= BUFFER_SIZE) {
            error("EchoingStreambuf::underflow: String too long");
        }
        for (int i = 0; i < n; i++) {
            inBuffer[i] = line[i];
        }
        inBuffer[n++] = '\n';
        inBuffer[n] = '\0';
        setg(inBuffer, inBuffer, inBuffer + n);
        
        // this is the place to echo the input
        // fprintf(stdout, "inBuffer: \"%s\"\n", inBuffer);
        // fflush(stdout);
        outstream << inBuffer;
        outstream.flush();
        
        return inBuffer[0];
    }

    virtual int overflow(int ch = EOF) {
        std::string line = "";
        for (char *cp = pbase(); cp < pptr(); cp++) {
            if (*cp == '\n') {
                // puts(line.c_str());
                outputPrinted += line.length();
                if (outputLimit > 0 && outputPrinted > outputLimit) {
                    error("excessive output printed");
                }
                line = "";
            } else {
                line += *cp;
            }
        }
        if (line != "") {
            // puts(line.c_str());
            outputPrinted += line.length();
            if (outputLimit > 0 && outputPrinted > outputLimit) {
                error("excessive output printed");
            }
        }
        setp(outBuffer, outBuffer + BUFFER_SIZE);
        if (ch != EOF) {
            outBuffer[0] = ch;
            pbump(1);
        }
        return ch != EOF;
    }
    
    virtual int sync() {
        return overflow();
    }
};

/*
 * A stream buffer that limits how many characters you can print to it.
 * If you exceed that many, it throws an ErrorException.
 */
class LimitedStreambuf : public std::streambuf {
private:
    std::ostream outstream;
    int outputLimit;
    int outputPrinted;

public:
    LimitedStreambuf(std::streambuf& buf, int limit)
            : outstream(&buf),
              outputLimit(limit),
              outputPrinted(0) {
        setp(0, 0);   // // no buffering, overflow on every char
    }

    virtual void setOutputLimit(int limit) {
        outputLimit = limit;
    }

    virtual int overflow(int ch = EOF) {
        outputPrinted++;
        if (outputLimit > 0 && outputPrinted > outputLimit) {
            // error("excessive output printed");
            // outstream.setstate(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
            // kill the program
            // (use a signal rather than error/exception
            // so student won't try to catch it)
            raise(SIGABRT);
        } else {
            outstream.put(ch);
        }
        return ch;
    }
};

void setOutputLimit(int limit) {
    if (limit <= 0) {
        error("Platform::setConsoleOutputLimit: limit must be a positive integer");
    }
    LimitedStreambuf* limitedbufOut = new LimitedStreambuf(*std::cout.rdbuf(), limit);
    LimitedStreambuf* limitedbufErr = new LimitedStreambuf(*std::cerr.rdbuf(), limit);
    std::cout.rdbuf(limitedbufOut);
    std::cerr.rdbuf(limitedbufErr);
}

void setEcho(bool value) {
    static EchoingStreambuf* echobufIn = NULL;
    static std::streambuf* oldBuf = NULL;
    
    if (!echobufIn && value) {
        // start to echo user input pulled from cin
        oldBuf = std::cin.rdbuf();
        echobufIn = new EchoingStreambuf(*std::cin.rdbuf(), std::cout);
        std::cin.rdbuf(echobufIn);
    } else if (echobufIn && !value) {
        // stop echo
        std::cin.rdbuf(oldBuf);
        oldBuf = NULL;
        echobufIn = NULL;
    }
}

} // namespace plainconsole
