#include "observable.h"
using namespace std;

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
        error("Observable::addObserver: NULL observer passed");
    }
    m_observers.erase(obs);
}
