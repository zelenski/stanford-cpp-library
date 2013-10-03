/*
 * File: direction.cpp
 * -------------------
 * This file implements the direction.h interface.
 */

#include "direction.h"
#include "error.h"
#include "strlib.h"
#include "tokenscanner.h"
using namespace std;

/*
 * Implementation notes: leftFrom, rightFrom, opposite
 * ---------------------------------------------------
 * These functions use the remainder operator to cycle through the
 * internal values of the enumeration type.  Note that the leftFrom
 * function cannot subtract 1 from the direction because the result
 * might then be negative; adding 3 achieves the same effect but
 * ensures that the values remain positive.
 */

Direction leftFrom(Direction dir) {
   return Direction((dir + 3) % 4);
}

Direction rightFrom(Direction dir) {
   return Direction((dir + 1) % 4);
}

Direction opposite(Direction dir) {
   return Direction((dir + 2) % 4);
}

/*
 * Implementation notes: directionToString
 * ---------------------------------------
 * The C++ compiler requires the default clause to ensure that this
 * function always returns a string, even if the direction is not one
 * of the legal values.
 */

string directionToString(Direction dir) {
   switch (dir) {
    case NORTH: return "NORTH";
    case EAST: return "EAST";
    case SOUTH: return "SOUTH";
    case WEST: return "WEST";
    default: return "???";
   }
}

/*
 * Implementation notes: <<
 * ------------------------
 * This operator must return the stream by reference after printing
 * the value.  The operator << returns this stream, so the function
 * can be implemented as a single line.
 */

std::ostream & operator<<(std::ostream & os, const Direction & dir) {
   return os << directionToString(dir);
}

/*
 * Implementation notes: >>
 * ------------------------
 * This implementation uses the TokenScanner to read tokens from the
 * stream.
 */

std::istream & operator>>(std::istream & is, Direction & dir) {
   TokenScanner scanner(is);
   scanner.ignoreWhitespace();
   string token = toUpperCase(scanner.nextToken());
   if (token == "") {
      dir = Direction(-1);
   } else if (startsWith("NORTH", token)) {
      dir = NORTH;
   } else if (startsWith("EAST", token)) {
      dir = EAST;
   } else if (startsWith("SOUTH", token)) {
      dir = SOUTH;
   } else if (startsWith("WEST", token)) {
      dir = WEST;
   } else {
      error("Direction: Unrecognized direction " + token);
   }
   return is;
}

/*
 * Implementation notes: ++
 * ------------------------
 * The int parameter in the signature for this operator is a marker used
 * by the C++ compiler to identify the suffix form of the operator.  Note
 * that the value after incrementing a variable containing WEST will be
 * out of the Direction range.  That fact will not cause a problem if
 * this operator is used only in the for loop idiom for which it is defined.
 */

Direction operator++(Direction & dir, int) {
   Direction old = dir;
   dir = Direction(dir + 1);
   return old;
}
