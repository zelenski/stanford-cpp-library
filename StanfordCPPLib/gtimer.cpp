/*
 * File: gtimer.cpp
 * ----------------
 * This file implements the gtimer.h interface.
 * 
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - removed unneeded include statements
 */

#include "gtimer.h"
#include "private/platform.h"

/* Implementation of the GTimer class */

GTimer::GTimer(double milliseconds) {
    gtd = new GTimerData();
    gtd->refCount = 1;
    stanfordcpplib::getPlatform()->gtimer_constructor(*this, milliseconds);
}

GTimer::~GTimer() {
    if (--gtd->refCount == 0) {
        delete gtd;
    }
}

void GTimer::start() {
    stanfordcpplib::getPlatform()->gtimer_start(*this);
}

void GTimer::stop() {
    stanfordcpplib::getPlatform()->gtimer_stop(*this);
}

bool GTimer::operator==(GTimer t2) {
    return gtd == t2.gtd;
}

bool GTimer::operator!=(GTimer t2) {
    return gtd != t2.gtd;
}

GTimer::GTimer(GTimerData *gtd) {
    this->gtd = gtd;
    gtd->refCount++;
}

GTimer::GTimer(const GTimer & src) {
    this->gtd = src.gtd;
    this->gtd->refCount++;
}

GTimer & GTimer::operator=(const GTimer & src) {
    if (this != &src) {
        if (--gtd->refCount == 0) {
            delete gtd;
        }
        this->gtd = src.gtd;
        this->gtd->refCount++;
    }
    return *this;
}
