/*
 * File: gtimer.h
 * --------------
 * This file defines the <code>GTimer</code> class, which implements a
 * general interval timer.
 */

#ifndef _gtimer_h
#define _gtimer_h

#include <string>

/*
 * Friend type: GTimerData
 * -----------------------
 * This type maintains a reference count to determine when it is
 * possible to free the timer.  The address of this block is used
 * as the timer id.
 */

struct GTimerData {
   int refCount;
};

/*
 * Class: GTimer
 * -------------
 * This class implements a simple interval timer that generates a
 * <code>GTimerEvent</code> with a specified frequency.  Copying
 * a <code>GTimer</code> object is legal and creates an object that
 * refers to the same internal timer.
 */

class GTimer {

public:

/*
 * Constructor: GTimer
 * Usage: GTimer timer(milliseconds);
 * ----------------------------------
 * Creates a timer object that generates a <code>GTimerEvent</code>
 * each time the specified number of milliseconds has elapsed.  No
 * events are generated until the client calls <code>start</code>
 * on the timer.  For more details on using timers, see the documentation
 * for the <a href="GTimerEvent-class.html"><code>GTimerEvent</code></a>
 * class.
 */

   GTimer(double milliseconds);

/*
 * Destructor: ~GTimer
 * -------------------
 * Frees the resources associated with the timer.
 */

   virtual ~GTimer();

/*
 * Method: start
 * Usage: timer.start();
 * ---------------------
 * Starts the timer.  A timer continues to generate timer events until it
 * is stopped; to achieve the effect of a one-shot timer, the simplest
 * approach is to call the <code>stop</code> method inside the event
 * handler.
 */

   void start();

/*
 * Method: stop
 * Usage: timer.stop();
 * --------------------
 * Stops the timer so that it stops generating events until it is restarted.
 */

   void stop();

/*
 * Friend operator: ==
 * Usage: if (t1 == t2) ...
 * ------------------------
 * Checks whether the two objects refer to the same timer.
 */

   bool operator==(GTimer t2);

/*
 * Friend operator: !=
 * Usage: if (t1 != t2) ...
 * ------------------------
 * Checks whether the two objects refer to the different timers.
 */

   bool operator!=(GTimer t2);

/* Private section */

   GTimer(GTimerData *gtd);
   GTimer(const GTimer & src);
   GTimer & operator=(const GTimer & src);

private:

/* Instance variables */

   GTimerData *gtd;

   friend class Platform;
   friend class GTimerEvent;

};

#endif
