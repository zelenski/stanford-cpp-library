/*
 * File: thread.c
 * Last modified on Fri Sep 26 09:34:55 2014 by eroberts
 * -----------------------------------------------------
 * This file contains only the unit test for the thread module.  The
 * actual implementation is provided by the following source files:
 *
 *   MacOSX and Unix:  posixthread.c
 *   Windows:          winthread.c
 *
 * Only one of those files will generate code, so it is safe to
 * include all of them in the compilation.
 */

#include <stdio.h>
#include "cslib.h"
#include "exception.h"
#include "generic.h"
#include "strlib.h"
#include "thread.h"
#include "unittest.h"

/**********************************************************************/
/* Unit test for the thread module                                    */
/**********************************************************************/

#ifndef _NOTEST_

/* Constants */

#define MAX_QUEUE 5

typedef struct {
   string name;
   int count;
} *Counter;

typedef struct {
   Lock lock;
   int count;
   char buffer[MAX_QUEUE];
} *CharacterQueue;

/* Exceptions */

static exception Ex1 = { "Ex1" };

/* Import friends functions */

extern Thread getOwner(Lock lock);

/* Private function prototypes */

static Counter newCounter(string name);
static void counterThread(void *data);
static void testCounterThreads(void);
static void testSynchronized(void);
static void testThrowThroughLock(void);
static CharacterQueue newCharacterQueue(void);
static bool isQueueEmpty(CharacterQueue queue);
static bool isQueueFull(CharacterQueue queue);
static void enqueueChar(CharacterQueue queue, char ch);
static char dequeueChar(CharacterQueue queue);
static void producerThread(void *data);
static void testProducerConsumer(void);

/* Unit test */

void testThreadModule(void) {
   testCounterThreads();
   testSynchronized();
   testThrowThroughLock();
   testProducerConsumer();
}

/* Section 1 -- Test basic concurrency mechanism */

static Counter newCounter(string name) {
   Counter counter;

   counter = newBlock(Counter);
   counter->name = name;
   counter->count = 0;
   return counter;
}

static void counterThread(void *data) {
   Counter counter;
   int i;

   counter = (Counter) data;
   for (i = 0; i < 10; i++) {
      if (counter->count != i) {
         reportError("counterThread sequence error");
      }
      counter->count++;
      yield();
   }
}

static void testCounterThreads(void) {
   Thread t1, t2, t3;

   reportMessage("testCounterThreads() {");
   adjustReportIndentation(+3);
   trace(t1 = forkThread(counterThread, newCounter("T1")));
   trace(t2 = forkThread(counterThread, newCounter("T2")));
   trace(t3 = forkThread(counterThread, newCounter("T3")));
   trace(joinThread(t1));
   trace(joinThread(t2));
   trace(joinThread(t3));
   adjustReportIndentation(-3);
   reportMessage("}");
}

/* Section 2 -- Test the synchronized statement */

static void testSynchronized(void) {
   Lock lock;
   Thread mainThread;

   reportMessage("testSynchronized() {");
   adjustReportIndentation(+3);
   trace(lock = newLock());
   trace(mainThread = getCurrentThread());
   reportMessage("synchronized(lock) {");
   synchronized (lock) {
      adjustReportIndentation(+3);
      test(getOwner(lock) == mainThread, true);
      adjustReportIndentation(-3);
   }
   reportMessage("}");
   test(getOwner(lock) == NULL, true);
   adjustReportIndentation(-3);
   reportMessage("}");
}

/*
 * Function: testThrowThroughLock
 * Usage: testThrowThroughLock();
 * ------------------------------
 * Tests to make sure that the <code>synchronized</code> statement
 * unlocks its locks if an exception transfers control out of the
 * critical section.
 */

static void testThrowThroughLock(void) {
   Lock lock;
   Thread mainThread;

   reportMessage("testThrowThroughLock() {");
   adjustReportIndentation(+3);
   trace(lock = newLock());
   trace(mainThread = getCurrentThread());
   reportMessage("try {");
   adjustReportIndentation(+3);
   try {
      reportMessage("synchronized(lock) {");
      adjustReportIndentation(+3);
      synchronized (lock) {
         test(getOwner(lock) == mainThread, true);
         reportMessage("throw(Ex1);");
         throw(Ex1);
         reportError("Should not get here");
      }
      reportError("Should not get here");
   } catch (Ex1) {
      test(getOwner(lock) == NULL, true);
   } finally {
      adjustReportIndentation(-3);
      reportMessage("}");
   } endtry;
   adjustReportIndentation(-3);
   reportMessage("}");
   adjustReportIndentation(-3);
}

/* Section 3 -- Test the waitThread/signalThread methods */

static CharacterQueue newCharacterQueue(void) {
   CharacterQueue queue;

   queue = newBlock(CharacterQueue);
   queue->lock = newLock();
   queue->count = 0;
   return queue;
}

static bool isQueueEmpty(CharacterQueue queue) {
   return queue->count == 0;
}

static bool isQueueFull(CharacterQueue queue) {
   return queue->count == MAX_QUEUE;
}

static void enqueueChar(CharacterQueue queue, char ch) {
   synchronized (queue->lock) {
      while (isQueueFull(queue)) {
         waitThread(queue->lock);
      }
      queue->buffer[queue->count++] = ch;
      signalThread(queue->lock);
   }
}

static char dequeueChar(CharacterQueue queue) {
   char ch;
   int i;

   synchronized (queue->lock) {
      while (isQueueEmpty(queue)) {
         waitThread(queue->lock);
      }
      ch = queue->buffer[0];
      queue->count--;
      for (i = 0; i < queue->count; i++) {
         queue->buffer[i] = queue->buffer[i + 1];
      }
      signalThread(queue->lock);
   }
   return ch;
}

static void producerThread(void *data) {
   CharacterQueue queue;
   char ch;

   queue = (CharacterQueue) data;
   for (ch = 'A'; ch <= 'Z'; ch++) {
      enqueueChar(queue, ch);
   }
}

static void testProducerConsumer(void) {
   CharacterQueue queue;
   Thread producer;
   string result;
   char ch;

   reportMessage("testProducerConsumer");
   queue = newCharacterQueue();
   result = "";
   producer = forkThread(producerThread, queue);
   while (true) {
      ch = dequeueChar(queue);
      result = concat(result, charToString(ch));
      if (ch == 'Z') break;
   }
   joinThread(producer);
   test(result, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

#endif
