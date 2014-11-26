/*
 * File: gtest-marty.h
 * -------------------
 * This file contains declarations of utility code to augment the functionality
 * of the Google Test C++ unit testing framework.
 * 
 * @author Marty Stepp
 * @version 2014/11/24
 * - heavy refactor; moved code out to autogradertest, testresultprinter, threading, unittestdetails
 * @version 2014/10/31
 * - added code for graphical test runner UI
 * @version 2014/10/14
 * - moved some code out to diff.h and stylecheck.h
 */

#ifndef _gtest_marty_h
#define _gtest_marty_h

#include <string>
#include "assertions.h"
#include "autogradertest.h"
#include "gtest.h"
#include "threading.h"

/*
 * A big nasty macro that defines a subclass of AutograderTest to
 * represent a single unit test.
 */
#define GTEST_TEST_TIMED(test_case_name, test_name, parent_class, timeoutMS, parent_id) \
    class GTEST_TEST_CLASS_NAME_(test_case_name, test_name) : public parent_class { \
    public: \
        GTEST_TEST_CLASS_NAME_(test_case_name, test_name)() { \
            this->name = #test_name; \
            this->category = #test_case_name; \
        } \
    private: \
        virtual void TestRealBody(); \
        virtual void TestBody(); \
        static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_; \
        GTEST_DISALLOW_COPY_AND_ASSIGN_(GTEST_TEST_CLASS_NAME_(test_case_name, test_name)); \
    }; \
\
    ::testing::TestInfo* const GTEST_TEST_CLASS_NAME_(test_case_name, test_name) \
      ::test_info_ = \
        ::testing::internal::MakeAndRegisterTestInfo( \
            #test_case_name, #test_name, NULL, NULL, \
            (parent_id), \
            parent_class::SetUpTestCase, \
            parent_class::TearDownTestCase, \
            new ::testing::internal::TestFactoryImpl<GTEST_TEST_CLASS_NAME_(test_case_name, test_name)>); \
    void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBody() { \
        autograder::setCurrentTestShouldRun(shouldRun()); \
        if (!shouldRun()) { \
            return; \
        } \
        if (timeoutMS > 0) { \
            setTestTimeout(timeoutMS); \
            runTestWithTimeout(this); \
        } else { \
            TestRealBody(); \
        } \
    } \
    void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestRealBody()

#define TIMED_TEST(test_fixture, test_name, timeoutMS)\
  GTEST_TEST_TIMED(test_fixture, test_name, test_fixture, timeoutMS, \
              ::testing::internal::GetTypeId<test_fixture>())


#define TIMED_TEST_F(test_fixture, test_name)\
  GTEST_TEST_TIMED(test_fixture, test_name, test_fixture, test_fixture::TIMEOUT_MS_DEFAULT, \
              ::testing::internal::GetTypeId<test_fixture>())

// semicolon omitted below so that the client shall include it
// (looks better that way in client code)
#define TEST_CATEGORY(className, text) \
    class className : public autograder::AutograderTest { \
    public: \
        static void SetUpTestCase() { \
            autograder::setCurrentCategoryName((#text)); \
        } \
    } \

#endif // _gtest_marty_h
