///*
// * Test file for verifying the Stanford C++ lib functionality.
// */

//#include <iostream>
//#include "console.h"
//#include "process.h"
//using namespace std;

//void testProcess();

//int mainProcess() {
//    cout << "Stanford C++ lib tester" << endl;
//    testProcess();
//    return 0;
//}

//void testProcess() {
//#ifdef PROCESS_H_ENABLED   // won't be enabled
//    Process proc;
//    proc.addCommandLineArgs({
////        "/bin/uname",
////        "-a"
//        // "/home/stepp/Dropbox/data/docs/stanford/StanfordCPPLib/test_programs/exitcode42"
//        // "/home/stepp/Dropbox/data/docs/stanford/StanfordCPPLib/test_programs/delayed_output"
//        "/home/stepp/Dropbox/data/docs/stanford/StanfordCPPLib/test_programs/infinite_loop"
//    });
//    cout << "my process is: " << proc << endl << endl;
//    cout << "About to call startAndWait ..." << endl;

//    proc.setTimeout(1000);
//    proc.startAndWait();

//    cout << "Done with startAndWait" << endl << endl;
//    cout << "my process is: " << proc << endl << endl;

//    cout << "About to call stop ..." << endl;
//    proc.stop();
//    cout << "Done with stop" << endl << endl;

//    cout << "exit code:" << proc.exitCode() << endl;

//    cout << "output (length " << proc.output().length() << "):" << endl
//         << proc.output() << endl << endl;
//    cout << "my process is: " << proc << endl << endl;
//#endif // PROCESS_H_ENABLED
//}
