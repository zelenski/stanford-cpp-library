/*
 * File: random.c
 * Last modified on Wed Dec 19 14:49:19 2012 by eroberts
 * -----------------------------------------------------
 * This file implements the random.h interface.
 */

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cslib.h"
#include "random.h"
#include "unittest.h"
#include "private/randompatch.h"

/* Private function prototype */

static void initRandomSeed(void);

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
   double d, s;

   initRandomSeed();
   d = rand() / ((double) RAND_MAX + 1);
   s = d * ((double) high - low + 1);
   return (int) (floor(low + s));
}

/*
 * Implementation notes: randomReal
 * --------------------------------
 * The code for randomReal is similar to that for randomInteger,
 * without the final conversion step.
 */

double randomReal(double low, double high) {
   double d, s;

   initRandomSeed();
   d = rand() / ((double) RAND_MAX + 1);
   s = d * (high - low);
   return low + s;
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

static void initRandomSeed(void) {
   static bool initialized = false;
   if (!initialized) {
      srand((int) time(NULL));
      initialized = true;
   }
}

/**********************************************************************/
/* Unit test for the random module                                    */
/**********************************************************************/

#ifndef _NOTEST_

/* Unit test */

#define N_RANGES 20
#define N_TRIALS 10000

/* Private function prototypes */

static bool randomIntegerSeemsReasonable(int low, int high);
static bool randomRealSeemsReasonable(double low, double high);
static bool randomChanceSeemsReasonable(double p);
static bool hugeRangeSeemsReasonable();

void testRandomModule(void) {
   test(randomIntegerSeemsReasonable(0, 9), true);
   test(randomIntegerSeemsReasonable(-10, 10), true);
   test(randomRealSeemsReasonable(0.0, 10.0), true);
   test(randomRealSeemsReasonable(-100.0, 100.0), true);
   test(randomChanceSeemsReasonable(0.5), true);
   test(randomChanceSeemsReasonable(0), true);
   test(randomChanceSeemsReasonable(1), true);
   test(randomChanceSeemsReasonable(0.9), true);
   test(hugeRangeSeemsReasonable(), true);
}

static bool randomIntegerSeemsReasonable(int low, int high) {
   int i, k, rangeSize, *counts, outcome;
   bool ok;
   double expected;

   rangeSize = high - low + 1;
   counts = newArray(rangeSize, int);
   for (i = 0; i < rangeSize; i++) {
      counts[i] = 0;
   }
   ok = true;
   for (i = 0; ok && i < N_TRIALS; i++) {
      k = randomInteger(low, high);
      if (k < low || k > high) {
         reportError("randomInteger returned out of range value %d", k);
         ok = false;
      } else {
         counts[k - low]++;
      }
   }
   expected = (double) N_TRIALS / rangeSize;
   for (i = 0; ok && i < rangeSize; i++) {
      outcome = low + i;
      if (counts[i] < 0.5 * expected) {
         reportError("Low count for outcome %d", outcome);
         ok = false;
      } else if (counts[i] > 1.5 * expected) {
         reportError("High count for outcome %d", outcome);
         ok = false;
      }
   }
   freeBlock(counts);
   return ok;
}

static bool randomRealSeemsReasonable(double low, double high) {
   int i, counts[N_RANGES];
   double d, range, expected;
   bool ok;

   range = high - low;
   for (i = 0; i < N_RANGES; i++) {
      counts[i] = 0;
   }
   ok = true;
   for (i = 0; ok && i < N_TRIALS; i++) {
      d = randomReal(low, high);
      if (d < low || d >= high) {
         reportError("RandomReal returned out of range value %g", d);
         ok = false;
      } else {
         counts[(int)(N_RANGES * (d - low) / range)]++;
      }
   }
   expected = (double) N_TRIALS / N_RANGES;
   for (i = 0; ok && i < N_RANGES; i++) {
      if (counts[i] < 0.5 * expected) {
         reportError("Low count for outcome %d", i);
         ok = false;
      } else if (counts[i] > 1.5 * expected) {
         reportError("High count for outcome %d", i);
         ok = false;
      }
   }
   return ok;
}

static bool randomChanceSeemsReasonable(double p) {
   int i, hits;
   double expected;

   hits = 0;
   for (i = 0; i < N_TRIALS; i++) {
      if (randomChance(p)) hits++;
   }
   expected = p * N_TRIALS;
   if (hits < 0.5 * expected || (p == 1.0 && hits != N_TRIALS)) {
      reportError("Hit count low");
      return false;
   } else if (hits > 1.5 * expected || (p == 0.0 && hits != 0)) {
      reportError("Hit count high");
      return false;
   }
   return true;
}

static bool hugeRangeSeemsReasonable() {
   unsigned maskHigh;
   int i, k, rangeBit, rangesLeft;

   maskHigh = ((unsigned) INT_MAX + 1) >> 1;
   rangesLeft = 0xF;
   for (i = 0; i < N_TRIALS && rangesLeft; i++) {
      k = randomInteger(INT_MIN, INT_MAX);
      if (k < 0) {
         rangeBit = (k & maskHigh) ? 1 : 2;
      } else {
         rangeBit = (k & maskHigh) ? 8 : 4;
      }
      rangesLeft &= ~rangeBit;
   }
   if (rangesLeft & 1) {
      reportError("No large negative values generated");
   }
   if (rangesLeft & 2) {
      reportError("No small negative values generated");
   }
   if (rangesLeft & 4) {
      reportError("No small positive values generated");
   }
   if (rangesLeft & 8) {
      reportError("No large positive values generated");
   }
   return rangesLeft == 0;
}

#endif
