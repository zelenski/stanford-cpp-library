/*
 * File: unittest.h
 * ----------------
 * This interface exports several functions that facilitate writing unit
 * tests for packages.
 */

#ifndef _unittest_h
#define _unittest_h

#include <stdarg.h>
#include "cslib.h"
#include "exception.h"

extern void uTTest(string exp, string value, ...);

/*
 * Function: trace
 * Usage: trace(exp);
 * ------------------
 * Tests to make sure that a procedure does not generate any calls
 * to <code>error</code>.
 */

#define trace(exp) { \
   try { \
      reportMessage("%s;", #exp); \
      exp; \
   } catch (ErrorException) { \
      reportError("%s throws \"%s\"", #exp, getExceptionValue()); \
   } endtry \
}

/*
 * Function: test
 * Usage: test(exp, value);
 * ------------------------
 * Tests to make sure that a function computes a specified value.  The
 * first argument is an expression (typically a function call in the
 * module under test), and the second argument is the expected value,
 * which must be of one of the following types:
 *
 * <p><ul>
 * <li><code>bool</code>
 * <li><code>char</code>
 * <li><code>double</code>
 * <li><code>int</code>
 * <li><code>string</code>
 * </ul>
 */

#define test(exp, value) { \
   try { \
      uTTest(#exp, #value, exp, value); \
   } catch (ErrorException) { \
      reportError("%s throws \"%s\"", #exp, getExceptionValue()); \
   } endtry \
}

/*
 * Function: testError
 * Usage: testError(exp);
 * ----------------------
 * Tests to make sure that a method call generates an error.
 */

#define testError(exp) { \
   int indent = getReportIndentation(); \
   try { \
      exp; \
      setReportIndentation(indent); \
      reportError("%s should generate an error", #exp); \
   } catch (ErrorException) { \
      setReportIndentation(indent); \
      reportMessage("%s throws \"%s\"", #exp, getExceptionValue()); \
   } catch (ANY) { \
      setReportIndentation(indent); \
      reportMessage("%s throws \"%s\"", #exp, getExceptionName()); \
   } endtry \
}

/*
 * Function: testModule
 * Usage: errorCount = testModule(name, fn);
 * -----------------------------------------
 * Runs the specified test method and generates a report.  The
 * second parameter must be the name of a test function inside
 * the named package.
 */

int testModule(string name, proc fn);

/*
 * Function: setVerboseTestingFlag
 * Usage: setVerboseTestingFlag(flag);
 * -----------------------------------
 * Sets a global flag that determines whether the test process
 * traces all the test operations.
 */

void setVerboseTestingFlag(bool flag);

/*
 * Function: getVerboseTestingFlag
 * Usage: flag = getVerboseTestingFlag();
 * --------------------------------------
 * Returns the setting of the verbose testing flag.
 */

bool getVerboseTestingFlag(void);

/*
 * Function: reportError
 * Usage: reportError(msg, ...);
 * -----------------------------
 * Reports an error in the current package.  If <code>msg</code> is
 * <code>NULL</code>, the error counter is incremented, but no
 * message is generated.
 */

void reportError(string msg, ...);

/*
 * Function: reportMessage
 * Usage: reportMessage(msg, ...);
 * -------------------------------
 * Generates a line of output if verbose tracing in enabled.
 */

void reportMessage(string msg, ...);

/*
 * Function: setReportIndentation
 * Usage: setReportIndentation(indent);
 * ------------------------------------
 * Sets the indentation in the <code>reportError</code> and
 * <code>reportMessage</code>, which allows test script designers
 * to improve readability of the verbose output.  In most cases,
 * clients will not use <code>setReportIndentation</code> directly
 * but will instead rely on <code>adjustReportIndentation</code>.
 * This call, however, is necessary for tests that throw errors
 * or other exceptions.
 */

void setReportIndentation(int indent);

/*
 * Function: getReportIndentation
 * Usage: indent = getReportIndentation();
 * ---------------------------------------
 * Gets the current indentation used in the <code>reportError</code>
 * and <code>reportMessage</code> calls.  In most cases, clients
 * will not use <code>getReportIndentation</code> directly but will
 * instead rely on <code>adjustReportIndentation</code>.
 */

int getReportIndentation(void);

/*
 * Function: adjustReportIndentation
 * Usage: adjustReportIndentation(delta);
 * --------------------------------------
 * Adjusts the indentation in the <code>reportError</code> and
 * <code>reportMessage</code> calls to improve readability of
 * the verbose output.
 */

void adjustReportIndentation(int delta);

#endif
