/*
 * File: threading.cpp
 * -------------------
 * This file contains implementation of code to run GoogleTest test cases
 * with a timeout, possibly in a separate thread depending on the platform.
 *
 * @author Marty Stepp
 * @version 2014/11/24
 * @since 2014/11/24
 */

#include "autograder.h"
#include "autogradertest.h"
#include "platform.h"

static const std::string TIMEOUT_ERROR_MESSAGE = "test timed out! possible infinite loop";

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#undef MOUSE_EVENT
#undef KEY_EVENT
#undef MOUSE_MOVED
#undef HELP_KEY
static DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
    autograder::AutograderTest* currentTest = (autograder::AutograderTest*) lpParam;
    currentTest->TestRealBody();
    return (DWORD) 0;
}

void runTestWithTimeout(autograder::AutograderTest* test) {
    DWORD threadID;
    HANDLE hThread = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size
        MyThreadFunction,       // thread function name
        (LPVOID) test,          // argument to pass to thread function
        0,                      // use default creation flags
        &threadID);             // returns the thread identifier
    if (!hThread) {
        error("Unable to run test case thread: " + getPlatform()->os_getLastError());
    }
    int timeoutMS = test->getTestTimeout();
    DWORD result = WaitForSingleObject(hThread, timeoutMS);
    if (result == WAIT_TIMEOUT) {
        TerminateThread(hThread, 1);
        autograder::setFailDetails(autograder::UnitTestDetails(
            autograder::UnitTestType::TEST_FAIL,
            TIMEOUT_ERROR_MESSAGE));
        error(TIMEOUT_ERROR_MESSAGE);
    }
}
#else // not _WIN32
static void alarmCancel();
static void alarmHandler(int /*sig*/);

void runTestWithTimeout(autograder::AutograderTest* test) {
    signal(SIGALRM, handleAlarm);
    int timeoutMS = test->getTestTimeout();
    alarm(timeoutMS / 1000);
    currentTest->TestRealBody();
    alarmCancel();
}

static void alarmCancel() {
    signal(SIGALRM, SIG_IGN);
    alarm(0);   // cancel alarm
}

static void alarmHandler(int /*sig*/) {
    alarmCancel();
    std::string msg = TIMEOUT_ERROR_MESSAGE;
    autograder::setFailDetails(autograder::UnitTestDetails(
        autograder::UnitTestType::TEST_FAIL,
        TIMEOUT_ERROR_MESSAGE));
    error(TIMEOUT_ERROR_MESSAGE);
}

#endif // _WIN32
