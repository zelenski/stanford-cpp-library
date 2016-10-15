/*
 * File: random.cpp
 * ----------------
 * This file implements the random.h interface.
 * 
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
#include <queue>
#include "private/static.h"

/* Private function prototype */

static void initRandomSeed();

/* internal buffer of fixed random numbers to return; used by autograders */
STATIC_VARIABLE_DECLARE_COLLECTION_EMPTY(std::queue<bool>, fixedBools)
STATIC_VARIABLE_DECLARE_COLLECTION_EMPTY(std::queue<int>, fixedInts)
STATIC_VARIABLE_DECLARE_COLLECTION_EMPTY(std::queue<double>, fixedReals)

void randomFeedBool(bool value) {
    STATIC_VARIABLE(fixedBools).push(value);
}

void randomFeedInteger(int value) {
    STATIC_VARIABLE(fixedInts).push(value);
}

void randomFeedReal(double value) {
    STATIC_VARIABLE(fixedReals).push(value);
}
/* end 'fixed' internal stuff */

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
    if (!STATIC_VARIABLE(fixedBools).empty()) {
        bool top = STATIC_VARIABLE(fixedBools).front();
        STATIC_VARIABLE(fixedBools).pop();
        return top;
    }
    initRandomSeed();
    return randomReal(0, 1) < p;
}

int randomColor() {
    if (!STATIC_VARIABLE(fixedInts).empty()) {
        int top = STATIC_VARIABLE(fixedInts).front();
        STATIC_VARIABLE(fixedInts).pop();
        return top & 0x00ffffff;
    }
    initRandomSeed();
    return rand() & 0x00ffffff;
}

// don't want to depend on gwindow.h
extern std::string convertRGBToColor(int rgb);

std::string randomColorString() {
    return convertRGBToColor(randomColor());
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
    if (!STATIC_VARIABLE(fixedInts).empty()) {
        int top = STATIC_VARIABLE(fixedInts).front();
        STATIC_VARIABLE(fixedInts).pop();
        return top;
    }
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
    if (!STATIC_VARIABLE(fixedReals).empty()) {
        double top = STATIC_VARIABLE(fixedReals).front();
        STATIC_VARIABLE(fixedReals).pop();
        return top;
    }
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
