/*
 * File: timer.h
 * -------------
 * This file exports a Timer class that is useful for measuring the elapsed
 * time of a program in milliseconds over a given interval.
 */

#ifndef _timer_h
#define _timer_h

class Timer {
public:
    /*
     * Constructs a new timer.
     * If an optional bool parameter of true is passed, also starts the timer.
     */
    Timer(bool autostart = false);

    /*
     * Returns the number of milliseconds that have elapsed since this timer
     * was started.
     * Returns 0 if the timer was never started.
     */
    long elapsed() const;

    /*
     * Returns true if the timer has been started.
     */
    bool isStarted() const;

    /*
     * Starts the timer.
     * If the timer was already started, restarts it such that its 'elapsed'
     * time will be 0 at the moment of the call.
     */
    void start();

    /*
     * Stops the timer and returns the number of elapsed milliseconds.
     * If the timer was never started, 0 ms will be considered to have elapsed.
     */
    long stop();

    /*
     * A static utility function for getting the current time as a Unix
     * timestamp of milliseconds since the epoch.
     */
    static long currentTimeMS();

private:
    /* instance variables */
    long m_startMS;
    long m_stopMS;
    bool m_isStarted;
};

#endif // _timer_h
