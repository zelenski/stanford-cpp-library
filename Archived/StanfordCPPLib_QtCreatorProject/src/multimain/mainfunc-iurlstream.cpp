/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include <iostream>
#include <string>
#include "gdownloader.h"
#include "urlstream.h"

void testAllUrls();

int multimain(url) {
    std::cout << "Downloading ..." << std::endl;
    testAllUrls();
    std::cout << "Downloads complete." << std::endl;
    return 0;
}

void testURL(const std::string& url) {
//    GDownloader* downloader = new GDownloader();
//    string text = downloader->downloadAsString(url);
//    if (downloader->hasError()) {
//        cout << "error downloading " << url << ":" << endl << downloader->getErrorMessage() << endl;
//    } else {
//        cout << "text of " << url << ":" << endl
//             << text << endl
//             << "=======================================================" << endl;
//    }

    iurlstream input;
    input.setHeader("User-Agent",
            "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.4; en-US; rv:1.9.2.2) Gecko/20100316 Firefox/3.6.2");
    input.open(url);
    if (!input) {
        std::cerr << "An error occurred: " << input.getErrorCode() << " " << input.getErrorMessage() << std::endl;
    } else {
        std::cout << input.str() << std::endl;
    }
}

void testAllUrls() {
    testURL("http://www.martystepp.com/");
    testURL("https://en.wikipedia.org/w/index.php?title=Quokka&action=edit");
    testURL("https://en.wikipedia.org/w/api.php?action=query&format=json&titles=Quokka&prop=extracts&explaintext&exlimit=1");
    testURL("https://en.wikipedia.org/w/api.php?action=query&format=json&titles=Quokka&redirects");
    testURL("http://www.bogusbogus.com/bogusbogus.html");
    testURL("http://www.google.com/bogusbogus.html");
}
