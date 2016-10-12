/*
 * File: observable.cpp
 * --------------------
 * This file defines the implementation of the superclass <code>Observable</code>.
 * See observable.h for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - fixed bug in error string on removeObserver (said 'addObserver')
 * @since 2014/03/09
 */

#include "observable.h"

void Observable::addObserver(Observer* obs) {
    if (!obs) {
        error("Observable::addObserver: NULL observer passed");
    }
    m_observers.insert(obs);
}

void Observable::notifyObservers(void* arg) {
    for (Observer* obs : m_observers) {
        obs->update(this, arg);
    }
}

void Observable::removeObserver(Observer* obs) {
    if (!obs) {
        error("Observable::removeObserver: NULL observer passed");
    }
    m_observers.erase(obs);
}
