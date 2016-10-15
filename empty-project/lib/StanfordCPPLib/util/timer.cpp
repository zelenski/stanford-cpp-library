/*
 * File: timer.cpp
 * ---------------
 * Implementation of the Timer class as declared in timer.h.
 */

#include "timer.h"
#include <sys/time.h>
#include "error.h"

Timer::Timer(bool autostart) {
    m_startMS = 0;
    m_stopMS = 0;
    m_isStarted = false;
    if (autostart) {
        start();
    }
}

long Timer::elapsed() const {
    return m_stopMS - m_startMS;
}

bool Timer::isStarted() const {
    return m_isStarted;
}

void Timer::start() {
    m_startMS = currentTimeMS();
    m_isStarted = true;
}

long Timer::stop() {
    m_stopMS = currentTimeMS();
    if (!m_isStarted) {
        // error("Timer is not started");
        m_startMS = m_stopMS;
    }
    m_isStarted = false;
    return elapsed();
}

long Timer::currentTimeMS() {
    timeval time;
    gettimeofday(&time, nullptr);
    return (time.tv_sec * 1000000 + time.tv_usec) / 1000;
}
