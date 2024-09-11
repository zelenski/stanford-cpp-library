/*
 * Test file for verifying the Stanford C++ lib urlstream module.
 */

#include "urlstream.h"
#include <iostream>
#include <string>
#include "SimpleTest.h"

PROVIDED_TEST("download url using iurlstream") {
    std::cout << "Downloading ..." << std::endl;
    iurlstream testurl;
    testurl.open("https://web.stanford.edu/class/cs106b/");
    std::cout << "in the URL stream: " << std::endl;
    std::cout << "====================" << std::endl;
    std::string line;
    while (getline(testurl, line)) {
        std::cout << "line: " << line << std::endl;
    }
    testurl.close();
    std::cout << "====================" << std::endl;
}
