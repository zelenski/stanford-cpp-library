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
     * on the timer.
     *
     * Due to implementation details, you must create at least one GWindow
     * before you can start() a GTimer object.
     *
     * For more details on using timers, see the documentation
     * for the <a href="GTimerEvent-class.html"><code>GTimerEvent</code></a>
     * class.
     */
    GTimer(double milliseconds);

    /*
     * Method: getDelay
     * Usage: double delay = timer.getDelay();
     * ---------------------------------------
     * Returns the delay in milliseconds between each tick of this timer.
     */
    double getDelay() const;

    /*
     * Method: isStarted
     * Usage: if (timer.isStarted()) { ... }
     * -------------------------------------
     * Returns true if the given timer has been started (via start()).
     * If you stop the timer or have not started it yet,
     * this method will return false.
     */
    bool isStarted() const;

    /*
     * Method: restart
     * Usage: timer.restart();
     * -----------------------
     * Stops the timer (if it was started) and then starts it again.
     */
    void restart();

    /*
     * Method: setDelay
     * Usage: timer.setDelay(delay);
     * -----------------------------
     * Changes the delay in milliseconds between each tick of this timer.
     * If the timer is currently running, calling this method will stop
     * and restart the timer with the new delay.
     */
    void setDelay(double ms);

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

private:
    /* Instance variables */
    double _ms;
    int _id;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gtimer_h
