/*
 * File: tplatform.h
 * -----------------
 * This interface defines the platform-specific methods on threads
 * and locks.
 */

/* Methods for threads */

int forkForPlatform(void (*fn)(void *), void *arg);
void incThreadRefCountForPlatform(int id);
void decThreadRefCountForPlatform(int id);
void joinForPlatform(int id);
int getCurrentThreadForPlatform();
void yieldForPlatform();

/* Methods for locks */

int initLockForPlatform();
void incLockRefCountForPlatform(int id);
void decLockRefCountForPlatform(int id);
void lockForPlatform(int id);
void unlockForPlatform(int id);
void waitForPlatform(int id);
void signalForPlatform(int id);
