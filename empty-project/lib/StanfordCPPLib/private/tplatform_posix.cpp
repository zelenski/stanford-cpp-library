/*
 * File: tplatform_posix.cpp
 * -------------------------
 * Implements the platform-specific code for threads using Posix threads.
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
/* Copyright (c) 2014 by Eric Roberts <eroberts@cs.stanford.edu>         */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

#include <iostream>
#include <string>
#include <pthread.h>
#include "error.h"
#include "map.h"
#include "tplatform.h"

#define __macosx__ 1
#ifdef __macosx__
extern int sched_yield(void);
#define pthread_yield sched_yield
#endif

struct ThreadData {
    int id;
    pthread_t pid;
    bool terminated;
    void (* fn)(void* arg);
    void* arg;
    int refCount;
};

struct LockData {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    int depth;
    int owner;
    int refCount;
};

/* Constants */

#define MAX_THREAD_ID int(unsigned(-1) >> 1)
#define MAX_LOCK_ID    int(unsigned(-1) >> 1)

/* Private function prototypes */

static Map<int, ThreadData*>& getThreadDataMap();
static Map<int, LockData*>& getLockDataMap();
static int getNextFreeThread();
static int getNextFreeLock();
static pthread_key_t& getThreadDataKey();
static pthread_key_t* createThreadDataKey();
static void* startThread(void* arg);

/* Functions */

int forkForPlatform(void (* fn)(void*), void* arg) {
    int id = getNextFreeThread();
    ThreadData* tdp = new ThreadData;
    tdp->terminated = false;
    tdp->refCount = 1;
    tdp->fn = fn;
    tdp->arg = arg;
    getThreadDataMap().put(id, tdp);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int osErr = pthread_create(&tdp->pid, &attr, startThread, tdp);
    pthread_attr_destroy(&attr);
    if (osErr != 0) {
        error("forkThread: Can't create new thread");
    }
    return id;
}

void incThreadRefCountForPlatform(int /*id*/) {
    // Fill in
}

void decThreadRefCountForPlatform(int /*id*/) {
    // Fill in
}

void joinForPlatform(int id) {
    pthread_join(getThreadDataMap().get(id)->pid, nullptr);
}

int getCurrentThreadForPlatform() {
    ThreadData* tdp = (ThreadData*) pthread_getspecific(getThreadDataKey());
    if (!tdp) {
        tdp = new ThreadData;
        tdp->id = 0;
        pthread_setspecific(getThreadDataKey(), tdp);
    }
    return tdp->id;
}

void yieldForPlatform() {
    pthread_yield();
}

int initLockForPlatform() {
    int id = getNextFreeLock();
    LockData* ldp = new LockData;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&ldp->mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    pthread_cond_init(&ldp->condition, nullptr);
    ldp->owner = -1;
    ldp->depth = 0;
    getLockDataMap().put(id, ldp);
    return id;
}

void incLockRefCountForPlatform(int /*id*/) {
    // Fill in
}

void decLockRefCountForPlatform(int /*id*/) {
    // Fill in
}

void lockForPlatform(int id) {
    LockData* ldp = getLockDataMap().get(id);
    pthread_mutex_lock(&ldp->mutex);
    if (ldp->depth++ == 0) {
        ldp->owner = getCurrentThreadForPlatform();
    }
}

void unlockForPlatform(int id) {
    LockData* ldp = getLockDataMap().get(id);
    if (--ldp->depth == 0) {
        ldp->owner = -1;
    }
    pthread_mutex_unlock(&ldp->mutex);
}

void waitForPlatform(int id) {
    LockData* ldp = getLockDataMap().get(id);
    pthread_cond_wait(&ldp->condition, &ldp->mutex);
}

void signalForPlatform(int id) {
    LockData* ldp = getLockDataMap().get(id);
    pthread_cond_broadcast(&ldp->condition);
}

/* Static functions */

static Map<int, ThreadData*>& getThreadDataMap() {
    static Map<int, ThreadData*>* mp = new Map<int, ThreadData*>();
    return *mp;
}

static Map<int, LockData*>& getLockDataMap() {
    static Map<int,LockData*>* mp = new Map<int, LockData*>();
    return *mp;
}

static int getNextFreeThread() {
    static int nextThread = 1;
    Map<int, ThreadData*>& map = getThreadDataMap();
    while (map.containsKey(nextThread)) {
        nextThread++;
        if (nextThread == MAX_THREAD_ID) {
            nextThread = 1;
        }
    }
    return nextThread++;
}

static int getNextFreeLock() {
    static int nextLock = 1;
    Map<int, LockData*>& map = getLockDataMap();
    while (map.containsKey(nextLock)) {
        nextLock++;
        if (nextLock == MAX_LOCK_ID) {
            nextLock = 1;
        }
    }
    return nextLock++;
}

static pthread_key_t& getThreadDataKey() {
    static pthread_key_t* pkp = createThreadDataKey();
    return *pkp;
}

static pthread_key_t* createThreadDataKey() {
    pthread_key_t* pkp = new pthread_key_t;
    pthread_key_create(pkp, nullptr);
    return pkp;
}

static void* startThread(void* arg) {
    ThreadData* tdp = (ThreadData*) arg;
    pthread_setspecific(getThreadDataKey(), tdp);
    try {
        tdp->fn(tdp->arg);
    } catch (ErrorException e) {
        std::cerr << "Error: " << e.getMessage() << std::endl;
        std::exit(1);
    }
    return nullptr;
}
