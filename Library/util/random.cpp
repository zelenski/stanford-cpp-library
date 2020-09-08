/*
 * File: random.cpp
 * ----------------
 * This file implements the random.h interface.
 *
 * @version 2019/05/16
 * - added randomColor that takes min/max RGB
 * @version 2017/10/05
 * - added randomFeedClear
 * @version 2017/09/28
 * - moved random 'feed' functions into autograder namespace
 * - ensure that randomly fed integers are within the specified range
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2016/08/02
 * - added randomColor, randomColorString
 * @version 2014/10/19
 * - alphabetized functions
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include "random.h"
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <queue>
#include <sstream>
#include "error.h"
#include "private/static.h"

/* Private function prototype */

static void initRandomSeed();

bool randomBool() {
    return randomChance(0.5);
}

/*
 * Implementation notes: randomChance
 * ----------------------------------
 * The code for randomChance calls randomReal(0, 1) and then checks
 * whether the result is less than the requested probability.
 */
bool randomChance(double p) {
    initRandomSeed();
    return randomReal(0, 1) < p;
}

int randomColor() {
    initRandomSeed();
    return rand() & 0x00ffffff;
}

int randomColor(int minRGB, int maxRGB) {
    if (minRGB < 0 || minRGB > 255 || maxRGB < 0 || maxRGB > 255
            || minRGB > maxRGB) {
        error("randomColor: min/max values out of range");
    }
    int r = randomInteger(minRGB, maxRGB);
    int g = randomInteger(minRGB, maxRGB);
    int b = randomInteger(minRGB, maxRGB);
    return r << 16 | g << 8 | b;
}

// see convertRGBToColor in gcolor.h (repeated here to avoid Qt dependency)
std::string randomColorString() {
    int rgb = randomColor();
    std::ostringstream os;
    os << std::hex << std::uppercase << "#";
    os << std::setw(2) << std::setfill('0') << (rgb >> 16 & 0xFF);
    os << std::setw(2) << std::setfill('0') << (rgb >> 8 & 0xFF);
    os << std::setw(2) << std::setfill('0') << (rgb & 0xFF);
    return os.str();
}

std::string randomColorString(int minRGB, int maxRGB) {
    int rgb = randomColor(minRGB, maxRGB);
    std::ostringstream os;
    os << std::hex << std::uppercase << "#";
    os << std::setw(2) << std::setfill('0') << (rgb >> 16 & 0xFF);
    os << std::setw(2) << std::setfill('0') << (rgb >> 8 & 0xFF);
    os << std::setw(2) << std::setfill('0') << (rgb & 0xFF);
    return os.str();
}

/*
 * Implementation notes: randomInteger
 * -----------------------------------
 * The code for randomInteger produces the number in four steps:
 *
 * 1. Generate a random real number d in the range [0 .. 1).
 * 2. Scale the number to the range [0 .. N) where N is the number of values.
 * 3. Translate the number so that the range starts at the appropriate value.
 * 4. Convert the result to the next lower integer.
 *
 * The implementation is complicated by the fact that both the expression
 *
 *     RAND_MAX + 1
 *
 * and the expression for the number of values
 *
 *     high - low + 1
 *
 * can overflow the integer range.  These calculations must therefore be
 * performed using doubles instead of ints.
 */
int randomInteger(int low, int high) {
    initRandomSeed();
    double d = rand() / (double(RAND_MAX) + 1);
    double s = d * (double(high) - low + 1);
    return int(floor(low + s));
}

/*
 * Implementation notes: randomReal
 * --------------------------------
 * The code for randomReal is similar to that for randomInteger,
 * without the final conversion step.
 */
double randomReal(double low, double high) {
    initRandomSeed();
    double d = rand() / (double(RAND_MAX) + 1);
    double s = d * (high - low);
    return low + s;
}

/*
 * Implementation notes: setRandomSeed
 * -----------------------------------
 * The setRandomSeed function simply forwards its argument to srand.
 * The call to initRandomSeed is required to set the initialized flag.
 */
void setRandomSeed(int seed) {
    initRandomSeed();
    srand(seed);
}

/*
 * Implementation notes: initRandomSeed
 * ------------------------------------
 * The initRandomSeed function declares a static variable that keeps track
 * of whether the seed has been initialized.  The first time initRandomSeed
 * is called, initialized is false, so the seed is set to the current time.
 */
static void initRandomSeed() {
    static bool _initialized = false;
    if (!_initialized) {
        srand(int(time(nullptr)));
        rand();   // BUGFIX: throwaway call to get randomness going
        _initialized = true;
    }
}
