/*
 * Test file for verifying the Stanford C++ lib system functionality.
 */

#include "SimpleTest.h"
#include "simpio.h"
#include "ioutils.h"
#include "strlib.h"
#include "filelib.h"
using namespace std;


PROVIDED_TEST("getChar rejects empty input, reprompt") {
    ioutils::setConsoleEchoUserInput(true);
    string input = stringJoin({"", "a", "", "b", "", "c"}, '\n');
    ioutils::redirectStdinBegin(input);

    EXPECT_EQUAL(getChar(), 'a');
    EXPECT_EQUAL(getChar("Enter a nice character:"), 'b');
    EXPECT_EQUAL(getChar("Enter a nice char or else:", "I am mad!"), 'c');
    ioutils::redirectStdinEnd();
}

PROVIDED_TEST("getInteger rejects malformed input, reprompt") {
    ioutils::setConsoleEchoUserInput(true);
    string input = stringJoin({"", "0", "8junk", "1", "2.5", "-2"}, '\n');
    ioutils::redirectStdinBegin(input);

    EXPECT_EQUAL(getInteger(), 0);
    EXPECT_EQUAL(getInteger("Enter a nice int:"), 1);
    EXPECT_EQUAL(getInteger("Enter a nice int or else:", "I am mad!"), -2);
    ioutils::redirectStdinEnd();
}

PROVIDED_TEST("getDouble rejects malformed input, reprompt") {
    ioutils::setConsoleEchoUserInput(true);
    string input = stringJoin({"", "0", "8junk", ".1", "2.5.1", "-2.5"}, '\n');
    ioutils::redirectStdinBegin(input);

    EXPECT_EQUAL(getDouble(), 0);
    EXPECT_EQUAL(getDouble("Enter a nice double:"), .1);
    EXPECT_EQUAL(getDouble("Enter a nice double or else:", "I am mad!"), -2.5);
    ioutils::redirectStdinEnd();
}

PROVIDED_TEST("getYesOrNo rejects malformed input, reprompt") {
    ioutils::setConsoleEchoUserInput(true);
    string input = stringJoin({"ok", "y", "X", "", "YE", "yEs", "not", "N"}, '\n');
    ioutils::redirectStdinBegin(input);

    EXPECT(getYesOrNo("Enter yes or no or else:", "I am mad"));
    EXPECT(getYesOrNo("Enter to continue:", "just hit return!", "y"));
    EXPECT(getYesOrNo("Will you?"));
    EXPECT(getYesOrNo("Say Yes:"));
    EXPECT(!getYesOrNo("Say No:"));
    EXPECT(!getYesOrNo("Why not?"));
    ioutils::redirectStdinEnd();
}

PROVIDED_TEST("getIntegerBetween rejects values outside range") {
    ioutils::setConsoleEchoUserInput(true);
    string input = stringJoin({"2", "-1.0", "-2"}, '\n');
    ioutils::redirectStdinBegin(input);

    int low = -2, high = -1;
    cout << endl << "low = " << low << " high = " << high << endl;
    int val = getIntegerBetween("Enter int between low and high:", low, high);
    EXPECT(val >= low && val <= high);
    ioutils::redirectStdinEnd();
}

PROVIDED_TEST("getDoubleBetween rejects values outside range") {
    ioutils::setConsoleEchoUserInput(true);
    string input = stringJoin({".1", "0.0", "-2.5", "0.36"}, '\n');
    ioutils::redirectStdinBegin(input);

    double low = 0.25, high = 0.5;
    cout << endl << "low = " << low << " high = " << high << endl;
    double val = getDoubleBetween("Enter double between low and high:", low, high);
    EXPECT(val >= low && val <= high);
    ioutils::redirectStdinEnd();
}

PROVIDED_TEST("promptUserForFile accepts valid filenames, rejects others") {
    ioutils::setConsoleEchoUserInput(true);
    string input = stringJoin({"", "garbage", __FILE__, "", "/s", __FILE__}, '\n');
    ioutils::redirectStdinBegin(input);

    EXPECT(fileExists(promptUserForFilename()));
    EXPECT(fileExists(promptUserForFilename("Enter a nice filename", "I am mad!")));
    ioutils::redirectStdinEnd();
}
