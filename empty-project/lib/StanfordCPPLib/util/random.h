/*
 * File: random.h
 * --------------
 * This file exports functions for generating pseudorandom numbers.
 * 
 * @version 2016/08/02
 * - added randomColor, randomColorString
 * @version 2014/10/19
 * - alphabetized functions
 */

#ifndef _random_h
#define _random_h

#include <string>

/*
 * Function: randomBool
 * Usage: if (randomBool()) ...
 * -------------------------------
 * Returns <code>true</code> with 50% probability.
 */
bool randomBool();

/*
 * Function: randomChance
 * Usage: if (randomChance(p)) ...
 * -------------------------------
 * Returns <code>true</code> with the probability indicated by <code>p</code>.
 * The argument <code>p</code> must be a floating-point number between
 * 0 (never) and 1 (always).  For example, calling
 * <code>randomChance(.30)</code> returns <code>true</code> 30 percent
 * of the time.
 */
bool randomChance(double p);

/*
 * Function: randomColor
 * Usage: int color = randomColor();
 * ---------------------------------
 * Returns a random RGB color as an integer.
 * See also: gwindow.h convertRGBToColor()
 */
int randomColor();

/*
 * Function: randomColorString
 * Usage: string color = randomColorString();
 * ------------------------------------------
 * Returns a random RGB color as a hex string like "#ff00ff" for magenta.
 * See also: gwindow.h convertColorToRGB()
 */
std::string randomColorString();

/*
 * Function: randomInteger
 * Usage: int n = randomInteger(low, high);
 * ----------------------------------------
 * Returns a random integer in the range <code>low</code> to
 * <code>high</code>, inclusive.
 */
int randomInteger(int low, int high);

/*
 * Function: randomReal
 * Usage: double d = randomReal(low, high);
 * ----------------------------------------
 * Returns a random real number in the half-open interval
 * [<code>low</code>&nbsp;..&nbsp;<code>high</code>).  A half-open
 * interval includes the first endpoint but not the second, which
 * means that the result is always greater than or equal to
 * <code>low</code> but strictly less than <code>high</code>.
 */
double randomReal(double low, double high);

/*
 * Function: setRandomSeed
 * Usage: setRandomSeed(seed);
 * ---------------------------
 * Sets the internal random number seed to the specified value.  You
 * can use this function to set a specific starting point for the
 * pseudorandom sequence or to ensure that program behavior is
 * repeatable during the debugging phase.
 */
void setRandomSeed(int seed);

// extra functions to facilitate creation of autograder programs
namespace autograder {
/*
 * Inserts the given boolean value to be returned by the random number
 * generator, rather than truly random choices.
 */
void randomFeedBool(bool value);

/*
 * Inserts the given integer to be returned by the random number generator,
 * rather than truly random numbers.
 */
void randomFeedInteger(int value);

/*
 * Inserts the given real number to be returned by the random number generator,
 * rather than truly random numbers.
 */
void randomFeedReal(double value);
}

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _random_h
