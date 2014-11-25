/*
 * File: autogradertest.h
 * ----------------------
 * This file contains the declaration of a class that extends GoogleTest's
 * Test class to add some functionality such as timeouts and a list of known
 * test names.
 * 
 * @author Marty Stepp
 * @version 2014/11/24
 * @since 2014/11/24
 */

#ifndef _autogradertest_h
#define _autogradertest_h

#include <string>
#include "gtest.h"
#include "map.h"
#include "set.h"
#include "timer.h"
#include "vector.h"

namespace autograder {

class AutograderTest : public testing::Test {
public:
    /*
     * Constants for timeouts in ms
     */
    static int TIMEOUT_MS_DEFAULT;
    static int TIMEOUT_MS_MIN;

    /*
     * A list of all known test case names, grouped by category.
     * Used to enable/disable categories to run.
     */
    static void addTestToList(const std::string& categoryName, const std::string& testName);
    static const Vector<std::string>& getAllCategories();
    static const Vector<std::string>& getAllTests(const std::string& categoryName = "");

    /*
     * get/set the default timeout values
     */
    static int getDefaultTimeout();
    static void setDefaultTimeout(int timeoutMS);

    virtual int getTestTimeout() const;
    virtual void setTestTimeout(int ms);
    virtual void SetUp();
    virtual void TearDown();
    virtual void TestBody();   // override me
    virtual void TestRealBody();

private:
    static Vector<std::string> allCategories;
    static Map<std::string, Vector<std::string> > allTests;

    int timeoutMS;   // test's timeout in milliseconds (0 for none)
};

} // namespace autograder

#endif // _autogradertest_h
