/*
 * File: thread_platform.h
 * -----------------------
 * This interface defines the platform-specific functions on threads
 * and locks.
 */

/* Functions for threads */

int forkForPlatform(void (*fn)(void *), void *arg);
void incThreadRefCountForPlatform(int id);
void decThreadRefCountForPlatform(int id);
void joinForPlatform(int id);
int getCurrentThreadForPlatform();
void yieldForPlatform();

/* Functions for locks */

int initLockForPlatform();
void incLockRefCountForPlatform(int id);
void decLockRefCountForPlatform(int id);
void lockForPlatform(int id);
void unlockForPlatform(int id);
void waitForPlatform(int id);
void signalForPlatform(int id);

