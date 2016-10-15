/*
 * File: forwardingstreambuf.h
 * ---------------------------
 * This file defines the <code>ForwardingStreambuf</code> class, which
 * represents a stream buffer that just wraps another stream buffer.
 * We mostly use this to merge cout and cerr into a single output target.
 *
 * @version 2016/10/04
 * - initial version
 */

#ifndef _forwardingstreambuf_h
#define _forwardingstreambuf_h

#include <iostream>
#include <locale>
#include <streambuf>

namespace stanfordcpplib {
/*
 * A stream buffer that just "wraps" another stream buffer.
 * This is used here to distinguish cout (black text) from cerr (red text).
 */
class ForwardingStreambuf : public std::streambuf {
private:
    ConsoleStreambuf& delegate;
    bool isStderr;

public:
    ForwardingStreambuf(ConsoleStreambuf& del, bool err = false)
            : delegate(del), isStderr(err) {
        // empty
    }

    bool isBlocked() {
        return delegate.isBlocked();
    }

    virtual int overflow(int ch = EOF) {
        return delegate.overflow(ch, isStderr);
    }

    virtual int overflow(int ch, bool isStderr) {
        return delegate.overflow(ch, isStderr);
    }

    virtual int sync() {
        return delegate.sync(isStderr);
    }

    virtual int sync(bool isStderr) {
        return delegate.sync(isStderr);
    }

    virtual int underflow() {
        return delegate.underflow();
    }

    // functions below are overridden for completeness,
    // but all just delegate to underlying ConsoleStreambuf
    std::streamsize in_avail() {
        return delegate.in_avail();
    }

    int snextc() {
        return delegate.snextc();
    }

    int sbumpc() {
        return delegate.sbumpc();
    }

    int sgetc() {
        return delegate.sgetc();
    }

    std::streamsize sgetn(char* s, std::streamsize n) {
        return delegate.sgetn(s, n);
    }

    int sputbackc(char c) {
        return delegate.sputbackc(c);
    }

    int sungetc() {
        return delegate.sungetc();
    }

    int sputc(char c) {
        return delegate.sputc(c);
    }

    std::streamsize sputn(const char* s, std::streamsize n) {
        return delegate.sputn(s, n);
    }

    std::locale pubimbue(const std::locale& loc) {
        return delegate.pubimbue(loc);
    }

    std::locale getloc() const {
        return delegate.getloc();
    }

    std::streambuf* pubsetbuf(char* s, std::streamsize n) {
        return delegate.pubsetbuf(s, n);
    }

    std::streampos pubseekoff(std::streamoff off, std::ios_base::seekdir way,
                          std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) {
        return delegate.pubseekoff(off, way, which);
    }

    std::streampos pubseekpos(std::streampos pos, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) {
        return delegate.pubseekpos(pos, which);
    }

    int pubsync() {
        return delegate.pubsync();
    }
};
} // namespace stanfordcpplib

#endif // _forwardingstreambuf_h
