#include "gbrowserpane.h"
#include "gwindow.h"
#include "SimpleTest.h"
#include "TestDriver.h"
#include "TextUtils.h"
#include "MemoryDiagnostics.h"
#include "error.h"
#include "gwindow.h"
#include "simpio.h"
using namespace std;

namespace SimpleTest {
    /* Type representing how a test turned out. */
    enum class TestResult {
        WAITING,
        RUNNING,
        PASS,
        FAIL,
        LEAK,
        EXCEPTION
    };

    /* Type representing a single test and its result. */
    struct Test {
        TestCase tcase;

        TestResult result;
        string detailMessage;
    };

    /* Type representing a group of tests and whether group is selected to run. */
    struct TestGroup {
        string name;

        Vector<Test> tests;
        bool selected;
    };

    string id(const TestResult& tr)
    {
        switch (tr) {
            case TestResult::WAITING:   return "waiting";
            case TestResult::RUNNING:   return "running";
            case TestResult::PASS:      return "pass";
            case TestResult::FAIL:      return "fail";
            case TestResult::LEAK:      return "leak";
            case TestResult::EXCEPTION: return "exception";
            default: error("Invalid TestResult");
        }
    }

    string status(const TestResult& tr)
    {
        switch (tr) {
            case TestResult::WAITING:   return "&nbsp;";
            case TestResult::RUNNING:   return "Running...";
            case TestResult::PASS:      return "Correct";
            case TestResult::FAIL:      return "Incorrect";
            case TestResult::LEAK:      return "Leak";
            case TestResult::EXCEPTION: return "Exception";
            default: error("Invalid TestResult");
        }
    }

    /* Runs a single test. */
    void runSingleTest(Test& test) {
        try {
            /* Reset memory counters so we don't have carryover across tests. */
            MemoryDiagnostics::clear();

            /* Run the test. */
            test.tcase.callback();

            /* See if there were any memory leaks. */
            auto errors = MemoryDiagnostics::typesWithErrors();
            if (errors.empty()) {
                test.result = TestResult::PASS;
            } else {
                /* We have memory leaks. */
                test.result = TestResult::LEAK;

                ostringstream out;
                out << "    Test failed due to memory errors with these types:" << endl;
                for (const auto& entry: errors) {
                    string type = entry.first;
                    int    delta = entry.second;

                    if (delta > 0) {
                        out << "            " << type << ": Leaked " << pluralize(delta, "object") << "." << endl;
                    } else {
                        out << "            " << type << ": Deallocated " << pluralize(-delta, "more object") << " than allocated." << endl;
                    }
                }

                test.detailMessage = out.str();
            }
        } catch (const TestFailedException& e) {
            test.result = TestResult::FAIL;
            ostringstream out;
            out << "    " << e.what() << endl;
            test.detailMessage = out.str();
        } catch (const ErrorException& e) {
            test.result = TestResult::EXCEPTION;
            ostringstream out;
            out << "    Test failed due to the program triggering an ErrorException." << endl;
            out << endl;
            out << "    This means that the test did not fail because of a call" << endl;
            out << "    to EXPECT() or EXPECT_ERROR() failing, but rather because" << endl;
            out << "    some code explicitly called the error() function." << endl;
            out << endl;
            out << "    Error: " << e.getMessage();
            test.detailMessage = out.str();
        } catch (const exception& e) {
            test.result = TestResult::EXCEPTION;
            ostringstream out;
            out << "    Test failed due to the program triggering an exception." << endl;
            out << endl;
            out << "    This means that the test did not fail because of a call" << endl;
            out << "    to EXPECT() or an EXPECT_ERROR() failing, but rather because" << endl;
            out << "    some code - probably an internal C++ library - triggered" << endl;
            out << "    an error." << endl;
            out << endl;
            out << "    Error: " << e.what() << endl;
            test.detailMessage = out.str();
        } catch (...) {
            test.result = TestResult::EXCEPTION;
            ostringstream out;
            out << "    Test failed due to the program triggering an unknown type" << endl;
            out << "    of exception. " << endl;
            out << endl;
            out << "    This means that the test did not fail because of a call" << endl;
            out << "    to EXPECT() or an EXPECT_ERROR() failing, but rather because" << endl;
            out << "    some code triggered an error whose format we couldn't" << endl;
            out << "    recognize." << endl;
            out << endl;
            test.detailMessage = out.str();
        }
    }

    /* Displays all the results from the given test group. */
    void displayResults(GBrowserPane *bp, const string& stylesheet, const Vector<TestGroup>& testGroups)
    {
        std::stringstream h;

        h << "<html><style>" << stylesheet << "</style><body><ul>";
        for (const auto& group: testGroups) {
            if (!group.selected) continue;
            h << "<li class=" << quotedVersionOf("title") << "> Tests from " << group.name << "</li>" ;
            /* Display each test as list item */
            for (const auto& test: group.tests) {
                h << "<hr>" << endl;
                string li = "<li class=" + quotedVersionOf(id(test.result)) + ">";
                h << li << "<b>" << status(test.result) << "</b> (" << test.tcase.owner << ", line "<< test.tcase.line << ") " << test.tcase.name << "</li>";
                if (!test.detailMessage.empty()) {
                    h << li << "<pre>" << test.detailMessage << "</pre></li>";
                }
                h << endl;
            }
            h << "<hr>" << endl;
        }
        h << "</ul></body></html>";
        bp->setText(h.str());   // display in browser pane
    }

    /* Given raw data from the testing system, produce a visual TestGroup for that data. */
    TestGroup testsToGroup(const pair<const TestKey, multimap<int, TestCase>>& entry) {
        TestGroup result;
        result.name = entry.first;
        result.selected = false;

        for (const auto& rawTest: entry.second) {
            Test test;
            test.tcase = rawTest.second;
            test.result = TestResult::WAITING; // It hasn't run yet
            result.tests += test;
        }

        return result;
    }
  
    void runSelectedGroups(Vector<TestGroup>& groups)
    {
        GWindow* window = new GWindow(700, 600);
        GBrowserPane *bp = new GBrowserPane();
        window->setTitle("SimpleTest");
        window->setExitOnClose(true);
        window->add(bp);

        window->setVisible(true);
        window->requestFocus();
        // student can customize display by editing the stylesheet
        string path = "src/testing/styles.css";
        string stylesheet = fileExists(path) ? readEntireFile(path) : "";
        
        /* Show everything so there's some basic data available. */
        displayResults(bp, stylesheet, groups);

        /* Now, go run the tests. */
        for (auto& group: groups) {
            if (!group.selected) continue;
            cout << endl << "[SimpleTest] ---- Tests from " << group.name << " -----" << endl;
            for (auto& test: group.tests) {
                /* Make clear that we're running the test. */
                test.result = TestResult::RUNNING;
                displayResults(bp,stylesheet, groups);
                cout << "[SimpleTest] starting test (" << test.tcase.owner <<", line "<< test.tcase.line << ") " << left << setfill('.') << setw(25) << test.tcase.name.substr(0,25) << "... " << flush;
                runSingleTest(test);
                cout << " =  "<< status(test.result) << endl << test.detailMessage;
                displayResults(bp, stylesheet, groups);
            }
        }
    }

    int userChoiceFromMenu(Vector<string>& options)
    {
        cout << endl;
        cout << "Tests are grouped by filename." << endl;
        cout << "Select the test groups you wish to run:" << endl;
        cout << "----------------------------------------" << endl;
        int i = 0;
        for (const auto& entry: options) {
            cout << "  " << i++ << ") " << entry << endl;
        }
        int chosen = getIntegerBetween("Enter your selection:", 0, options.size()-1);
        if (chosen == 0) {
            cout << "No tests selected." << endl << endl;
        }
        return chosen;
    }

    int getChoice(Vector<TestGroup>& groups, Choice ch)
    {
        if (ch == NO_TESTS) {
            return 0;
        } else if (ch == ALL_TESTS ) {
            return groups.size()+1;
        } else {
            Vector<string> names = { "None", "All of the above" };
            for (const auto& entry: groups) {
                names.insert(names.size()-1, entry.name);
            }
            return userChoiceFromMenu(names);
        }
    }

    bool selectChosenGroup(Vector<TestGroup>& groups, Choice ch)
    {
        int chosen = getChoice(groups, ch);

        if (chosen == 0) {
            return false;
        } else {
            for (int i = 0; i < groups.size(); i++) {
                groups[i].selected = (chosen == i+1 || chosen == groups.size()+1);
            }
            return true;
        }
    }

    bool runSimpleTests(Choice ch)
    {
        Vector<TestGroup> testGroups;

        for (const auto& entry: gTestsMap()) {
            testGroups += testsToGroup(entry); // group is by filename
        }
        if (selectChosenGroup(testGroups, ch)) {
            runSelectedGroups(testGroups);
            return true;
        } else {
            return false;
        }
    }
}

bool runSimpleTests(Choice ch) {
    return SimpleTest::runSimpleTests(ch);
}

