/*
 * File: gtimer.h
 * --------------
 * This interface exports a general interval timer.
 */

#ifndef _gtimer_h
#define _gtimer_h

#include "cslib.h"

/*
 * Type: GTimer
 * ------------
 * This type implements a simple interval timer that generates a timer
 * event with a specified frequency.
 */

typedef struct GTimerCDT *GTimer;

/*
 * Function: newGTimer
 * Usage: timer = newGTimer(milliseconds);
 * ---------------------------------------
 * Creates a timer that generates a timer event each time the specified
 * number of milliseconds has elapsed.  No events are generated until
 * the client calls <code>startTimer</code>.
 */

GTimer newGTimer(double milliseconds);

/*
 * Function: freeGTimer
 * Usage: freeGTimer(timer);
 * -------------------------
 * Frees the resources associated with the timer.
 */

void freeGTimer(GTimer timer);

/*
 * Function: startTimer
 * Usage: startTimer(timer);
 * -------------------------
 * Starts the timer.  A timer continues to generate timer events until it
 * is stopped; to achieve the effect of a one-shot timer, the simplest
 * approach is to call the <code>stop</code> function inside the event
 * handler.
 */

void startTimer(GTimer timer);

/*
 * Function: stopTimer
 * Usage: stopTimer(timer);
 * ------------------------
 * Stops the timer so that it stops generating events.
 */

void stopTimer(GTimer timer);

#endif
