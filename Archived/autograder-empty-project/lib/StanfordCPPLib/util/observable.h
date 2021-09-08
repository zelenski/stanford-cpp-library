/*
 * File: observable.h
 * ------------------
 * This file defines an abstract superclass named <code>Observable</code> that
 * allows objects to store lists of observers, which are other objects that are
 * notified when some part of the state of the observable object changes.
 * This is an example of the classic Observer/Observable design pattern.
 *
 * @author Marty Stepp
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2017/10/25
 * - added addObserver/removeObserver overloads that accept reference param
 * @version 2016/11/20
 * - refactored to use template for event type
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - fixed bug in error string on removeObserver (said 'addObserver')
 * @version 2014/03/09
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _observable_h
#define _observable_h

#include <set>

#define INTERNAL_INCLUDE 1
#include "error.h"
#undef INTERNAL_INCLUDE

// forward declarations
template <class T>
class Observer;

/**
 * This abstract superclass allows objects to store lists of observers,
 * which are other objects that would like to be notified when some part of the
 * state of the observable object changes.
 *
 * The intended usage is that you should extend Observable in the object you
 * want to be watched, then call notifyObservers in its code at appropriate
 * places.  Then create some other object that extends Observer and defines an
 * update method, and attach it to the Observable so it will be notified.
 */
template <typename T>
class Observable {
public:
    /**
     * Adds the given observer object to this observable object's internal list
     * of observers.  The observer's update method will be called when the
     * notifyObservers method is called afterward.
     * Precondition: obs != nullptr
     */
    void addObserver(Observer<T>* obs);

    /**
     * Adds the given observer object to this observable object's internal list
     * of observers.  The observer's update method will be called when the
     * notifyObservers method is called afterward.
     */
    void addObserver(Observer<T>& obs);

    /**
     * Calls the update method of all observers that have been added previously
     * to this observable object.
     * The given argument can be passed to provide extra information to the
     * observers if necessary.  If no argument is passed, nullptr is used.
     */
    void notifyObservers(T arg = T());

    /**
     * Removes the given observer object from this observable object's internal
     * list of observers.  The observer will no longer be notified.
     */
    void removeObserver(Observer<T>* obs);

    /**
     * Removes the given observer object from this observable object's internal
     * list of observers.  The observer will no longer be notified.
     */
    void removeObserver(Observer<T>& obs);

private:
    // a list of observers to notify when notifyObservers is called
    std::set<Observer<T>*> m_observers;
};

/**
 * An object that wishes to be notified when the state of an observable object
 * changes.
 */
template <typename T>
class Observer {
public:
    /**
     * Called by an Observable to inform this observer that its state changed.
     * The 'obs' parameter will be a pointer to the observable object itself
     * on which the state change occurred.  The 'arg' parameter will be
     * the extra information passed by the Observable when it called
     * notifyObservers, if any.
     */
    virtual void update(Observable<T>* obs, const T& arg = T()) = 0;
};

template <typename T>
void Observable<T>::addObserver(Observer<T>* obs) {
    if (!obs) {
        error("Observable::addObserver: null observer passed");
    }
    m_observers.insert(obs);
}

template <typename T>
void Observable<T>::addObserver(Observer<T>& obs) {
    addObserver(&obs);
}

template <typename T>
void Observable<T>::notifyObservers(T arg) {
    for (Observer<T>* obs : m_observers) {
        obs->update(this, arg);
    }
}

template <typename T>
void Observable<T>::removeObserver(Observer<T>* obs) {
    if (!obs) {
        error("Observable::removeObserver: null observer passed");
    }
    m_observers.erase(obs);
}

template <typename T>
void Observable<T>::removeObserver(Observer<T>& obs) {
    removeObserver(&obs);
}

#endif // _observable_h
