/*
 * File: private/randompatch.h
 * ---------------------------
 * This file patches the implementation of the random number library
 * to avoid some serious bugs in standard implementations of rand,
 * particularly on Mac OS X.  It also includes a hack to set the
 * seed from the RANDOM_SEED environment variable, which makes it
 * possible to produce repeatable figures.
 */

/*
 * Implementation notes: rand, srand
 * ---------------------------------
 * To ensure that this package works the same way on all platforms,
 * this file completely reimplements the rand and srand methods.  The
 * algorithm is a conventional linear congruential generator with the
 * parameters used in GNU's gclib.  RAND_MAX for this implementation
 * is 2147483647 (2^31 - 1).
 */

#define MULTIPLIER 1103515245
#define OFFSET 12345

static int _seed = 1;

#undef rand
#define rand() ((_seed = MULTIPLIER * _seed + OFFSET) & 0x7FFFFFFF)

#undef srand
#define srand(seed) (_seed = int(seed), _seed = (_seed <= 0) ? 1 : _seed)

#undef RAND_MAX
#define RAND_MAX 2147483647

/*
 * Implementation notes: Windows patch
 * -----------------------------------
 * On some versions of Windows, the time function is too coarse to use
 * as a random seed.  On those versions, this definition substitutes the
 * GetTickCount function.
 */

#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#  include <windows.h>
#  define time(dummy) (GetTickCount())
#endif

#ifdef __APPLE__

#  include <cstdlib>

   static time_t patchedTime(time_t *) {
      char *str = getenv("RANDOM_SEED");
      if (!str) {
         return time(nullptr);
      } else {
         return atoi(str);
      }
   }

#  define time(dummy) patchedTime(dummy)

#endif
