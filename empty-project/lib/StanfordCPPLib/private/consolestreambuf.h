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

extern void endLineConsole(bool isStderr);
extern std::string getLineConsole();
extern void putConsole(const std::string& str, bool isStderr);

class ConsoleStreambuf : public std::streambuf {
private:
    /* Constants */
    static const int BUFFER_SIZE = 4096;

    /* Instance variables */
    char inBuffer[BUFFER_SIZE];
    char outBuffer[BUFFER_SIZE];
    int blocked;

public:
    ConsoleStreambuf() {
        setg(inBuffer, inBuffer, inBuffer);
        setp(outBuffer, outBuffer + BUFFER_SIZE);
        blocked = 0;
    }

    ~ConsoleStreambuf() {
        /* Empty */
    }

    bool isBlocked() {
        return blocked > 0;
    }

    virtual int overflow(int ch = EOF) {
        return overflow(ch, /* isStderr */ false);
    }

    virtual int overflow(int ch, bool isStderr) {
        std::string line = "";
        for (char *cp = pbase(); cp < pptr(); cp++) {
            if (*cp == '\n') {
                putConsole(line, isStderr);
                endLineConsole(isStderr);
                line = "";
            } else {
                line += *cp;
            }
        }
        if (line != "") {
            putConsole(line, isStderr);
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
        std::string line = getLineConsole();
        blocked--;
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
} // namespace stanfordcpplib

#endif // _consolestreambuf_h
