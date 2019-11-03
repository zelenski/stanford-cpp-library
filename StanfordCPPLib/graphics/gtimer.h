/*
 * File: gtimer.h
 * --------------
 * This file defines the <code>GTimer</code> class, which implements a
 * general interval timer.
 *
 * @version 2019/01/23
 * - added destructor
 * @version 2018/09/09
 * - updated to use new Qt GUI timer interface
 * - added doc comments for new documentation generation
 */


#ifndef _gtimer_h
#define _gtimer_h

#include <string>

/**
 * This class implements a simple interval timer that generates a
 * <code>GTimerEvent</code> with a specified frequency.
 */
class GTimer {
public:
    /**
     * Creates a timer object that generates a <code>GTimerEvent</code>
     * each time the specified number of milliseconds has elapsed.  No
     * events are generated until the client calls <code>start</code>
     * on the timer.
     *
     * Due to implementation details, you must create at least one GWindow
     * before you can start() a GTimer object.
     *
     * @throw ErrorException if milliseconds is negative
     */
    GTimer(double milliseconds);

    /**
     * Destroys the timer, stopping it if it's currently running.
     */
    ~GTimer();

    /**
     * Returns the delay in milliseconds between each tick of this timer.
     */
    double getDelay() const;

    /**
     * Method: isStarted
     * Usage: if (timer.isStarted()) { ... }
     * -------------------------------------
     * Returns true if the given timer has been started (via start()).
     * If you stop the timer or have not started it yet,
     * this method will return false.
     */
    bool isStarted() const;

    /**
     * Stops the timer (if it was started) and then starts it again.
     */
    void restart();

    /**
     * Changes the delay in milliseconds between each tick of this timer.
     * If the timer is currently running, calling this method will stop
     * and restart the timer with the new delay.
     *
     * @throw ErrorException if milliseconds is negative
     */
    void setDelay(double ms);

    /**
     * Starts the timer.  A timer continues to generate timer events until it
     * is stopped; to achieve the effect of a one-shot timer, the simplest
     * approach is to call the <code>stop</code> method inside the event
     * handler.
     */
    void start();

    /**
     * Stops the timer so that it stops generating events until it is restarted.
     */
    void stop();

private:
    double _ms;
    int _id;
};

#endif // _gtimer_h
