/*
 * Test file for verifying the Stanford C++ lib system functionality.
 */

#include "common.h"
#include "SimpleTest.h"
#include "filelib.h"
#include "strlib.h"
using namespace std;

PROVIDED_TEST("filelib path utility functions") {
    string cwd = getCurrentDirectory();
    EXPECT(fileExists(cwd));
    EXPECT(isDirectory(cwd));
    EXPECT(!isFile(cwd));

    string thisFile = __FILE__;
    EXPECT(fileExists(thisFile));
    EXPECT(isFile(thisFile));
    EXPECT(!isDirectory(thisFile));
    string expanded = expandPathname(thisFile);
    EXPECT(fileExists(expanded));
    string abs = getAbsolutePath(thisFile);
    EXPECT(fileExists(expanded));
    EXPECT_EQUAL(getExtension(thisFile), ".cpp");
    EXPECT_EQUAL(getRoot(getTail(thisFile)), "test-filelib");
    EXPECT_EQUAL(getHead(abs), cwd);

    Vector<string> files = listDirectory("res");
    cout << files << endl;

    string path = "res/lf_unix";
    EXPECT(fileExists(path));
    EXPECT_EQUAL(fileSize(path), fileSize( "res/cr_mac"));
}

PROVIDED_TEST("read entire file, discard line endings, confirm contents read correctly") {
    string lf = readEntireFile("res/lf_unix");
    string cr = readEntireFile("res/cr_mac");
    string crlf = readEntireFile("res/crlf_windows");
    string without_lf = stringReplace(lf, "\n", "");
    string without_cr = stringReplace(cr, "\r", "");
    string without_crlf = stringReplace(crlf, "\r\n", "");
    EXPECT_EQUAL(without_lf, without_cr);
    EXPECT_EQUAL(without_lf, without_crlf);
}

PROVIDED_TEST("read lines") {
    ifstream in;
    Vector<string> lines;

    openFile(in, "res/lf_unix");
    readEntireFile(in, lines);
    EXPECT_EQUAL(lines.size(), 10);

    openFile(in, "res/cr_mac");
    readEntireFile(in, lines);
    EXPECT_EQUAL(lines.size(), 10);

    openFile(in, "res/crlf_windows");
    readEntireFile(in, lines);
    EXPECT_EQUAL(lines.size(), 10);
}
