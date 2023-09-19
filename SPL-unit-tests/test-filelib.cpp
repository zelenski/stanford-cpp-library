/*
 * Test file for verifying the Stanford C++ lib system functionality.
 */

#include "SimpleTest.h"
#include "filelib.h"
#include "strlib.h"
using namespace std;

PROVIDED_TEST("Test filelib path utility functions") {
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
}

PROVIDED_TEST("Test filelib create/rename/delete files and directories") {
    string tmp = getTempDirectory();
    EXPECT(fileExists(tmp));
    EXPECT(isDirectory(tmp));
    EXPECT(!isFile(tmp));
    string parent = tmp + getDirectoryPathSeparator() + "filelib-" + integerToString(time(0));
    cout << parent << endl;
    createDirectory(parent);
    EXPECT(fileExists(parent));
    EXPECT(isDirectory(parent));
    EXPECT(!isFile(parent));
    string name = "file.txt";
    string filename = parent + getDirectoryPathSeparator() + name;
    string str = "0123456789";
    EXPECT(writeEntireFile(filename, str));
    EXPECT(fileExists(filename));
    EXPECT(isFile(filename));
    EXPECT_EQUAL(fileSize(filename), str.length());
    EXPECT_EQUAL(listDirectory(parent), { name });
    EXPECT(writeEntireFile(filename, str, true)); // append
    EXPECT_EQUAL(fileSize(filename), 2*str.length());
    renameFile(filename, filename + "2");
    EXPECT(fileExists(filename + "2"));
    EXPECT(!fileExists(filename));
    EXPECT_EQUAL(listDirectory(parent), { name + "2" });
    deleteFile(filename + "2");
    EXPECT_EQUAL(listDirectory(parent), {});
    string subdir = parent + "/nope/sorry";
    EXPECT_ERROR(createDirectory(subdir));
    EXPECT(!fileExists(subdir));
    EXPECT_NO_ERROR(createDirectoryPath(subdir));
    EXPECT(fileExists(subdir));
}

static string openAndRead(string name) {
    ifstream in(name);
    if (in.fail()) error("Cannot open file");
    return readEntire(in);
}

PROVIDED_TEST("Test filelib read/write line endings") {
    ofstream out("output");
    Vector<string> vec = {"1", "2", "3", "4", "5"};
    for (string s: vec) {
        out << s << endl;
    }
    out.close();

    string myOut = openAndRead("output");
    Vector<string> options = {"res/end_cr", "res/end_lf", "res/end_crlf"};
    for (string f: options) {
        ifstream in;
        EXPECT(openFile(in, f));
        string raw = readEntire(in);
        rewindStream(in);
        Vector<string> lines = readLines(in);
        if (lines == vec && myOut != raw) {
           cout << "Surprise: This platform can read/convert line endings: " << f << endl;
        } else if (myOut == raw) {
            EXPECT_EQUAL(lines, vec);
            cout << "This platform read/writes line endings: " << f << endl;
        } else {
            cout << "This platform CANNOT read line endings: " << f << endl;
        }
    }
    deleteFile("output");
    EXPECT(!fileExists("output"));
}

