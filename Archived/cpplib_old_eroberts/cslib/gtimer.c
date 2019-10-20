/*
 * File: gtimer.c
 * --------------
 * This file implements the gtimer.h interface.  The functions that
 * implement receiving timer from the environment are implemented in
 * the platform package.
 */

#include <stdio.h>
#include "cslib.h"
#include "gtimer.h"
#include "platform.h"

/*
 * Type GTimerCDT
 * --------------
 * This type is the concrete type for the timer.
 */

struct GTimerCDT {
   double duration;
};

GTimer newGTimer(double milliseconds) {
   GTimer timer;

   timer = newBlock(GTimer);
   timer->duration = milliseconds;
   createTimerOp(timer, milliseconds);
   return timer;
}

void freeGTimer(GTimer timer) {
   deleteTimerOp(timer);
}

void startTimer(GTimer timer) {
   startTimerOp(timer);
}

void stopTimer(GTimer timer) {
   stopTimerOp(timer);
}

/**********************************************************************/
/* Unit test for the gtimer module                                    */
/**********************************************************************/

#ifndef _NOTEST_

#include "unittest.h"

/* Unit test */

void testGTimerModule(void) {
   // Fill in
}

#endif
