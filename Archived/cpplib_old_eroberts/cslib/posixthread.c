/*
 * File: posixthread.c
 * Last modified on Sat Aug 13 02:48:52 2011 by eroberts
 * -----------------------------------------------------
 * This file is the top-level file in the implementation of the
 * thread.h interface using POSIX threads.
 */

#ifdef __unix__

#define _GNU_SOURCE 1

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "exception.h"
#include "cslib.h"
#include "strlib.h"
#include "thread.h"

/* Account for differences in the various implementations */

#if defined(__macosx__)
   extern int sched_yield(void);
#  define pthread_yield sched_yield
#else
#  if !defined(__linux__)
#  define pthread_yield() usleep(1);
#  endif
#endif

/* Constants */

#define MAX_AUTOMATIC_THREAD_NAME 30

/* Define the concrete type for a thread */

struct ThreadCDT {
   pthread_t id;
   string name;
   bool terminated;
   Lock lock;
};

/* Define the concrete type for a lock */

struct LockCDT {
   pthread_mutex_t mutex;
   pthread_cond_t condition;
   bool exitFlag;
   int depth;
   Thread owner;
};

/* Define a data block as a wrapper for the thread data */

typedef struct {
   proc fn;
   void *data;
   Thread thread;
} *ThreadDataBlock;

/* Static variables used to implement the package */

static pthread_key_t threadKey;
static pthread_key_t exceptionStackKey;
static pthread_once_t onceOnly = PTHREAD_ONCE_INIT;

/* Internal function prototypes */

ExceptionContextBlock *getThreadSpecificStack(void);
void pushThreadSpecificStack(ExceptionContextBlock *cptr);
void popThreadSpecificStack(void);

/* Private function prototypes */

static void *threadProc(void *arg);
static void initThreadPackage(void);
static void createKeys(void);
static void popLockException(void);

/* Section 1 - Functions on threads */

Thread forkThread(proc fn, void *data) {
   Thread thread;
   ThreadDataBlock dp;
   pthread_attr_t attr;
   int osErr;

   initThreadPackage();
   thread = newBlock(Thread);
   thread->name = NULL;
   thread->terminated = false;
   thread->lock = newLock();
   dp = newBlock(ThreadDataBlock);
   dp->fn = fn;
   dp->data = data;
   dp->thread = thread;
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   osErr = pthread_create(&thread->id, &attr, threadProc, dp);
   pthread_attr_destroy(&attr);
   if (osErr != 0) error("forkThread: Can't create new thread");
   return thread;
}

void joinThread(Thread thread) {
   if (thread->lock == NULL) {
      error("joinThread: Can't join with the main thread");
   }
   synchronized (thread->lock) {
      while (!thread->terminated) {
         waitThread(thread->lock);
      }
   }
}

void yield(void) {
   pthread_yield();
}

Thread getCurrentThread(void) {
   Thread thread;

   initThreadPackage();
   thread = (Thread) pthread_getspecific(threadKey);
   if (thread == NULL) {
      thread = newBlock(Thread);
      thread->name = "Main";
      thread->id = 0;
      pthread_setspecific(threadKey, thread);
   }
   return thread;
}

void setThreadName(Thread thread, string name) {
   thread->name = name;
}

string getThreadName(Thread thread) {
   char buffer[MAX_AUTOMATIC_THREAD_NAME];

   if (thread == NULL) return "NULL";
   if (thread->name == NULL) {
      sprintf(buffer, "Thread<%lu>", (long) thread->id);
      thread->name = copyString(buffer);
   }
   return thread->name;
}

/* Section 2 - Functions on locks */

Lock newLock(void) {
   Lock lock;
   pthread_mutexattr_t attr;

   lock = newBlock(Lock);
   pthread_mutexattr_init(&attr);
   pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
   pthread_mutex_init(&lock->mutex, &attr);
   pthread_mutexattr_destroy(&attr);
   pthread_cond_init(&lock->condition, NULL);
   lock->owner = NULL;
   lock->exitFlag = false;
   lock->depth = 0;
   return lock;
}

void waitThread(Lock lock) {
   pthread_cond_wait(&lock->condition, &lock->mutex);
}

void signalThread(Lock lock) {
   pthread_cond_broadcast(&lock->condition);
}

void lockInternal(Lock lock) {
   pthread_mutex_lock(&lock->mutex);
   if (lock->depth++ == 0) lock->owner = getCurrentThread();
}

void unlockInternal(Lock lock) {
   if (--lock->depth == 0) lock->owner = NULL;
   pthread_mutex_unlock(&lock->mutex);
}

bool startSync(Lock lock) {
   ExceptionContextBlock *xcb;

   if (lock->exitFlag) {
      lock->exitFlag = false;
      popLockException();
      return false;
   }
   lockInternal(lock);
   xcb = newBlock(ExceptionContextBlock *);
   xcb->lock = lock;
   pushExceptionStack(xcb);
   return true;
}

void endSync(Lock lock) {
   lock->exitFlag = true;
}

Thread getOwner(Lock lock) {
   return lock->owner;
}

/* Section 3 -- Internal interface to the exception module */

ExceptionContextBlock *getThreadSpecificStack(void) {
   ExceptionContextBlock *stack;

   initThreadPackage();
   stack = (ExceptionContextBlock *) pthread_getspecific(exceptionStackKey);
   while (stack != NULL && stack->lock != NULL) {
      popLockException();
      stack = stack->link;
   }
   return stack;
}

void pushThreadSpecificStack(ExceptionContextBlock *cptr) {
   ExceptionContextBlock *stack;

   initThreadPackage();
   stack = (ExceptionContextBlock *) pthread_getspecific(exceptionStackKey);
   cptr->link = stack;
   pthread_setspecific(exceptionStackKey, cptr);
}

void popThreadSpecificStack(void) {
   ExceptionContextBlock *stack;

   stack = getThreadSpecificStack();
   if (stack != NULL) stack = stack->link;
   pthread_setspecific(exceptionStackKey, stack);
}

/* Private functions */

static void *threadProc(void *arg) {
   ThreadDataBlock dp;

   initThreadPackage();
   dp = (ThreadDataBlock) arg;
   pthread_setspecific(threadKey, dp->thread);
   dp->fn(dp->data);
   synchronized (dp->thread->lock) {
      dp->thread->terminated = true;
      signalThread(dp->thread->lock);
   }
   return NULL;
}

static void initThreadPackage(void) {
   pthread_once(&onceOnly, createKeys);
}

static void createKeys(void) {
   pthread_key_create(&threadKey, NULL);
   pthread_key_create(&exceptionStackKey, NULL);
}

static Thread findThread(pthread_t id) {
   Thread thread;

   initThreadPackage();
   thread = (Thread) pthread_getspecific(threadKey);
   if (thread == NULL) {
      thread = newBlock(Thread);
      thread->id = id;
      thread->name = NULL;
      pthread_setspecific(threadKey, thread);
   }
   return thread;
}

static void popLockException(void) {
   ExceptionContextBlock *stack;

   initThreadPackage();
   stack = (ExceptionContextBlock *) pthread_getspecific(exceptionStackKey);
   if (stack == NULL) error("popLockException: Internal error");
   pthread_setspecific(exceptionStackKey, stack->link);
   unlockInternal((Lock) stack->lock);
   freeBlock(stack);
}

#endif
