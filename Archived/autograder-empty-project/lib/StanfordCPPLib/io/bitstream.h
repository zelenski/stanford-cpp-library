/*
 * File: bitstream.h
 * -----------------
 * This file defines the ibitstream and obitstream classes which are basically
 * same as the ordinary istream and ostream classes, but add the
 * functionality to read and write one bit at a time.
 *
 * The idea is that you can substitute an ibitstream in place of an
 * istream and use the same operations (get, fail, >>, etc.)
 * along with added member functions of readBit, rewind, and size.
 *
 * Similarly, the obitstream can be used in place of ofstream, and has
 * same operations (put, fail, <<, etc.) along with additional
 * member functions writeBit and size.
 *
 * There are two subclasses of ibitstream: ifbitstream and istringbitstream,
 * which are similar to the ifstream and istringstream classes.  The
 * obitstream class similarly has ofbitstream and ostringbitstream as
 * subclasses.
 *
 * @author Keith Schwarz, Eric Roberts, Marty Stepp
 * @version 2019/04/20
 * - added toPrintable(string)
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2016/11/12
 * - made toPrintable non-static and visible
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _bitstream_h
#define _bitstream_h

#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>

/**
 * Constant: PSEUDO_EOF
 * A constant representing the PSEUDO_EOF marker that you will
 * write at the end of your Huffman-encoded file.
 */
const int PSEUDO_EOF = 256;

/**
 * Constant: NOT_A_CHAR
 * A constant representing an extended character that does not
 * actually hold a value.  When you are constructing your Huffman
 * encoding tree, you should set the characters in each internal
 * node (non-leaf) to this value to explicitly mark that they are not
 * being used.
 */
const int NOT_A_CHAR = 257;

/**
 * Defines a class for reading files with all the functionality of istream
 * along with an added member function for reading a single bit and convenience
 * functions for rewinding the stream back to the beginning and getting the stream
 * size.
 *
 * You will probably not create instances of this class directly.  Instead, you
 * will create ifbitstreams or istringbitstreams to read from files or string buffers.
 */
class ibitstream: public std::istream {
public:
    /**
     * Initializes a new ibitstream that is not attached to any source.  You are
     * unlikely to use this function directly.
     */
    ibitstream();

    /**
     * Reads a single bit from the ibitstream and returns 0 or 1 depending on
     * the bit value.  If the stream is exhausted, EOF (-1) is returned.
     * Raises an error if this ibitstream has not been properly opened.
     */
    int readBit();

    /**
     * Rewinds the ibitstream back to the beginning so that subsequent reads
     * start again from the beginning.  Raises an error if this ibitstream
     * has not been properly opened.
     */
    void rewind();
    
    /**
     * Sets 'fake' mode, where it actually reads bytes when you say readBit.
     */
    void setFake(bool fake);

    /**
     * Returns the size in bytes of the data attached to this stream.
     * Raises an error if this ibitstream has not been properly opened.
     */
    long size();

    /**
     * Returns whether or not this ibitstream is opened.  This only has
     * meaning if the ibitstream is a file stream; otherwise it always
     * returns true.
     */
    virtual bool is_open();

private:
    std::streampos lastTell;
    int curByte;
    int pos;
    bool fake;
};


/**
 * Defines a class for writing files with all the functionality of ostream
 * along with an added member function for writing a single bit and a convenience
 * function for getting the stream size.
 *
 * You are unlikely to instantiate this class directly; instead, instantiate one
 * of the subclasses.
 */
class obitstream: public std::ostream {
public:
    /**
     * Initializes a new obitstream that is not attached to any file.  Use the
     * open member function from ofstream to attach the stream to a file.
     */
    obitstream();

    /**
     * Writes a single bit to the obitstream.
     * Raises an error if this obitstream has not been properly opened.
     */
    void writeBit(int bit);

    /**
     * Returns the size in bytes of the file attached to this stream.
     * Raises an error if this obitstream has not been properly opened.
     */
    long size();

    /**
     * Sets 'fake' mode, where it actually writes bytes when you say writeBit.
     */
    void setFake(bool fake);
    
    /**
     * Returns whether or not this obitstream is opened.  This only has
     * meaning if the obitstream is a file stream; otherwise it always
     * returns true.
     */
    virtual bool is_open();

private:
    std::streampos lastTell;
    int curByte;
    int pos;
    bool fake;
};

/**
 * A class for reading files in all of the usual ways, plus bit-by-bit.
 * You can treat this class like a normal ifstream, except that there is
 * extra support for bit-level operations.
 */
class ifbitstream: public ibitstream {
public:
    /**
     * Constructs a new ifbitstream not attached to any file.  You can
     * open a file for reading using the .open() member functions.
     */
    ifbitstream();

    /**
     * Constructs a new ifbitstream that reads the file with the given name,
     * if it exists.  If not, the stream enters an error state.
     */
    ifbitstream(const char* filename);

    /**
     * Constructs a new ifbitstream that reads the specified file, if
     * it exists.  If not, the stream enters an error state.
     */
    ifbitstream(const std::string& filename);

    /**
     * Opens the specified file for reading.  If an error occurs, the
     * stream enters a failure state, which can be detected by calling
     * ifb.fail().
     */
    void open(const char* filename);

    /**
     * Opens the specified file for reading.  If an error occurs, the
     * stream enters a failure state, which can be detected by calling
     * ifb.fail().
     */
    void open(const std::string& filename);

    /**
     * Returns whether or not this ifbitstream is connected to a file for
     * reading.
     */
    bool is_open();

    /**
     * Closes the currently-opened file, if the stream is open.  If the
     * stream is not open, puts the stream into a fail state.
     */
    void close();

private:
    // the actual file buffer which does reading and writing.
    std::filebuf fb;
};

/**
 * A class for writing files in all of the usual ways, plus bit-by-bit.
 * You can treat this class like a normal ofstream, except that there is
 * extra support for bit-level operations.
 *
 * As a safety feature, you cannot use ofbitstream to open files that end
 * in .h, .hh, .cpp, or .cc for writing, as this could very easily cause
 * you to destroy your source files.
 */
class ofbitstream: public obitstream {
public:
    /**
     * Constructs a new ofbitstream not attached to any file.  You can
     * open a file for writing using the .open() member functions.
     */
    ofbitstream();

    /**
     * Constructs a new ofbitstream that writes the specified file, if
     * it exists.  If not, the stream enters an error state.  Read
     * the documentation on "open" for more details.
     */
    ofbitstream(const char* filename);

    /**
     * Constructs a new ofbitstream that writes the specified file, if
     * it exists.  If not, the stream enters an error state.  Read
     * the documentation on "open" for more details.
     */
    ofbitstream(const std::string& filename);

    /**
     * Opens the specified file for writing.  If an error occurs, the
     * stream enters a failure state, which can be detected by calling
     * ifb.fail().  If an invalid filename is specified (for example,
     * a source file), reports an error.
     */
    void open(const char* filename);

    /**
     * Opens the specified file for writing.  If an error occurs, the
     * stream enters a failure state, which can be detected by calling
     * ifb.fail().  If an invalid filename is specified (for example,
     * a source file), reports an error.
     */
    void open(const std::string& filename);

    /**
     * Returns whether or not this ofbitstream is connected to a file for
     * reading.
     */
    bool is_open();

    /**
     * Closes the currently-opened file, if the stream is open.  If the
     * stream is not open, puts the stream into a fail state.
     */
    void close();

private:
    // the actual file buffer which does reading and writing.
    std::filebuf fb;
};

/**
 * A variant on C++'s istringstream class, which acts as a stream that
 * reads its data from a string.  This is mostly used by the testing
 * code to test your Huffman encoding without having to read or write
 * files on disk, but you can use it in your own testing if you would
 * like.
 */
class istringbitstream: public ibitstream {
public:
    /**
     * Constructs an istringbitstream reading the specified string.
     */
    istringbitstream(const std::string& s = "");

    /**
     * Sets the underlying string of the istringbitstream.
     */
    void str(const std::string& s);
private:
    // the actual string buffer that does character storage
    std::stringbuf sb;
};

/**
 * A variant on C++'s ostringstream class, which acts as a stream that
 * writes its data to a string.  This is mostly used by the testing
 * code to test your Huffman encoding without having to read or write
 * files on disk, but you can use it in your own testing if you would
 * like.
 */
class ostringbitstream: public obitstream {
public:
    /**
     * Constructs an ostringbitstream.
     */
    ostringbitstream();

    /**
     * Retrieves the underlying string of the istringbitstream.
     */
    std::string str();

private:
    // the actual string buffer that does character storage
    std::stringbuf sb;
};

/**
 * Returns a printable string for the given character.
 * @example toPrintable('c') returns "c"
 * @example toPrintable('\n') returns "\\n"
 */
std::string toPrintable(int ch);

/**
 * Returns a string with each non-printable character in the given string
 * replaced by one that is printable.
 * Certain common escape characters are replaced by a backslash representation,
 * and non-printable ASCII characters are replaced by a backslash and their
 * ASCII numeric representation, such as \255.
 * @example toPrintable("hi \0 there\n') returns "hi \\0 there\\n"
 */
std::string toPrintable(const std::string& s);

#endif // _bitstream_h
