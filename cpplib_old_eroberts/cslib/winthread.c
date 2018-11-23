/*
 * File: winthread.c
 * Last modified on Mon Oct 20 17:56:40 2008 by eroberts
 * -----------------------------------------------------
 * This file is the top-level file in the implementation of the
 * thread.h interface using WIN32.
 */

#ifdef __WIN32__

#include <stdio.h>
#include <windows.h>
#include "exception.h"
#include "cslib.h"
#include "strlib.h"
#include "thread.h"

/* Constants */

#define MAX_AUTOMATIC_THREAD_NAME 30

/* Define the concrete type for a thread */

struct ThreadCDT {
   DWORD id;
   string name;
   bool terminated;
   Lock lock;
};

/* Define the concrete type for a lock */

struct LockCDT {
   HANDLE mutex;
   HANDLE semaphore;
   HANDLE event;
   bool exitFlag;
   int waitingThreads;
   int depth;
   Thread owner;
};

/* Define a data block as a wrapper for the thread data */

typedef struct {
   proc fn;
   void *data;
   Thread thread;
} *ThreadDataBlock;

/* Private variables */

static bool initialized = false;
static DWORD threadIndex;
static DWORD cbIndex;

/* Private function prototypes */

static void initThreadPackage(void);
static DWORD threadProc(LPVOID arg);
static void popLockException(void);

/* Section 1 - Functions on threads */

Thread forkThread(proc fn, void *data) {
   Thread thread;
   ThreadDataBlock data;
   HANDLE threadHandle;

   if (!initialized) initThreadPackage();
   thread = newBlock(Thread);
   thread->name = NULL;
   thread->terminated = false;
   thread->lock = newLock();
   data = newBlock(ThreadDataBlock);
   data->fn = fn;
   data->data = data;
   data->thread = thread;
   threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) threadProc,
                               data, 0, &thread->id);
   if (threadHandle == 0) error("forkThread: Can't create new thread");
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
   switchToThread();
}

Thread GetCurrentThread(void) {
   Thread thread;

   if (!initialized) initThreadPackage();
   thread = (Thread) TlsGetValue(threadIndex);
   if (thread == NULL) {
      thread = newBlock(Thread);
      thread->name = "Main";
      thread->id = 0;
      TlsSetValue(threadIndex, thread);
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

   lock = newBlock(Lock);
   lock->mutex = CreateMutex(NULL, false, NULL);
   lock->semaphore = CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
   lock->event = CreateEvent(NULL, false, false, NULL);
   lock->exitFlag = false;
   lock->waitingThreads = 0;
   lock->depth = 0;
   return lock;
}

void waitThread(Lock lock) {
   lock->waitingThreads++;
   signalObjectAndWait(lock->mutex, lock->semaphore, INFINITE, false);
   lock->waitingThreads--;
   if (lock->waitingThreads == 0) {
      signalObjectAndWait(lock->event, lock->mutex, INFINITE, false);
   } else {
      waitForSingleObject(lock->mutex, INFINITE);
   }
}

void signalThread(Lock lock) {
   if (lock->waitingThreads > 0) {
      ReleaseMutex(lock->mutex);
      ReleaseSemaphore(lock->semaphore, lock->waitingThreads, 0);
      waitForSingleObject(lock->event, INFINITE);
      waitForSingleObject(lock->mutex, INFINITE);
   }
}

void lockInternal(Lock lock) {
   waitForSingleObject(lock->mutex, INFINITE);
   if (lock->depth++ == 0) lock->owner = GetCurrentThread();
}

void unlockInternal(Lock lock) {
   if (--lock->depth == 0) lock->owner = NULL;
   ReleaseMutex(lock->mutex);
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

void XEndSync(Lock lock) {
   lock->exitFlag = true;
}

Thread getOwner(Lock lock) {
   return lock->owner;
}

/* Section 3 -- Internal interface to the exception module */

ExceptionContextBlock *XGetThreadSpecificStack(void) {
   ExceptionContextBlock *stack;

   if (!initialized) initThreadPackage();
   stack = (ExceptionContextBlock *) TlsGetValue(cbIndex);
   while (stack != NULL && stack->lock != NULL) {
      popLockException();
      stack = stack->link;
   }
   return stack;
}

void pushThreadSpecificStack(ExceptionContextBlock *cptr) {
   ExceptionContextBlock *stack;

   if (!initialized) initThreadPackage();
   stack = (ExceptionContextBlock *) TlsGetValue(cbIndex);
   cptr->link = stack;
   TlsSetValue(cbIndex, cptr);
}

void popThreadSpecificStack(void) {
   ExceptionContextBlock *stack;

   if (!initialized) initThreadPackage();
   stack = XGetThreadSpecificStack();
   if (stack != NULL) stack = stack->link;
   TlsSetValue(cbIndex, stack);
}

/* Private functions */

static void initThreadPackage(void) {
   threadIndex = TlsAlloc();
   cbIndex = TlsAlloc();
   if (threadIndex == TLS_OUT_OF_INDEXES || cbIndex == TLS_OUT_OF_INDEXES) {
      error("initThreadPackage: Out of thread storage");
   }
   initialized = true;
}

static DWORD threadProc(LPVOID arg) {
   ThreadDataBlock data;

   data = (ThreadDataBlock) arg;
   if (!TlsSetValue(threadIndex, data->thread)) {
      error("forkThread: Can't store thread identifier");
   }
   if (!TlsSetValue(cbIndex, NULL)) {
      error("forkThread: Can't initialize exception handler chain");
   }
   data->fn(data->data);
   synchronized (data->thread->lock) {
      data->thread->terminated = true;
      signalThread(data->thread->lock);
   }
   return 0;
}

static void popLockException(void) {
   ExceptionContextBlock *stack;

   if (!initialized) initThreadPackage();
   stack = (ExceptionContextBlock *) TlsGetValue(cbIndex);
   if (stack == NULL) error("popLockException: Internal error");
   TlsSetValue(cbIndex, stack->link);
   unlockInternal((Lock) stack->lock);
   freeBlock(stack);
}

#endif
