/*
 * File: exception.c
 * Last modified on Sat Apr  5 17:00:43 2008 by eroberts
 * -----------------------------------------------------
 * This file implements the C exception handler.  Much of the
 * real work is done in the exception.h header file.
 */

#include <stdio.h>
#include <stdarg.h>
#include "exception.h"
#include "cslib.h"
#include "strlib.h"
#include "thread.h"
#include "unittest.h"

/* Constants */

#define MAX_UNHANDLED_MESSAGE 100
#define ERROR_EXIT_STATUS 1

/* Publically accessible exceptions */

exception ANY = { "ANY" };
exception ErrorException = { "ErrorException" };

/* Prototypes for friends functions */

void unhandledError(string msg);
void unhandledException(string name);

/* Prototypes for internal functions in threads module */

extern ExceptionContextBlock *getThreadSpecificStack(void);
extern void pushThreadSpecificStack(ExceptionContextBlock *cptr);
extern void popThreadSpecificStack(void);

/*
 * Triggers an exception by finding an appropriate handler and then
 * using <code>longjmp</code> to return to the context stored there
 * after resetting the exception stack.  If no handler exists, the
 * function calls <code>error</code> with an unhandled exception.
 */

void throwException(exception *e, string name, void *value) {
   ExceptionContextBlock *stack;

   stack = getThreadSpecificStack();
   if (stack == NULL) {
      if (e == &ErrorException) {
         unhandledError((string) value);
      } else {
         unhandledException(name);
      }
   }
   stack->id = e;
   stack->name = name;
   stack->value = value;
   longjmp(stack->jmp, 1);
}

/* Friends entries */

/*
 * Implementation notes: unhandledError
 * ------------------------------------
 * Called when an unhandled <code>ErrorException</code> occurs.
 */

void unhandledError(string msg) {
   fprintf(stderr, "error: %s\n", msg);
   exit(ERROR_EXIT_STATUS);
}

/*
 * Implementation notes: unhandledException
 * ----------------------------------------
 * Called when an unhandled exception occurs.
 */

void unhandledException(string name) {
   fprintf(stderr, "Unhandled exception: %s\n", name);
   exit(E_UNHANDLED_EXCEPTION);
}

/* Private functions */

/*
 * Pushes a new exception context on the exception stack.
 */

void pushExceptionStack(ExceptionContextBlock *cptr) {
   pushThreadSpecificStack(cptr);
}

/*
 * Discards the top exception context from the exception stack.
 */

void popExceptionStack(void) {
   popThreadSpecificStack();
}

/*
 * Reraises an exception in the top stack frame.
 */

void unwindException(ExceptionContextBlock *cptr) {
   ExceptionContextBlock *stack;

   stack = getThreadSpecificStack();
   if (stack == NULL) {
      if (cptr->id == &ErrorException) {
         unhandledError((string) cptr->value);
      } else {
         unhandledException(cptr->name);
      }
   }
   stack->id = cptr->id;
   stack->name = cptr->name;
   stack->value = cptr->value;
   longjmp(stack->jmp, 1);
}

/**********************************************************************/
/* Unit test for the exception module                                 */
/**********************************************************************/

#ifndef _NOTEST_

/* Exceptions */

static exception Ex1 = { "Ex1" };
static exception Ex2 = { "Ex2" };
static exception Ex3 = { "Ex3" };

/* Private function prototypes */

static void nestedThrow(string exceptionName);
static string testSimpleException(string exceptionName);
static string testExceptionWithFinallyClause(string exceptionName);

/* Unit test */

void testExceptionModule(void) {
   trace(testSimpleException(""));
   trace(testSimpleException("Ex1"));
   trace(testSimpleException("Ex2"));
   testError(testSimpleException("Ex3"));
   trace(testExceptionWithFinallyClause(""));
   trace(testExceptionWithFinallyClause("Ex1"));
   trace(testExceptionWithFinallyClause("Ex2"));
   testError(testSimpleException("Ex3"));
}

/* Private functions */

static void nestedThrow(string exceptionName) {
   if (stringEqual(exceptionName, "")) return;
   reportMessage("throw(%s);", exceptionName);
   if (stringEqual(exceptionName, "Ex1")) {
      throw(Ex1);
   } else if (stringEqual(exceptionName, "Ex2")) {
      throw(Ex2);
   } else if (stringEqual(exceptionName, "Ex3")) {
      throw(Ex3);
   } else {
      reportError("Illegal exception name: %s", exceptionName);
   }
}

static string testSimpleException(string exceptionName) {
   string events;

   adjustReportIndentation(+3);
   trace(events = "");
   reportMessage("try {");
   adjustReportIndentation(+3);
   try {
      nestedThrow(exceptionName);
      test(exceptionName, "");
   } catch (Ex1) {
      adjustReportIndentation(-3);
      reportMessage("} catch (Ex1) {");
      adjustReportIndentation(+3);
      trace(events = concat(events, "Ex1"));
   } catch (Ex2) {
      adjustReportIndentation(-3);
      reportMessage("} catch (Ex2) {");
      adjustReportIndentation(+3);
      trace(events = concat(events, "Ex2"));
   } endtry
   adjustReportIndentation(-3);
   reportMessage("}");
   test(events, (string) exceptionName);
   adjustReportIndentation(-3);
   return events;
}

static string testExceptionWithFinallyClause(string exceptionName) {
   string events;

   adjustReportIndentation(+3);
   trace(events = "");
   reportMessage("try {");
   adjustReportIndentation(+3);
   try {
      nestedThrow(exceptionName);
      test(exceptionName, "");
   } catch (Ex1) {
      adjustReportIndentation(-3);
      reportMessage("} catch (Ex1) {");
      adjustReportIndentation(+3);
      trace(events = concat(events, "Ex1"));
   } catch (Ex2) {
      adjustReportIndentation(-3);
      reportMessage("} catch (Ex2) {");
      adjustReportIndentation(+3);
      trace(events = concat(events, "Ex2"));
   } finally {
      adjustReportIndentation(-3);
      reportMessage("finally");
      adjustReportIndentation(+3);
      trace(events = concat(events, "+finally"));
   } endtry
   adjustReportIndentation(-3);
   reportMessage("}");
   test(events, (string) concat(exceptionName, "+finally"));
   adjustReportIndentation(-3);
   return events;
}

#endif
