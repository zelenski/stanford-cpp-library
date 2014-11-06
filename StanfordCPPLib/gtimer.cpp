/*
 * File: gtimer.cpp
 * ----------------
 * This file implements the gtimer.h interface.
 * 
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - removed unneeded include statements
 */

#include "gtimer.h"
#include "platform.h"

/* Global variables */

static Platform *pp = getPlatform();

/* Implementation of the GTimer class */

GTimer::GTimer(double milliseconds) {
    gtd = new GTimerData();
    gtd->refCount = 1;
    pp->gtimer_constructor(*this, milliseconds);
}

GTimer::~GTimer() {
    if (--gtd->refCount == 0) delete gtd;
}

void GTimer::start() {
    pp->gtimer_start(*this);
}

void GTimer::stop() {
    pp->gtimer_stop(*this);
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
        if (--gtd->refCount == 0) delete gtd;
        this->gtd = src.gtd;
        this->gtd->refCount++;
    }
    return *this;
}
