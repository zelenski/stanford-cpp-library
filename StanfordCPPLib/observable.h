/*
 * File: observable.h
 * ------------------
 * This file defines an abstract superclass named <code>Observable</code> that
 * allows objects to store lists of observers, which are other objects that are
 * notified when some part of the state of the observable object changes.
 * This is an example of the classic Observer/Observable design pattern.
 *
 * @author Marty Stepp
 * @version 2014/03/09
 */

#ifndef _observable_h
#define _observable_h

#include <set>
#include "error.h"

class Observable;
class Observer;

/*
 * This abstract superclass allows objects to store lists of observers,
 * which are other objects that would like to be notified when some part of the
 * state of the observable object changes.
 *
 * The intended usage is that you should extend Observable in the object you
 * want to be watched, then call notifyObservers in its code at appropriate
 * places.  Then create some other object that extends Observer and defines an
 * update method, and attach it to the Observable so it will be notified.
 */
class Observable {
public:
    /*
     * Adds the given observer object to this observable object's internal list
     * of observers.  The observer's update method will be called when the
     * notifyObservers method is called afterward.
     * Precondition: obs != NULL
     */
    void addObserver(Observer* obs);

    /*
     * Calls the update method of all observers that have been added previously
     * to this observable object.
     * The given argument can be passed to provide extra information to the
     * observers if necessary.  If no argument is passed, NULL is used.
     */
    void notifyObservers(void* arg = NULL);

    /*
     * Removes the given observer object from this observable object's internal
     * list of observers.  The observer will no longer be notified.
     */
    void removeObserver(Observer* obs);

private:
    /*
     * A list of observers to notify when notifyObservers is called.
     */
    std::set<Observer*> m_observers;
};

/*
 * An object that wishes to be notified when the state of an observable object
 * changes.
 */
class Observer {
public:
    /*
     * Called by an Observable to inform this observer that its state changed.
     * The 'obs' parameter will be a pointer to the observable object itself
     * on which the state change occurred.  The 'arg' parameter will be
     * the extra information passed by the Observable when it called
     * notifyObservers, if any.
     */
    virtual void update(Observable* obs, void* arg = NULL) = 0;
};

#endif
