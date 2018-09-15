/*
 * File: thread.h
 * --------------
 * This interface exports a platform-independent thread abstraction,
 * along with simple functions for concurrency control.
 */

#ifndef _thread_h
#define _thread_h

#include <setjmp.h>
#include "cslib.h"

#ifdef __macosx__
#  define getCurrentThread XgetCurrentThread
#endif

/*
 * Type: Thread
 * ------------
 * The <code>Thread</code> type is used to represent a <i>thread,</i>
 * which is a lightweight process running in the same address space
 * as the creator.
 */

typedef struct ThreadCDT *Thread;

/*
 * Type: Lock
 * ----------
 * The <code>Lock</code> type is used to manage concurrent access
 * to some data structure within an application.  Only one thread
 * can hold a lock at any point in time; other threads seeking to
 * gain access queue on the lock until it is released by the thread
 * that originally obtained it.  The general strategy for using a
 * lock is to use the <code>synchronized</code> statement to
 * protect a critical region of code, as illustrated in the
 * discussion of <code>synchronized</code> later in this file.
 */

typedef struct LockCDT *Lock;

/* Exported entries */

/*
 * Function: forkThread
 * Usage: thread = forkThread(fn, data);
 * -------------------------------------
 * Forks a new thread to invokes the specified function, passing
 * <code>data</code> as an argument.  Threads created by
 * <code>forkThread</code> become dormant on completion and
 * wait for the client to synchronize with them using a
 * <code>joinThread</code> operation.
 */

Thread forkThread(proc fn, void *data);

/*
 * Function: joinThread
 * Usage: joinThread(thread);
 * --------------------------
 * Waits for the specified thread to finish before proceeding.
 */

void joinThread(Thread thread);

/*
 * Function: yield
 * Usage: yield();
 * ---------------
 * Yields the processor to allow another thread to run.
 */

void yield(void);

/*
 * Function: getCurrentThread
 * Usage: self = getCurrentThread();
 * ---------------------------------
 * Returns the currently executing thread.
 */

Thread getCurrentThread(void);

/*
 * Function: setThreadName
 * Usage: setThreadName(thread, name);
 * -----------------------------------
 * Sets the name of a thread to the given string.  This name
 * is used primarily for debugging purposes.
 */

void setThreadName(Thread thread, string name);

/*
 * Function: getThreadName
 * Usage: name = getThreadName(thread);
 * ------------------------------------
 * Returns the name of the specified thread.  If no name has been
 * set for the thread, this function returns a string in the form
 * <code>Thread&lt;xxx&gt;</code>, where <code>xxx</code> is an
 * integer uniquely identifying the thread.
 */

string getThreadName(Thread thread);

/*
 * Function: newLock
 * Usage: lock = newLock();
 * ------------------------
 * Creates a new <code>Lock</code> object.
 */

Lock newLock(void);

/*
 * Macro: synchronized
 * Usage: synchronized (lock) { . . . }
 * ------------------------------------
 * Defines a critical section protected by the specified lock.  The
 * general strategy for using this facility is shown in the following
 * paradigmatic pattern:<p>
 *
 *<pre>
 *    synchronized (lock) {
 *       . . . statements in the critical section . . .
 *    }
 *</pre>
 */

#define synchronized(lock) for ( ; startSync(lock) ; endSync(lock))

/*
 * Function: waitThread
 * Usage: waitThread(lock);
 * ------------------------
 * Waits for some other thread to issue a call to <code>signalThread</code>
 * on this lock.  This call requires that the lock be owned by the
 * calling thread.  The effect of this function is to release the
 * lock and then wait until the desired <code>signalThread</code> operation
 * occurs, at which point the lock is reacquired and control passes
 * to the statement following the <code>waitThread</code>.
 *
 * <p>The <code>waitThread</code> function is useful only if the call is
 * embedded inside a <code>while</code> loop that checks a condition
 * before proceeding.  That <code>while</code> statement must itself
 * be embedded inside a <code>synchronized</code> statement that
 * acquires the lock.  Thus, the standard paradigm for using the
 * <code>waitThread</code> function looks like this:<p>
 *
 *<pre>
 *    synchronized (lock) {
 *        while (conditional test) {
 *            waitThread(lock);
 *        }
 *        . . . code to manipulate the locked resource . . .
 *    }
 *</pre>
 */

void waitThread(Lock lock);

/*
 * Function: signalThread
 * Usage: signalThread(lock);
 * --------------------------
 * Signals all threads waiting on the lock so that they wake up and
 * recheck the corresponding condition.
 */

void signalThread(Lock lock);

/* Internal entry points -- see implementation for details */

bool startSync(Lock lock);
void endSync(Lock lock);
void lockInternal(Lock lock);
void unlockInternal(Lock lock);

#endif
