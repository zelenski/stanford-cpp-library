/**
 * TestingGUI.cpp
 *
 * @author Keith Schwarz
 * @version 2020/3/5
 *    Keith final revision from end of quarter 19-2
 * @version 2021 Fall Quarter
 *    Julie edits
 */
#include "console.h"
#include "error.h"
#include "filelib.h"
#include "gbrowserpane.h"
#include "gthread.h"
#include "gwindow.h"
#include <iomanip>
#include <map>
#include "MemoryDiagnostics.h"
#include <QCoreApplication> // for application name
#include <QLoggingCategory> // to control log messages
#include <QScrollBar>
#include "simpio.h"
#include "SimpleTest.h"
using namespace std;

static Vector<string> gDetails;

void addDetail(const string& msg)
{
    gDetails.add(msg);
}

// hand prototype to avoid having map in exposed header (and leading students astray)
std::map<TestKey, std::multimap<int, TestCase>>& gTestsMap();

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
        string testname;
        string id; // string identifying owner, module, line number
        std::function<void()> callback;
        TestResult result;
        string detailMessage;
    };

    /* Type representing a group of tests and whether group is selected to run. */
    struct TestGroup {
        string name;
        Vector<Test> tests;
        bool selected;
    };

    static const string NORMAL="\033[0m", RED="\033[31m", BLUE="\033[34m",
           GREEN="\033[32m", YELLOW="\033[33m", BOLD="\033[1m", FAINT="\033[2m";

    struct status_info {
        string status, id, color;
    };

    static Map<TestResult, status_info> info = {
        {TestResult::WAITING,   {"&nbsp;", "waiting", FAINT} },
        {TestResult::RUNNING,   {"Running...", "running", BLUE} },
        {TestResult::PASS,      {"Correct", "pass", GREEN + BOLD} },
        {TestResult::FAIL,      {"Incorrect", "fail",  RED + BOLD} },
        {TestResult::LEAK,      {"Leak", "leak", YELLOW + BOLD} },
        {TestResult::EXCEPTION, {"Exception", "exception", RED + BOLD} }
    };

    string affirmation()
    {
        Vector<string> choices = {
            "Excellent!", "Good job!", "Good on ya!", "Sweet!", "Bien hecho!",
            "Way to go!", "Super!", "Awesome!", "Great!", "Perfect!", "Nice work!",
            "You rock!", "Love it!", "Right on!", "No complaints here!", "Que bien!",
            "Buen trabajo!", "Bravo!", "Fantastico!", "Perfecto!", "Lo hiciste!",
            "Muy bien!", "Jolly good!", "Congratulations!", "Nice going!",
            "Terrific!", "Keep it up!", "You did it!", "A+", "Take a bow!"
        };
        return randomElement(choices);
    }

    string sanitize(string s)
    {
        for (size_t i = 0; i < s.length(); i++) {
            if (s[i] == '<') s.replace(i, 1, "&lt;");
            if (s[i] == '>') s.replace(i, 1, "&gt;");
        }
        return s;
    }

    /* Runs a single test. */
    void runSingleTest(Test& test) {
        try {
            /* Reset memory counters so we don't have carryover across tests. */
            MemoryDiagnostics::clear();
            gDetails.clear();

            /* Run the test. */
            test.callback();

            /* grab any details accumulated during run */
            test.detailMessage = stringJoin(gDetails, "\n"); // will be overwritten in case of actual failure

            /* See if there were any memory leaks. */
            auto errors = MemoryDiagnostics::typesWithErrors();
            if (errors.empty()) {
                test.result = TestResult::PASS;
            } else {
                /* We have memory leaks. */
                test.result = TestResult::LEAK;

                ostringstream out;
                out << endl << "Test had allocation count mismatch with these types:" << endl;
                for (const auto& entry: errors) {
                    string type = entry.first;
                    int delta = entry.second;

                    if (delta > 0) {
                        out << "    " << type << ": Leaked " << pluralize(delta, "object") << "." << endl;
                    } else {
                        out << "    " << type << ": Deallocated " << pluralize(-delta, "more object") << " than allocated." << endl;
                    }
                }
                test.detailMessage += out.str();
            }
        } catch (const TestFailedException& e) {
            test.result = TestResult::FAIL;
            ostringstream out;
            out << e.what() << endl;
            test.detailMessage = out.str();
        } catch (const ErrorException& e) {
            test.result = TestResult::EXCEPTION;
            ostringstream out;
            out << "Test failed due to the program triggering an ErrorException." << endl;
            out << endl;
            out << "This means that the test did not fail because of a call" << endl;
            out << "to EXPECT() or EXPECT_ERROR() failing, but rather because" << endl;
            out << "some code explicitly called the error() function." << endl;
            out << endl;
            out << "Error: " << e.getMessage();
            test.detailMessage = out.str();
        } catch (const exception& e) {
            test.result = TestResult::EXCEPTION;
            ostringstream out;
            out << "Test failed due to the program triggering an exception." << endl;
            out << endl;
            out << "This means that the test did not fail because of a call" << endl;
            out << "to EXPECT() or an EXPECT_ERROR() failing, but rather because" << endl;
            out << "some code - probably an internal C++ library - triggered" << endl;
            out << "an error." << endl;
            out << endl;
            out << "Error: " << e.what() << endl;
            test.detailMessage = out.str();
        } catch (...) {
            test.result = TestResult::EXCEPTION;
            ostringstream out;
            out << "Test failed due to the program triggering an unknown type" << endl;
            out << "of exception. " << endl;
            out << endl;
            out << "This means that the test did not fail because of a call" << endl;
            out << "to EXPECT() or an EXPECT_ERROR() failing, but rather because" << endl;
            out << "some code triggered an error whose format we couldn't" << endl;
            out << "recognize." << endl;
            out << endl;
            test.detailMessage = out.str();
        }
        string indented;
        for (auto& line : stringSplit(test.detailMessage, "\n")) {
            indented += "    " + line + "\n";
        }
        test.detailMessage = indented;
    }

    /* Displays all the results from the given test group. */
    string displayResults(GBrowserPane *bp, const string& stylesheet, const Vector<TestGroup>& testGroups, int npass=-1, int nrun=-1)
    {
        stringstream h;
        string conclusion;

        if (!bp) return affirmation();

        h << "<html><style>" << stylesheet << "</style><body><ul>";
        for (const auto& group: testGroups) {
            if (!group.selected) continue;
            h << "<li class=" << quotedVersionOf("title") << "> Tests from " << group.name << "</li>" ;
            /* Display each test as list item */
            for (const auto& test: group.tests) {
                h << "<hr>" << endl;
                string li = "<li class=" + quotedVersionOf(info[test.result].id) + ">";
                h << li << "<b>" << info[test.result].status << "</b> " << test.id << " " << test.testname << "</li>";
                if (!test.detailMessage.empty()) {
                    h << li << "<pre>" << sanitize(test.detailMessage) << "</pre></li>";
                }
                h << endl;
            }
            h << "<hr>" << endl;
        }
        h << "</ul>";
        if (nrun > 0) { // all tests completed
            h << "<h3> Passed " << npass << " of " << nrun << " tests.&nbsp;"; // trailing space after period consumed otherwise
            if (npass == nrun) conclusion = affirmation();
            h << conclusion << "</h3>";

        }
        h << "</body></html>";
        bp->setTextPreserveScroll(h.str(), nrun == -1); // patch now incorporated into library
        return conclusion;
    }

    void addTestToGroup(Map<string, TestGroup>& map, TestCase tcase, string groupname)
    {
        if (!map.containsKey(groupname)) {
            TestGroup tg;
            tg.name = groupname;
            tg.selected = false;
            map[groupname] = tg;
        }
        Test t;
        t.testname = tcase.testname;
        t.callback = tcase.callback;
        ostringstream os;
        if (tcase.filename == groupname) {
            os << " (" << tcase.owner << ", line " << setw(3) << tcase.line << ") ";
        } else {
            os << " (" << tcase.owner << ", " << tcase.filename << ":" << tcase.line << ") ";
        }
        t.id = os.str();
        t.result = TestResult::WAITING; // It hasn't run yet
        map[groupname].tests += t;
    }

   Vector<TestGroup> prepareGroups(const map<TestKey, multimap<int, TestCase>> &allTests)
   {
        Map<string, TestGroup> grouped;

        for (const auto& module: allTests) {
            for (const auto& rawTest: module.second) {
                TestCase tcase = rawTest.second;
                addTestToGroup(grouped, tcase, tcase.owner);   // add once in owner group
                addTestToGroup(grouped, tcase, module.first);  // add again in module group
            }
        }
        return grouped.values();
    }

    void runSelectedGroups(Vector<TestGroup>& groups, Where where)
    {
        GBrowserPane *bp = nullptr;
        string stylesheet;
        bool displayWindow = (where == CONSOLE_AND_WINDOW || where == WINDOW_ONLY);
        bool displayConsole = (where == CONSOLE_AND_WINDOW || where == CONSOLE_ONLY);
        ostringstream devnull;
        ostream& console = displayConsole ? cout : devnull;

        if (displayWindow) {
            GWindow* window = new GWindow(700, 600);
            bp = new GBrowserPane();
            window->setTitle("SimpleTest " + QCoreApplication::applicationName().toStdString());
            window->setExitOnClose(true);
            window->add(bp);

            window->setVisible(true);
            window->requestFocus();
            // student can customize display by editing the stylesheet
            readEntireFile("testing/styles.css", stylesheet);
        }

        /* Show everything so there's some basic data available. */
        displayResults(bp, stylesheet, groups);

        int nrun=0, npassed=0;
        const int test_name_length = 30;
        /* Now, go run the tests. */
        for (auto& group: groups) {
            if (!group.selected) continue;
            console << endl << "[SimpleTest] ---- Tests from " << group.name << " -----" << endl;
            for (auto& test: group.tests) {
                /* Make clear that we're running the test. */
                test.result = TestResult::RUNNING;
                displayResults(bp,stylesheet, groups);
                console << "[SimpleTest] starting" << test.id << left << setfill('.') << setw(test_name_length) << test.testname.substr(0,test_name_length) << "... " << flush;
                runSingleTest(test);
                nrun++;
                if (test.result == TestResult::PASS) npassed++;
                string status = info[test.result].status;
                if (!getConsoleEnabled()) status = info[test.result].color + status + NORMAL;
                console << " =  " << status << endl << test.detailMessage << flush;
                displayResults(bp, stylesheet, groups);
            }
        }
        string conclusion = displayResults(bp, stylesheet, groups, npassed, nrun);
        console << "You passed " << npassed << " of " << nrun << " tests. " << conclusion << endl << endl;
        if (npassed < nrun) {
            cout << "Failed tests:" << endl;
            for (const auto& group: groups) {
                if (!group.selected) continue;
                for (const auto& test: group.tests) {
                    if (test.result != TestResult::PASS)
                        cout << info[test.result].status << test.id << test.testname << endl;
                }
            }
        }
    }

    int userChoiceFromMenu(Vector<string>& options)
    {
        cout << endl;
        cout << "Tests are grouped by filename or by type." << endl;
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

    int getChoice(Vector<TestGroup>& groups, Choice ch, std::string groupName)
    {
        if (ch == ALL_TESTS ) {
            return groups.size()+1;
        } else {
            Vector<string> names = { "None (instead execute ordinary main() function)", "All of the above tests" };
            for (const auto& entry: groups) {
                int index = names.size() - 1;
                names.insert(index, "From " + entry.name);
                if (entry.name == groupName) {
                    return index;
                }
            }
            return userChoiceFromMenu(names);
        }
    }

    bool selectChosenGroup(Vector<TestGroup>& groups, Choice ch, std::string groupName)
    {
        int chosen = getChoice(groups, ch, groupName);

        if (chosen == 0) {
            return false;
        } else {
            for (int i = 0; i < groups.size(); i++) {
                groups[i].selected = (chosen == i+1 || chosen == groups.size()+1);
            }
            return true;
        }
    }

    bool runSimpleTests(Choice ch, Where where, std::string groupName = "")
    {
        // suppress harmless warning about font substitutions (this should be in library too)
        QLoggingCategory::setFilterRules("qt.qpa.fonts.warning=false");

        Vector<TestGroup> testGroups = prepareGroups(gTestsMap());

        if (selectChosenGroup(testGroups, ch, groupName)) {
            runSelectedGroups(testGroups, where);
            return true;
        } else {
            return false;
        }
    }
}

bool runSimpleTests(Choice ch, Where where) {
    return SimpleTest::runSimpleTests(ch, where, "");
}
bool runSimpleTests(std::string groupName, Where where) {
    return SimpleTest::runSimpleTests(SELECTED_TESTS, where, groupName);
}


