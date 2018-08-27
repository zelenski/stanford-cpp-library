/*
 * File: gtimer.cpp
 * ----------------
 * This file implements the gtimer.h interface.
 * 
 * @version 2018/07/08
 * - fixed bug with stale timer events after timer is destructed
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - removed unneeded include statements
 */

#include "gtimer.h"
#include <iostream>
#include <sstream>
#include "private/platform.h"

/* Implementation of the GTimer class */

std::int64_t GTimer::nextID = 0;

GTimer::GTimer(double milliseconds) {
    /* Assign a unique ID to this GTimer, and set the deleter on the
     * shared_ptr so that it will tell the system when we're done with
     * that underlying timer.
     */
    std::int64_t myID = nextID++;
    gtd = std::shared_ptr<std::int64_t>(new std::int64_t(myID), [myID](std::int64_t* memory) {
        stanfordcpplib::getPlatform()->gtimer_delete(std::to_string(myID));
        delete memory;
    });

    stanfordcpplib::getPlatform()->gtimer_constructor(*this, milliseconds);
}

GTimer::GTimer(std::shared_ptr<std::int64_t> id) : gtd(id) {
    // Handled in initializer list
}

std::string GTimer::getID() const {
    return gtd? std::to_string(*gtd) : "-1";
}

void GTimer::start() {
    stanfordcpplib::getPlatform()->gtimer_start(*this);
}

void GTimer::stop() {
    stanfordcpplib::getPlatform()->gtimer_stop(*this);
}

bool GTimer::operator ==(const GTimer& t2) {
    return gtd == t2.gtd;
}

bool GTimer::operator !=(const GTimer& t2) {
    return gtd != t2.gtd;
}
