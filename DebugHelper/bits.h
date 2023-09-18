#pragma once
#include <ostream>
#include "queue.h"

/**
 * Type representing a single bit value (0 or 1).  A Bit is largely
 * interchangeable with an int, except that a Bit raises an error
 * if value is anything other than 0 or 1.
 *
 *     Bit zero = 0;
 *
 *     queue.enqueue(0);
 *     queue.enqueue(Bit(1));
 *
 *     if (bit == 0) { ... }
 */
class Bit {
public:
    Bit() = default;
    Bit(int value);

    friend bool operator== (Bit lhs, Bit rhs);
    friend bool operator!= (Bit lhs, Bit rhs);
    friend std::ostream& operator<< (std::ostream& out, Bit bit);

private:
    bool _value;
};



/*
 * Type representing a binary-encoded message. The messageBits contains
 * the encoded message text and the treeShape and treeLeaves are the
 * flattened representation of the encoding tree.
 */
struct EncodedData {
    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    Queue<Bit>  messageBits;
};


/* For debugging purposes, you can print EncodedData objects to cout to see
 * what they contain.
 */
std::ostream& operator<< (std::ostream& out, const EncodedData& data);



/**
 * Routines for reading and writing EncodedData objects to a stream. The code in
 * here is what actually touches files on disk.
 */
void writeData(EncodedData& file, std::ostream& out);
EncodedData readData(std::istream& in);

