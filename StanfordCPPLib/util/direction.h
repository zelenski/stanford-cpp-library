/*
 * File: direction.h
 * -----------------
 * This file exports an enumerated type called <code>Direction</code>
 * whose elements are the four compass points: <code>NORTH</code>,
 * <code>EAST</code>, <code>SOUTH</code>, and <code>WEST</code>.
 *
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _direction_h
#define _direction_h

#include <iostream>
#include <string>

/**
 * This enumerated type is used to represent the four compass directions.
 */
enum Direction { NORTH, EAST, SOUTH, WEST };

/**
 * Returns the direction that is to the left of the argument.
 */
Direction leftFrom(Direction dir);

/**
 * Returns the direction that is to the right of the argument.
 */
Direction rightFrom(Direction dir);

/**
 * Returns the direction that is opposite to the argument.
 */
Direction opposite(Direction dir);

/**
 * Returns the name of the direction as a string.
 */
std::string directionToString(Direction dir);

/**
 * Overloads the <code>&lt;&lt;</code> operator so that it is able
 * to display <code>Direction</code> values.
 */
std::ostream& operator <<(std::ostream& os, const Direction& dir);

/**
 * Overloads the <code>&gt;&gt;</code> operator so that it is able
 * to read <code>Direction</code> values.
 */
std::istream& operator >>(std::istream& os, Direction& dir);

/**
 * Overloads the suffix version of the <code>++</code> operator to
 * work with <code>Direction</code> values.  The sole purpose of this
 * definition is to support the idiom
 *
 *<pre>
 *    for (Direction dir = NORTH; dir &lt;= WEST; dir++) ...
 *</pre>
 */
Direction operator ++(Direction& dir, int);

#endif // _direction_h
