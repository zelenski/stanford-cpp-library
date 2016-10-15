/*
 * File: limitoutputstreambuf.h
 * ----------------------------
 * This file defines the <code>LimitOutputStreambuf</code> class, which
 * represents a stream buffer that counts how much output has been produced
 * and throws an exception if a given maximum is exceeded.
 * We mostly use this in grading programs to halt runaway student code.
 *
 * @version 2016/10/04
 * - initial version
 */

#ifndef _limitoutputstreambuf_h
#define _limitoutputstreambuf_h

#include <exception>
#include <iostream>
#include <streambuf>

namespace stanfordcpplib {

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

} // namespace stanfordcpplib

#endif // _limitoutputstreambuf_h
