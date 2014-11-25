/*
 * File: threading.h
 * -----------------
 * This file contains declarations of code to run GoogleTest test cases
 * with a timeout, possibly in a separate thread depending on the platform.
 *
 * @author Marty Stepp
 * @version 2014/11/24
 * @since 2014/11/24
 */

#ifndef _threading_h
#define _threading_h

#include <string>
#include "gtest.h"
#include "map.h"
#include "set.h"
#include "timer.h"
#include "vector.h"

/*
 * Runs the given unit test but halts it with an error if its runtime
 * exceeds the amount of ms given by getTestTimeout on the given test.
 * On some platforms this is implemented using a thread; on others,
 * using a signal.
 * On Linux/Mac, the granularity of timeouts is only to the nearest second.
 */
void runTestWithTimeout(autograder::AutograderTest* test);

#endif // _threading_h
