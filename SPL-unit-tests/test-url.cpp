/*
 * Test file for verifying the Stanford C++ lib urlstream functionality.
 */


#include "urlstream.h"
#include <iostream>
#include <string>
#include "testing/SimpleTest.h"


PROVIDED_TEST("download url using iurlstream") {
    std::cout << "Downloading ..." << std::endl;
    iurlstream testurl;
    testurl.open("http://martystepp.com/");
    std::cout << "in the URL stream: " << std::endl;
    std::cout << "====================" << std::endl;
    std::string line;
    while (getline(testurl, line)) {
        std::cout << "line: " << line << std::endl;
    }
    testurl.close();
    std::cout << "====================" << std::endl;
}
