///*
// * Test file for verifying the Stanford C++ lib functionality.
// */

//#include <iostream>
//#include <string>
//#include "urlstream.h"
//using namespace std;

//void testAllUrls();

//int mainUrlStream() {
//    testAllUrls();
//    return 0;
//}

//void testURL(const string& url) {
//   iurlstream input;
//   input.setHeader("User-Agent",
//           "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.4; en-US; rv:1.9.2.2) Gecko/20100316 Firefox/3.6.2");
//   input.open(url);
//   if (!input) {
//       cerr << "An error occurred: " << input.getErrorCode() << endl;
//   } else {
//       cout << input.rdbuf() << endl;
//   }
//}

//void testAllUrls() {
//   testURL("https://en.wikipedia.org/w/index.php?title=Quokka&action=edit");
//   testURL("https://en.wikipedia.org/w/api.php?action=query&format=json&titles=Quokka&prop=extracts&explaintext&exlimit=1");
//   testURL("https://en.wikipedia.org/w/api.php?action=query&format=json&titles=Quokka&redirects");
//}
