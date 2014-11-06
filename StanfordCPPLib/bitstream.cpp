/*
 * File: bitstream.cpp
 * -------------------
 * This file contains the implementation of ibitstream and obitstream classes.
 * These classes are patterned after (and, in fact, inherit from) the standard
 * ifstream and ofstream classes.  Please see bitstream.h for information about
 * how a client properly uses these classes.
 *
 * @author Keith Schwarz, Eric Roberts, Marty Stepp
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * 2014/01/23
 * - added slightly more descriptive error messages e.g. in writeBit
 * - whitespace reformatting
 * Previously last modified on Mon May 21 19:50:00 PST 2012 by Keith Schwarz
 */

#include "bitstream.h"
#include <iostream>
#include "error.h"
#include "strlib.h"

static const int NUM_BITS_IN_BYTE = 8;

inline int GetNthBit(int n, int fromByte) {
    return ((fromByte & (1 << n)) != 0);
}

inline void SetNthBit(int n, int & inByte) {
    inByte |= (1 << n);
}

/*
 * Returns a printable string for the given character.
 */
static std::string toPrintable(int ch) {
    if (ch == '\n') {
        return "'\\n'";
    } else if (ch == '\t') {
        return "'\\t'";
    } else if (ch == '\r') {
        return "'\\r'";
    } else if (ch == '\f') {
        return "'\\f'";
    } else if (ch == '\b') {
        return "'\\b'";
    } else if (ch == '\0') {
        return "'\\0'";
    } else if (ch == ' ') {
        return "' '";
    } else if (ch == (int) PSEUDO_EOF) {
        return "EOF";
    } else if (ch == (int) NOT_A_CHAR) {
        return "NONE";
    } else if (!isgraph(ch)) {
        return "???";
    } else {
        return std::string("'") + (char) ch + std::string("'");
    }
}

/* Constructor ibitstream::ibitstream
 * ----------------------------------
 * Each ibitstream tracks 3 integers as private data.
 * "lastTell" is streampos of the last byte that was read (this is used
 * to detect when other non-readBit activity has changed the tell)
 * "curByte" contains contents of byte currently being read
 * "pos" is the bit position within curByte that is next to read
 * We set initial state for lastTell and curByte to 0, then pos is
 * set at 8 so that next readBit will trigger a fresh read.
 */
ibitstream::ibitstream() : std::istream(NULL), lastTell(0), curByte(0), pos(NUM_BITS_IN_BYTE) {
    this->fake = false;
}

/* Member function ibitstream::readBit
 * -----------------------------------
 * If bits remain in curByte, retrieve next and increment pos
 * Else if end of curByte (or some other read happened), then read next byte
 * and start reading from bit position 0 of that byte.
 * If read byte from file at EOF, return EOF.
 */
int ibitstream::readBit() {
    if (!is_open()) {
        error("ibitstream::readBit: Cannot read a bit from a stream that is not open.");
    }

    if (this->fake) {
        int bit = get();
        if (bit == 0 || bit == '0') {
            return 0;
        } else {
            return 1;
        }
    } else {
        // if just finished bits from curByte or if data read from stream after last readBit()
        if (lastTell != tellg() || pos == NUM_BITS_IN_BYTE) {
            if ((curByte = get()) == EOF) {
                // read next single byte from file
                return EOF;
            }
            pos = 0; // start reading from first bit of new byte
            lastTell = tellg();
        }
        int result = GetNthBit(pos, curByte);
        pos++;   // advance bit position for next call to readBit
        return result;
    }
}

/* Member function ibitstream::rewind
 * ----------------------------------
 * Simply seeks back to beginning of file, so reading begins again
 * from start.
 */
void ibitstream::rewind() {
    if (!is_open()) {
        error("ibitstream::rewind: Cannot rewind stream that is not open.");
    }
    clear();
    seekg(0, std::ios::beg);
}

void ibitstream::setFake(bool fake) {
    this->fake = fake;
}

/* Member function ibitstream::size
 * --------------------------------
 * Seek to file end and use tell to retrieve position.
 * In order to not disrupt reading, we also record cur streampos and
 * re-seek to there before returning.
 */
long ibitstream::size() {
    if (!is_open()) {
        error("ibitstream::size: Cannot get size of stream which is not open.");
    }
    clear();					// clear any error state
    streampos cur = tellg();	// save current streampos
    seekg(0, std::ios::end);			// seek to end
    streampos end = tellg();	// get offset
    seekg(cur);					// seek back to original pos
    return long(end);
}

/* Member function ibitstream::is_open
 * -----------------------------------
 * Default implementation of is_open has the stream always
 * open.	Subclasses can customize this if they'd like.
 */
bool ibitstream::is_open() {
    return true;
}

/* Constructor obitstream::obitstream
 * ----------------------------------
 * Each obitstream tracks 3 integers as private data.
 * "lastTell" is streampos of the last byte that was written (this is used
 * to detect when other non-writeBit activity has changed the tell)
 * "curByte" contains contents of byte currently being written
 * "pos" is the bit position within curByte that is next to write
 * We set initial state for lastTell and curByte to 0, then pos is
 * set at 8 so that next writeBit will start a new byte.
 */
obitstream::obitstream() : std::ostream(NULL), lastTell(0), curByte(0), pos(NUM_BITS_IN_BYTE) {
    this->fake = false;
}

/* Member function obitstream::writeBit
 * ------------------------------------
 * If bits remain to be written in curByte, add bit into byte and increment pos
 * Else if end of curByte (or some other write happened), then start a fresh
 * byte at position 0.
 * We write the byte out for each bit (backing up to overwrite as needed), rather
 * than waiting for 8 bits.	 This is because the client might make
 * 3 writeBit calls and then start using << so we can't wait til full-byte
 * boundary to flush any partial-byte bits.
 */
void obitstream::writeBit(int bit) {
    if (bit != 0 && bit != 1) {
        error(std::string("obitstream::writeBit: must pass an integer argument of 0 or 1. You passed the integer ")
              + toPrintable(bit) + " (" + integerToString(bit) + ").");
    }
    if (!is_open()) {
        error("obitstream::writeBit: stream is not open");
    }

    if (this->fake) {
        put(bit == 1 ? '1' : '0');
    } else {
        // if just filled curByte or if data written to stream after last writeBit()
        if (lastTell != tellp() || pos == NUM_BITS_IN_BYTE) {
            curByte = 0;   // zero out byte for next writes
            pos = 0;	   // start writing to first bit of new byte
        }

        if (bit) {
            // only need to change if bit needs to be 1 (byte starts already zeroed)
            SetNthBit(pos, curByte);
        }

        if (pos == 0 || bit) {   // only write if first bit in byte or changing 0 to 1
            if (pos != 0) {
                seekp(-1, std::ios::cur);   // back up to overwite if pos > 0
            }
            put(curByte);
        }

        pos++; // advance to next bit position for next write
        lastTell = tellp();
    }
}

void obitstream::setFake(bool fake) {
    this->fake = fake;
}

/* Member function obitstream::size
 * --------------------------------
 * Seek to file end and use tell to retrieve position.
 * In order to not disrupt writing, we also record cur streampos and
 * re-seek to there before returning.
 */
long obitstream::size() {
    if (!is_open()) {
        error("obitstream::size: stream is not open");
    }
    clear();					// clear any error state
    streampos cur = tellp();	// save current streampos
    seekp(0, std::ios::end);			// seek to end
    streampos end = tellp();	// get offset
    seekp(cur);					// seek back to original pos
    return long(end);
}

/* Member function obitstream::is_open
 * -----------------------------------
 * Default implementation of is_open has the stream always
 * open.	Subclasses can customize this if they'd like.
 */
bool obitstream::is_open() {
    return true;
}

/* Constructor ifbitstream::ifbitstream
 * ------------------------------------
 * Wires up the stream class so that it knows to read data
 * from disk.
 */
ifbitstream::ifbitstream() {
    init(&fb);
}

/* Constructor ifbitstream::ifbitstream
 * ------------------------------------
 * Wires up the stream class so that it knows to read data
 * from disk, then opens the given file.
 */
ifbitstream::ifbitstream(const char* filename) {
    init(&fb);
    open(filename);
}
ifbitstream::ifbitstream(const std::string& filename) {
    init(&fb);
    open(filename);
}

/* Member function ifbitstream::open
 * ---------------------------------
 * Attempts to open the specified file, failing if unable
 * to do so.
 */
void ifbitstream::open(const char* filename) {
    if (!fb.open(filename, std::ios::in | std::ios::binary)) {
        setstate(std::ios::failbit);
    }
}

void ifbitstream::open(const std::string& filename) {
    open(filename.c_str());
}

/* Member function ifbitstream::is_open
 * ------------------------------------
 * Determines whether the file stream is open.
 */
bool ifbitstream::is_open() {
    return fb.is_open();
}

/* Member function ifbitstream::close
 * ----------------------------------
 * Closes the file stream, if one is open.
 */
void ifbitstream::close() {
    if (!fb.close()) {
        setstate(std::ios::failbit);
    }
}

/* Constructor ofbitstream::ofbitstream
 * ------------------------------------
 * Wires up the stream class so that it knows to write data
 * to disk.
 */
ofbitstream::ofbitstream() {
    init(&fb);
}

/* Constructor ofbitstream::ofbitstream
 * ------------------------------------
 * Wires up the stream class so that it knows to write data
 * to disk, then opens the given file.
 */
ofbitstream::ofbitstream(const char* filename) {
    init(&fb);
    open(filename);
}

ofbitstream::ofbitstream(const std::string& filename) {
    init(&fb);
    open(filename);
}

/* Member function ofbitstream::open
 * ---------------------------------
 * Attempts to open the specified file, failing if unable
 * to do so.
 */
void ofbitstream::open(const char* filename) {
    // Confirm we aren't about to do something that could potentially be a
    // Very Bad Idea.
    if (endsWith(filename, ".cpp") || endsWith(filename, ".h") ||
            endsWith(filename, ".hh") || endsWith(filename, ".cc")) {
        error(std::string("ofbitstream::open: It is potentially dangerous to write to file ")
              + filename + ", because that might be your own source code.  "
              + "We are explicitly disallowing this operation.  Please choose a "
              + "different filename.");
        setstate(std::ios::failbit);
    } else {
        if (!fb.open(filename, std::ios::out | std::ios::binary)) {
            setstate(std::ios::failbit);
        }
    }
}
void ofbitstream::open(const std::string& filename) {
    open(filename.c_str());
}

/* Member function ofbitstream::is_open
 * ------------------------------------
 * Determines whether the file stream is open.
 */
bool ofbitstream::is_open() {
    return fb.is_open();
}

/* Member function ofbitstream::close
 * ----------------------------------
 * Closes the given file.
 */
void ofbitstream::close() {
    if (!fb.close()) {
        setstate(std::ios::failbit);
    }
}

/* Constructor istringbitstream::istringbitstream
 * ----------------------------------------------
 * Sets the stream to use the string buffer, then sets
 * the initial string to the specified value.
 */
istringbitstream::istringbitstream(const std::string& s) {
    init(&sb);
    sb.str(s);
}

/* Member function istringbitstream::str
 * -------------------------------------
 * Sets the underlying string in the buffer to the
 * specified string.
 */
void istringbitstream::str(const std::string& s) {
    sb.str(s);
}

/* Member function ostringbitstream::ostringbitstream
 * --------------------------------------------------
 * Sets the stream to use the string buffer.
 */
ostringbitstream::ostringbitstream() {
    init(&sb);
}

/* Member function ostringbitstream::str
 * -------------------------------------
 * Retrives the underlying string data.
 */
std::string ostringbitstream::str() {
    return sb.str();
}
