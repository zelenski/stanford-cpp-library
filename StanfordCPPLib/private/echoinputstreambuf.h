/*
 * File: echoinputstreambuf.h
 * --------------------------
 * This file defines the <code>EchoInputStreambuf</code> class, which
 * represents a stream buffer that echoes to stdout any user input.
 * We mostly use this to display console input when redirecting in from a file.
 *
 * @version 2016/10/04
 * - initial version
 */

#ifndef _echoinputstreambuf_h
#define _echoinputstreambuf_h

#include <iostream>
#include <streambuf>

namespace stanfordcpplib {

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
            setg(nullptr, nullptr, nullptr);
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

} // namespace stanfordcpplib

#endif // _echoinputstreambuf_h
