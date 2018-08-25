/*
 * File: consolestreambuf.h
 * --------------------------
 * This file defines the <code>ConsoleStreambuf</code> class, which
 * represents a stream buffer that reads/writes to the Stanford graphical console
 * using a process pipe to a Java back-end process.
 *
 * @version 2016/10/04
 * - initial version
 */

#ifndef _consolestreambuf_h
#define _consolestreambuf_h

#include <iostream>
#include <streambuf>

namespace stanfordcpplib {

class ConsoleStreambuf : public std::streambuf {
protected:
    /* Constants */
    static const int BUFFER_SIZE = 4096;

    /* Instance variables */
    char inBuffer[BUFFER_SIZE];
    char outBuffer[BUFFER_SIZE];
    int blocked;

    // to be overridden in subclasses
    virtual void myEndLineConsole(bool isStderr) = 0;

    virtual std::string myGetLineConsole() = 0;

    virtual void myPutConsole(const std::string& str, bool isStderr) = 0;

public:
    ConsoleStreambuf() {
        setg(inBuffer, inBuffer, inBuffer);
        setp(outBuffer, outBuffer + BUFFER_SIZE);
        blocked = 0;
    }

    ~ConsoleStreambuf() {
        /* Empty */
    }

    virtual bool isBlocked() {
        return blocked > 0;
    }

    virtual int overflow(int ch = EOF) {
        return overflow(ch, /* isStderr */ false);
    }

    virtual int overflow(int ch, bool isStderr) {
        std::string line = "";
        for (char *cp = pbase(); cp < pptr(); cp++) {
            if (*cp == '\n') {
                myPutConsole(line, isStderr);
                myEndLineConsole(isStderr);
                line = "";
            } else {
                line += *cp;
            }
        }
        if (line != "") {
            myPutConsole(line, isStderr);
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

    virtual int sync(bool isStderr) {
        return overflow(EOF, isStderr);
    }

    virtual int underflow() {
        // Allow long strings at some point
        blocked++;
        std::string line = myGetLineConsole();
        blocked--;

        bool eof = std::cin.eof();
        fflush(stdout);

        if (eof) {
            return EOF;
        }

        int n = line.length();
        if (n + 1 >= BUFFER_SIZE) {
            error("ConsoleStreambuf::underflow: String too long");
        }
        for (int i = 0; i < n; i++) {
            inBuffer[i] = line[i];
        }
        inBuffer[n++] = '\n';
        inBuffer[n] = '\0';
        setg(inBuffer, inBuffer, inBuffer + n);
        return inBuffer[0];
    }
};

namespace qtgui {

extern void endLineConsoleQt(bool isStderr);
extern std::string getLineConsoleQt();
extern void putConsoleQt(const std::string& str, bool isStderr);

/*
 * The following class is an exact copy of the ConsoleStreambuf class above,
 * except using different Qt-related functions for output.
 */
class ConsoleStreambufQt : public ::stanfordcpplib::ConsoleStreambuf {
public:
    ConsoleStreambufQt(bool isStderr = false)
            : ConsoleStreambuf(),
              _isStderr(isStderr) {
        // empty
    }

    ~ConsoleStreambufQt() {
        /* Empty */
    }

protected:
    virtual void myEndLineConsole(bool /* isStderr */) {
        endLineConsoleQt(_isStderr);
    }

    virtual std::string myGetLineConsole() {
        return getLineConsoleQt();
    }

    virtual void myPutConsole(const std::string& str, bool /* isStderr */) {
        return putConsoleQt(str, _isStderr);
    }

private:
    bool _isStderr;
};

} // namespace qtgui

} // namespace stanfordcpplib

#endif // _consolestreambuf_h
