/*
 * File: random.h
 * --------------
 * This file exports functions for generating pseudorandom numbers.
 * 
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2017/10/05
 * - added randomFeedClear
 * @version 2017/09/28
 * - moved random 'feed' functions into autograder namespace
 * @version 2016/08/02
 * - added randomColor, randomColorString
 * @version 2014/10/19
 * - alphabetized functions
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _random_h
#define _random_h

#include <string>

/**
 * Returns <code>true</code> with 50% probability.
 */
bool randomBool();

/**
 * Returns <code>true</code> with the probability indicated by <code>p</code>.
 * The argument <code>p</code> must be a floating-point number between
 * 0 (never) and 1 (always).  For example, calling
 * <code>randomChance(.30)</code> returns <code>true</code> 30 percent
 * of the time.
 */
bool randomChance(double p);

/**
 * Returns a random RGB color as an integer.
 * See also: gwindow.h convertRGBToColor()
 */
int randomColor();

/**
 * Returns a random RGB color as a hex string like "#ff00ff" for magenta.
 * See also: gwindow.h convertColorToRGB()
 */
std::string randomColorString();

/**
 * Returns a random integer in the range <code>low</code> to
 * <code>high</code>, inclusive.
 */
int randomInteger(int low, int high);

/**
 * Returns a random real number in the half-open interval
 * [<code>low</code>&nbsp;..&nbsp;<code>high</code>).  A half-open
 * interval includes the first endpoint but not the second, which
 * means that the result is always greater than or equal to
 * <code>low</code> but strictly less than <code>high</code>.
 */
double randomReal(double low, double high);

/**
 * Sets the internal random number seed to the specified value.  You
 * can use this function to set a specific starting point for the
 * pseudorandom sequence or to ensure that program behavior is
 * repeatable during the debugging phase.
 */
void setRandomSeed(int seed);

/**
 * Extra functions to facilitate creation of autograder programs.
 * @private
 */
namespace autograder {
/**
 * Inserts the given boolean value to be returned by the random number
 * generator, rather than truly random choices.
 */
void randomFeedBool(bool value);

/**
 * Removes any previously 'fed' random integers so that future calls
 * to random functions will return truly random values.
 */
void randomFeedClear();

/**
 * Inserts the given integer to be returned by the random number generator,
 * rather than truly random numbers.
 */
void randomFeedInteger(int value);

/**
 * Inserts the given real number to be returned by the random number generator,
 * rather than truly random numbers.
 */
void randomFeedReal(double value);
}

#endif // _random_h
