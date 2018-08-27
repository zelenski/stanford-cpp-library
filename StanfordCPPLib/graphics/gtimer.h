/*
 * File: gtimer.h
 * --------------
 * This file defines the <code>GTimer</code> class, which implements a
 * general interval timer.
 *
 * @version 2018/07/08
 * - fixed bug with stale timer events after timer is destructed
 */

#ifndef _gtimer_h
#define _gtimer_h

#include <string>
#include <memory>
#include <cstdint>

namespace stanfordcpplib {
class Platform;
}

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
     * Method: getID
     * Usage: int id = timer.getID();
     * ------------------------------
     * Returns a unique ID string for this timer.
     * The ID is related to the timer's memory address along with a unique number
     * that increments each time a timer is created.
     */
    std::string getID() const;

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
    bool operator ==(const GTimer& t2);

    /*
     * Friend operator: !=
     * Usage: if (t1 != t2) ...
     * ------------------------
     * Checks whether the two objects refer to the different timers.
     */
    bool operator !=(const GTimer& t2);

    /**
     * Constructor: GTimer(shared_ptr<int64_t> id);
     * ----------------------------------
     * Creates a timer that shares state with another timer. This is used
     * internally by the event system, and you should not use this
     * constructor on your own.
     */
    GTimer(std::shared_ptr<std::int64_t> id);

private:
    /* Instance variables */
    std::shared_ptr<std::int64_t> gtd;

    static std::int64_t nextID;

    friend class stanfordcpplib::Platform;
    friend class GTimerEvent;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gtimer_h
