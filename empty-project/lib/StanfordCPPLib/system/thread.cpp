/*
 * File: thread.cpp
 * ----------------
 * This file implements the platform-independent parts of the thread package.
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
#include <sstream>
#include <string>
#include "thread.h"
#include "private/tplatform.h"

Thread::Thread() {
    id = -1;
}

Thread::~Thread() {
    /* Empty */
}

std::string Thread::toString() {
    std::ostringstream stream;
    stream << "Thread" << id;
    return stream.str();
}

static void forkWithVoid(void *arg);

Thread forkThread(void (*fn)()) {
    Thread thread;
    StartWithVoid startup = { fn };
    thread.id = forkForPlatform(forkWithVoid, &startup);
    return thread;
}

void joinThread(Thread& thread) {
    joinForPlatform(thread.id);
}

void yieldThread() {
    yieldForPlatform();
}

Thread getCurrentThread() {
    Thread thread;
    thread.id = getCurrentThreadForPlatform();
    return thread;
}

Lock::Lock() {
    id = initLockForPlatform();
}

Lock::~Lock() {
    decLockRefCountForPlatform(id);
}

void Lock::wait() {
    waitForPlatform(id);
}

void Lock::signal() {
    signalForPlatform(id);
}

static void forkWithVoid(void *arg) {
    StartWithVoid* startup = (StartWithVoid*) arg;
    startup->fn();
}
