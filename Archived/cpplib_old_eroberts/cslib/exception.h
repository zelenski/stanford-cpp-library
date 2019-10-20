/*
 * File: exception.h
 * -----------------
 * This interface exports a portable exception-handling mechanism for C.
 *
 * <p>The <code>exception.h</code> interface makes it possible for
 * clients to specify a handler for an exceptional conditions in a
 * syntactically readable way.  As a client, your first step
 * is to declare an exception condition name by declaring
 * a variable of type exception, as in
 *
 *<pre>
 *    exception MyException;
 *</pre>
 *
 * <p>Normal visibility rules apply, so that you should declare the
 * exception variable at the appropriate level.  For example, if an
 * exception is local to an implementation, it should be declared
 * statically within that module.  If an exception condition is
 * shared by many modules, the exception variable should be declared
 * in an interface and exported to all clients that need it.  This
 * package defines and exports the exception <code>ErrorException</code>,
 * which is likely to be sufficient for many clients.
 *
 * <p>The basic functionality of exceptions is that one piece of code
 * can "throw" an exception so that it can then be "caught" by
 * special code in a dynamically enclosing section of the program.
 * Exceptions are triggered by calling the pseudo-function
 * <code>throw</code> with the exception name, as in
 *
 *<pre>
 *    throw(MyException);
 *</pre>
 *
 * <p>Exceptions are handled using the <code>try</code> statement
 * (actually implemented using macros), which has the form:
 *
 *<pre>
 *    try {
 *       . . . statements in the body of the block . . .
 *    } catch (exception1) {
 *       . . . statements to handle exception 1 . . .
 *    } catch (exception2) {
 *       . . . statements to handle exception 2 . . .
 *    } catch (ANY) {
 *       . . . statements to handle any exception . . .
 *    } finally {
 *       . . . statements to be executed before exit in all cases . . .
 *    } endtry
 *</pre>
 *
 * <p>Any number of <code>catch</code> clauses may appear.  The
 * <code>ANY</code> and <code>finally</code> clauses are optional.
 *
 * <p>When the program encounters the <code>try</code> statement, the
 * statements in the body are executed.  If no exception conditions
 * are thrown during that execution, either in this block or by a
 * function call nested inside this block, control passes to the end
 * of the <code>try</code> statement when the last statement in the
 * block is executed.  If an exception is thrown during the dynamic
 * execution of the block, control immediately passes to the statements
 * in the appropriate <code>catch</code> clause.  Only the statements
 * in that clause are executed; no <code>break</code> statement is
 * required to exit the block.
 *
 * <p>The <code>try</code> statement guarantees that the statements in
 * the <code>finally</code> clause, if present, will always be executed,
 * even if an exception is thrown that is caught at a higher level.
 *
 * <p>If no handler for the exception appears anywhere in the
 * control history, the program exits with an error.
 *
 * <p>Examples of use:
 *
 * <p>1.  Catching errors
 *
 * <p>The following code fragment traps calls to <code>error</code>, so
 * that the program does not quit but instead returns to the top-level
 * read-and-execute loop.
 *
 *<pre>
 *    while (true) {
 *        try {
 *            printf("> ");
 *            cmd = readCommand();
 *            executeCommand(cmd);
 *        } catch (ErrorException) {
 *            printf("error: %s\n", (string) getExceptionValue());
 *            -- additional handling code, if any --
 *        } endtry
 *    }
 *</pre>
 *
 * <p>If either <code>readCommand</code> or <code>executeCommand</code>
 * calls <code>error</code>, control will be passed back to the main
 * loop, after executing any additional handler code.  The error
 * message is passed as the exception value and can be printed as
 * shown in the example.
 *
 * <p>2.  Handling control-C (Unix systems)
 *
 * <p>The following code extends the example above so that typing
 * <code>^C</code> also returns to top-level:
 *
 *<pre>
 *    #include <signal.h>
 * &nbsp;
 *    static exception ControlCException;
 *    static int errorCount = 0;
 *    static int controlCHandler();
 * &nbsp;
 *    main() {
 *        string cmd;
 * &nbsp;
 *        signal(SIGINT, controlCHandler);
 *        while (true) {
 *            try {
 *                printf("> ");
 *                cmd = readCommand();
 *                executeCommand(cmd);
 *            } catch (ControlCException) {
 *                printf("^C\n");
 *                signal(SIGINT, controlCHandler);
 *            } catch (ErrorException) {
 *                errorCount++;
 *            } endtry
 *        }
 *    }
 * &nbsp;
 *    static int controlCHandler() {
 *        throw(ControlCException);
 *    }
 *</pre>
 */

#ifndef _exception_h
#define _exception_h

#include <setjmp.h>
#include <string.h>
#include "cslib.h"

/*
 * Implementation notes
 * --------------------
 * Most of the implementation of the exception mechanism is included
 * directly within the macros defined by this file.  Clients should
 * ordinarily be able to read the general package description and
 * ignore the details of the code.
 */

/* Define error status indicators */

#define E_TOO_MANY_EXCEPT_CLAUSES 101
#define E_UNHANDLED_EXCEPTION 102

/* Codes to keep track of the state of the try handler */

#define ES_BODY       1
#define ES_CATCH      2
#define ES_CAUGHT     3
#define ES_RETRY      4
#define ES_FINALLY    5
#define ES_FINISHED   6

/*
 * The following conditional macro definition is used to debug
 * the <code>try</code> macro.  If __DebugTry__ is set, the
 * program traces the finite state machine operation used
 * to implement exceptions.
 */

#ifdef __DebugTry__
#   define debugTry(s) printf(s "\n", _es)
#else
#   define debugTry(s)
#endif

/*
 * Type: exception
 * ---------------
 * This type is used to define the general class of exceptions.
 * Exceptions are specified by their address, so that the
 * actual structure does not matter.  Strings are used here
 * so that exporters of exceptions can store the exception
 * name for the use of debuggers and other tools.
 */

typedef struct { string name; } exception;

/*
 * This structure is used internally to maintain a chain of
 * exception scopes on the control stack.
 */

typedef struct exceptionContextBlock {
    jmp_buf jmp;
    exception *id;
    void *value;
    string name;
    void *lock;
    struct exceptionContextBlock *link;
} ExceptionContextBlock;

/*
 * Type: ErrorException
 * --------------------
 * Predefined exception type for the <code>error</code> function.
 */
extern exception ErrorException;

/*
 * Constant: ANY
 * -------------
 * Predefined exception type that allows handlers to catch an arbitrary
 * exception.
 */
extern exception ANY;

/* Macros for the pseudostatement forms */

#define throw(e) throwException(&e, #e, NULL)

#define try \
      { \
          ExceptionContextBlock _ctx; \
          int _es; \
          _es = ES_BODY; \
          _ctx.lock = NULL; \
          debugTry("try"); \
          debugTry("push exception stack [es = %d]"); \
          pushExceptionStack(&_ctx); \
          if (setjmp(_ctx.jmp) != 0) { \
              _es = ES_CATCH; \
          } \
          debugTry("setjmp [es = %d]"); \
          if (_es == ES_BODY)

#define catch(e) \
          if (_es == ES_BODY || _es == ES_CAUGHT) _es = ES_FINALLY; \
          debugTry("catch (" #e ") [es = %d]"); \
          if (_es == ES_CATCH && (_ctx.id == &e || &e == &ANY)) { \
              _es = ES_CAUGHT; \
          } if (_es == ES_CAUGHT)

#define finally \
          debugTry("finally [es = %d]"); \
          debugTry("pop exception stack [es = %d]"); \
          popExceptionStack(); \
          _es = (_es == ES_CATCH) ? ES_RETRY : ES_FINISHED;

#define endtry \
          debugTry("endtry [es = %d]"); \
          if (_es != ES_FINISHED && _es != ES_RETRY) { \
              debugTry("pop exception stack [es = %d]"); \
              popExceptionStack(); \
          } \
          if (_es == ES_RETRY) _es = ES_CATCH; \
          if (_es == ES_CATCH) unwindException(&_ctx); \
      }

#define getExceptionName() _ctx.name
#define getExceptionValue() _ctx.value
#define getCurrentException() _ctx.id

/* Internal entry points -- see implementation for details */

void throwException(exception *e, string name, void *value);
void pushExceptionStack(ExceptionContextBlock *cptr);
void popExceptionStack(void);
void unwindException(ExceptionContextBlock *cptr);

#endif
