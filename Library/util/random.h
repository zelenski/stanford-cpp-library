/*
 * File: random.h
 * --------------
 * This file exports functions for generating pseudorandom numbers.
 *
 * @version 2019/05/16
 * - added randomColor that takes min/max RGB
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
 * Returns a random RGB color as an integer, with the value of the RGB components
 * bounded between the given minimum and maximum.
 * @throw ErrorException if min or max is not in [0..255] or min > max
 * See also: gwindow.h convertRGBToColor()
 */
int randomColor(int minRGB, int maxRGB);

/**
 * Returns a random RGB color as a hex string like "#ff00ff" for magenta.
 * See also: gwindow.h convertColorToRGB()
 */
std::string randomColorString();

/**
 * Returns a random RGB color as a hex string like "#ff00ff" for magenta,
 * with the value of the RGB components bounded between the given minimum and maximum.
 * @throw ErrorException if min or max is not in [0..255] or min > max
 * See also: gwindow.h convertColorToRGB()
 */
std::string randomColorString(int minRGB, int maxRGB);

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

#endif // _random_h
