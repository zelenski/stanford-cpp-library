/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

////#include "console.h"
//#include "error.h"
//#include "collections.h"
//#include "exceptions.h"
//#include "gwindow.h"
//#include "gevents.h"
//#include "map.h"
//#include "note.h"
//#include "process.h"
//#include "vector.h"
//#include "queue.h"
//#include "random.h"
//#include "strlib.h"
//// #include "testcases.h"
//#include "private/version.h"
//#include "types.h"
//#include <exception>

#include <iomanip>
#include <iostream>

//#include <iostream>
//#include <string>
//#include "basicgraph.h"
//#include "call_stack.h"
//#include "console.h"
//#include "error.h"
//#include "exceptions.h"
//#include "map.h"
//#include "simpio.h"
//#include "vector.h"
//#include "thread.h"
//#include "biginteger.h"
//using namespace std;

//void bar() {
//    int x = 3;
//    int y = 5;
//    x += y;

//    int a = 3;
//    int b = 5;
//    a += b;

//    Vector<int> v;
//    v.add(42);
//    error(":-(");

//    int c = 2;
//    int d = 4;
//    c += d;
//    cout << "duh duh duh " << a << b << c << d << x << y << endl;
//}

//void foo() {
//    bar();
//}

//void test(Map<int, string> m) {
//    cout << m << endl;
//}

//void testNotes() {
//    // Note note(1.5, Note::G, 5, Note::NATURAL, false);
//    Note note("1.5 G 5 NATURAL false");
//    Note rest("1 R false");
//    // std::cout << "NOTE: " << note << std::endl;
//    std::cout << "about to play ... " << std::endl;
//    note.play();
//    rest.play();
//    std::cout << "about to play 2 ... " << std::endl;
//    note.play();
//    rest.play();
//}

//void testKeithWindowTest() {
//    // std::cout << "Complete." << std::endl;
////    cout << "You're about to be prompted to enter some text." << endl;
////    cout << "When you do, close the window." << endl;
////    cout << "Then, look down at stdout." << endl;
////    cout << "You should notice that some input was entered even though" << endl;
////    cout << "nothing was echoed to the graphical console." << endl;
////    cout << "This desyncs the graphics console from stdin/stdout," << endl;
////    cout << "though I don't know why." << endl;

//    GWindow gw(400, 300);
//    gw.setColor("blue");
//    gw.fillPolygon({30, 20, 90, 50, 70, 120, 150, 100, 180, 10});

//    for (int i = 0; ; i++) {
//        string line = getLine("Enter line " + to_string(i) + ": ");
//        cout << "You entered: " << line << endl;
//        cout << "Now waiting for a click event:" << endl;
//        GMouseEvent event = waitForClick();
//        cout << "event = " << event << endl;
//    }
//}

///*
//void testProcess() {
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
//}
//*/

//int f() {
//    if (2 % 2 == 1) {
//        return 46;
//    } else {
//        return 45;
//    }
//}

//void voidf() {
////    if (rand() && 2 % 2 == 1) {
////        cout << "f says 42!" << endl;
////    } else {
//        cout << "f says 43!" << endl;
////    }
//}

//bigint fact(int n) {
//    bigint result("1");
//    for (int i = 2; i <= n; i++) {
//        result *= BigInteger(integerToString(i));
//    }
//    return result;
//}

//void testBigInteger() {
//    {
//        BigInteger bi("-24");
//        cout << "abs = " << bi.abs() << endl;
//        cout << "pos: " << boolalpha << bi.isPositive() << ", neg: " << bi.isNegative() << ", nonneg: " << bi.isNonNegative() << endl;
//        bi = bi.abs();
//        cout << "pos: " << boolalpha << bi.isPositive() << ", neg: " << bi.isNegative() << ", nonneg: " << bi.isNonNegative() << endl;
//        cout << "pos: " << boolalpha << BigInteger::ZERO.isPositive() << ", neg: " << BigInteger::ZERO.isNegative() << ", nonneg: " << BigInteger::ZERO.isNonNegative() << endl;

//        BigInteger bi2("16");
//        cout << "gcd = " << bi.gcd(bi2) << endl << endl;

//        do {
//            cout << "int bi2 = " << bi2 << ", hex = " << bi2.toString(16) << ", oct = " << bi2.toString(8) << ", bin = " << bi2.toString(2) << endl;
//            bi2 *= BigInteger::TWO;
//        } while (bi2.isInt());

//        do {
//            cout << "long bi2 = " << bi2 << endl;
//            bi2 *= BigInteger::TEN;
//        } while (bi2.isLong());

//        cout << "max = " << bi.max(bi2) << ", min = " << bi.min(bi2) << endl;

//        cout << "modPow = " << BigInteger::TWO.modPow(10, 100) << endl;

//        cout << "pow = " << endl;
//        for (int i = 0; i < 20; i++) {
//            cout << "2 ^ " << i << " = " << BigInteger::TWO.pow(i) << endl;
//        }

//        // radix stuff
//        BigInteger biHex("0xff00", 16);
//        cout << "bihex = " << biHex << ", hex = " << biHex.toString(16) << endl;
//        BigInteger biBin("0b101010", 2);
//        cout << "bibin = " << biBin << ", bin = " << biBin.toString(2) << endl;

//        BigInteger biUnary("1111111", 1);
//        cout << "biunary = " << biUnary << ", unary " << biUnary.toString(1) << endl;

//        for (BigInteger bi = 1; bi <= 100; bi++) {
//            cout << "int bi = " << bi << ", hex = " << bi.toString(16) << ", oct = " << bi.toString(8) << ", bin = " << bi.toString(2) << endl;
//        }

//        // various int/long/double x BigInteger binary operations
//        cout << "bitwise stuff:" << endl;
//        BigInteger binary1("10100011", 2);
//        BigInteger binary2("01110110", 2);
//        cout << "a     = " << binary1.toString(2) << endl;
//        cout << "b     = " << setw(8) << binary2.toString(2) << endl;
//        cout << "a & b = " << setw(8) << (binary1 & binary2).toString(2) << endl;
//        cout << "a | b = " << setw(8) << (binary1 | binary2).toString(2) << endl;
//        cout << "a ^ b = " << setw(8) << (binary1 ^ binary2).toString(2) << endl;
//        cout << "~a    = " << setw(8) << (~binary1).toString(2) << endl;
//        cout << "!a    = " << setw(8) << (!binary1).toString(2) << endl;
//        cout << endl;

//        cout << "int +: " << (42 + bi) << endl;
//        cout << "int -: " << (42 - bi) << endl;
//        cout << "int *: " << (42 * bi) << endl;
//        cout << "int /: " << (42 / bi) << endl;
//        cout << "int %: " << (42 % bi) << endl;

//        cout << "long +: " << ((long)42 + bi) << endl;
//        cout << "long -: " << ((long)42 - bi) << endl;
//        cout << "long *: " << ((long)42 * bi) << endl;
//        cout << "long /: " << ((long)42 / bi) << endl;
//        cout << "long %: " << ((long)42 % bi) << endl;

////        cout << "double +: " << (42.5 + bi) << endl;
////        cout << "double -: " << (42.5 - bi) << endl;
////        cout << "double *: " << (42.5 * bi) << endl;
////        cout << "double /: " << (42.5 / bi) << endl;
////        cout << "double %: " << (42.5 % bi) << endl;
//    }

//    try {
//        BigInteger bad("-845lol098");
//        cout << bad << endl;
//    } catch (ErrorException& ex) {
//        cout << "correctly threw exception when creating bad integer: " << ex.getMessage() << endl;
//    }

//    // big division
//    BigInteger realbig(1000);
//    realbig *= realbig;
//    realbig *= realbig;
//    realbig *= realbig;
//    realbig *= realbig;
//    BigInteger almostAsBig(realbig);
//    realbig *= realbig;
//    cout << "realbig: " << realbig << endl;
//    cout << "almost : " << almostAsBig << endl;

//    try {
//        BigInteger quotient = realbig / almostAsBig;
//        cout << "quotient: " << quotient << endl;
//    } catch (ErrorException& ex) {
//        cout << "correctly threw exception when dividing by large integer" << endl;
//    }


////    cout << "Fib:" << endl;
////    BigInteger a(1);
////    BigInteger b(1);
////    for (int i = 0; i < 100; i++) {
////        BigInteger c = a + b;
////        cout << "#" << i << ": " << c << endl;
////        a = b;
////        b = c;
////    }

////    cout << "Fact:" << endl;
////    bigint f;
////    for (int i = 0; i < 50; i++) {
////        f = fact(i);
////        cout << "fact(" << i << ") = " << f << endl;
////    }

////    for (int i = 0; i < 19; i++) {
////        bigint g = fact(i);
////        bigint quotient = f / g;
////        bigint mod = f / g;

////        cout << "i = " << i << ", fact = " << g << ", quot = " << quotient << ", mod = " << mod << endl;
////    }
//}

//void stutter(Queue<int>& q) {
//    int size = q.size();
//    for (int i = 0; i < size; i++) {
//        q.enqueue(q.peek());
//        q.enqueue(q.dequeue());
//    }
//}

//int main() {
//    cout << "Stanford C++ lib tester" << endl;

//    Queue<int> q {17, -4, 0, 212, 212, 8468};
//    stutter(q);
//    cout << "Q = " << q << endl;

//    testBigInteger();

//    // testKeithWindowTest();

////    cout << "f is: " << f() << endl;


////    Thread th = forkThread(voidf);

////    testNotes();

////    testProcess();

////    cout << stanfordcpplib::collections::compareTo(42, 17) << endl;
////    cout << stanfordcpplib::collections::compareTo2(42, 42, 17, 35) << endl;
////    cout << stanfordcpplib::collections::compareTo3(42, 42, "a", "a", 'b', 'a') << endl;
////    cout << stanfordcpplib::collections::compareTo4(42, 42, "a", "a", 'b', 'b', 1.0, 0.5) << endl;
////    cout << stanfordcpplib::collections::compareTo5(42, 42, "a", "a", 'b', 'b', 1.0, 1.0, 17, 29) << endl;
////    cout << stanfordcpplib::collections::compareTo5(42, 42, "a", "a", 'b', 'b', 1.0, 1.0, 37, 29) << endl;
////    cout << stanfordcpplib::collections::compareTo5(42, 42, "a", "a", 'b', 'b', 1.0, 1.0, 29, 29) << endl;

//    /*
//    while (true) {
//        cout << "   DATA STRUCTURES::" << endl;
//        cout << "c) collections" << endl;
//        cout << "cs) collection stream extraction" << endl;
//        cout << "ci) collection initializer lists" << endl;
//        cout << "csh) collection shuffle" << endl;

//        cout << "   STUFF RELATED TO INPUT/OUTPUT:" << endl;
//        cout << "e) cout / cerr mix" << endl;
//        cout << "i) cin / cout" << endl;
//        cout << "an) animation" << endl;
//        cout << "p) pipe" << endl;
//        cout << "s) strings" << endl;
//        cout << "u) urlstream" << endl;
        
//        cout << "   GRAPHICS:" << endl;
//        cout << "fd) file dialog" << endl;
//        cout << "bi) buffered image" << endl;
//        cout << "op) optionpane" << endl;
//        cout << "rb) radio buttons" << endl;
//        cout << "ta) table" << endl;

//        cout << "   HTTP SERVER:" << endl;
//        cout << "ss) start server" << endl;

//        cout << "   ERROR HANDLING AND STACK TRACES:" << endl;
//        cout << "   (Try all 3 to make sure stack trace prints.)" << endl;
//        cout << "   (Current configuration shows no trace on stack overflow.)" << endl;
//        cout << "n) segfault (NULL ptr)" << endl;
//        cout << "t) throw exception" << endl;
//        cout << "o) stack overflow" << endl;
//        cout << "ri) recursion indent" << endl;
//        cout << endl;
//        string cmd = getLine("Command (Enter to quit)?");
//        if (cmd.empty()) {
//            break;
//        } else if (cmd == "an") {
//            animationTest();
//        } else if (cmd == "c") {
//            randomElementTest();
//            compareTest();
//            foreachTest();
//            hashCodeTest();
//        } else if (cmd == "cs") {
//            streamExtractTest();
//        } else if (cmd == "csh") {
//            shuffleTest();
//        } else if (cmd == "ci") {
//            initializerListTest();
//        } else if (cmd == "e") {
//            coutCerrMixTest();
//        } else if (cmd == "fd") {
//            fileDialogTest();
//        } else if (cmd == "bi") {
//            gbufferedImageTest();
//        } else if (cmd == "op") {
//            goptionpaneTest();
//        } else if (cmd == "rb") {
//            radioButtonTest();
//        } else if (cmd == "i") {
//            // cinOutTest();
//            killProcessTest();
//        } else if (cmd == "n") {
//            segfaultTest();
//        } else if (cmd == "p") {
//            outputColorTest();
//            cinOutTest();
//            coutCerrMixTest();
//            getIntegerTest();
//            longStringTest();
//            // killProcessTest();
//        } else if (cmd == "o") {
//            //getPlatform()->setStackSize(1024*1024*128);
//            stackOverflowTest();
//        } else if (cmd == "ri") {
//            recursionIndentTest();
//        } else if (cmd == "s") {
//            stringToIntegerTest();
//        } else if (cmd == "ss") {
//            serverTest();
//        } else if (cmd == "t") {
//            exceptionTest();
//        } else if (cmd == "ta") {
//            gtableTest();
//        } else if (cmd == "u") {
//            urlstreamTest();
//        }
//    }
//    */
    
//    cout << "Done." << endl;
//    return 0;
//}


#include <iostream>
#include <string>
#include "basicgraph.h"
// #include "console.h"
#include "functional.h"
#include "grid.h"
#include "range.h"
#include "sparsegrid.h"
#include "timer.h"
#include "vector.h"
using namespace std;

int fact(int n) {
    cout << "fact " << n << endl;
    if (n == 0) { return 1; }
    return n * fact(n-1);
}

bool is_even(int n) {
    return n % 2 == 0;
}

int add(int a, int b) {
    return a + b;
}

int squared(int n) {
    return n * n;
}

int len(string s) {
    return (int) s.length();
}

bool strLenLess(const string& s1, const string& s2) {
    return s1.length() < s2.length() ||
            (s1.length() == s2.length() && s1 < s2);
}

////////

#include <iostream>
#include <string>
#include "urlstream.h"
using namespace std;

void testURL(const string& url) {
   iurlstream input;
   input.setHeader("User-Agent",
                   "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.4; en-US; rv:1.9.2.2) Gecko/20100316 Firefox/3.6.2");
   input.open(url);
   if (!input) {
       cerr << "An error occurred: " << input.getErrorCode() << endl;
   } else {
       cout << input.rdbuf() << endl;
   }
}

int testAllUrls() {
   testURL("https://en.wikipedia.org/w/index.php?title=Quokka&action=edit");
   //testURL("https://en.wikipedia.org/w/api.php?action=query&format=json&titles=Quokka&prop=extracts&explaintext&exlimit=1");
   //testURL("https://en.wikipedia.org/w/api.php?action=query&format=json&titles=Quokka&redirects");
   return 0;
}

////////

//void testWindowWithScrollbar() {
//    GWindow window;
//    window.setSize(700, 500);
//    window.setResizable(true);
//    GOval oval(50, 50, 400, 400);
//    window.add(oval);

//    GTextField field(8);
//    window.addToRegion(field, GWindow::REGION_NORTH);
//    field.addActionListener();
//    field.addChangeListener();

//    GSlider slider(0, 100, 50);
//    window.addToRegion(slider, GWindow::REGION_SOUTH);
//    slider.addActionListener();
//    slider.addChangeListener();

//    GFormattedPane pane;
//    window.addToRegion(pane, GWindow::REGION_CENTER);
//    pane.readTextFromUrl("http://en.wikipedia.org/");

//    // TODO: get/setContentType?  text/html,  text/plain?

//    pane.setFont("Monospaced-14");
//    // pane.readTextFromFile("resfile3.html");
//    // pane.readTextFromUrl("http://poopypoopypoopypoopypoopypoopypoopypoopy.poop/");

//    // pane.setText("<h1>yay!!!</h1><p>hi</p><ul><li>lolol<li>bullet2<li>hooray</ul><p>goodbye</p>");

//    while (true) {
//        GEvent event = waitForEvent(ACTION_EVENT | CHANGE_EVENT | HYPERLINK_EVENT | WINDOW_EVENT);
//        if (event.getEventClass() == CHANGE_EVENT) {
//            GChangeEvent changeEvent(event);
//            GObject* source = changeEvent.getSource();
//            if (source == &field) {
//                cout << "field text: \"" << field.getText() << "\"" << endl;
//            } else if (source == &slider) {
//                int value = 2 * slider.getValue();
//                cout << "slider value: " << value << endl;
//                window.setColor(convertRGBToColor(value, value, value));
//                window.fillOval(10, 10, 100, 100);
//            }
//        } else if (event.getEventClass() == HYPERLINK_EVENT) {
//            GHyperlinkEvent hyperlinkEvent(event);
//            pane.readTextFromUrl(hyperlinkEvent.getUrl());
//        } else if (event.getEventType() == WINDOW_CLOSED) {
//            break;
//        }
//    }
//}

#include "qgcolor.h"
#include "qgobjects.h"
#include "qginteractors.h"
#include "qgui.h"

void grayscale(QGBufferedImage* image) {
    // convert image to grayscale
    image->setAutoRepaint(false);
    Timer tim;
    tim.start();
    for (int x = 0; x < image->getWidth(); x++) {
        for (int y = 0; y < image->getHeight(); y++) {
            int rgb = image->getRGB(x, y);
            int r = QGBufferedImage::getRed(rgb);
            int g = QGBufferedImage::getGreen(rgb);
            int b = QGBufferedImage::getBlue(rgb);
            int avg = (r + g + b) / 3;
            rgb = QGBufferedImage::createRgbPixel(avg, avg, avg);
            image->setRGB(x, y, rgb);
        }
    }
    image->repaint();
    tim.stop();
    cout << "took " << tim.elapsed() << "ms" << endl;
}

void testQwindow() {
//    cout << "This is the testQwindow function! yay!" << endl;
//    cout << "testQwindow is in thread " << QGui::instance()->getCurrentThread() << endl;
//    cout << "testQwindow sleeping 500ms ..." << endl;
//    QGui::instance()->getCurrentThread()->msleep(500);
//    cout << "testQwindow done sleeping." << endl;
//    return;

    QGui::instance()->runOnQtGuiThread([]() {
        static QGWindow* window = new QGWindow(900, 500);
        window->setTitle("QtGui Window");
        window->setResizable(true);
        window->setExitOnClose(false);
        window->center();

        QGLabel* label = new QGLabel("Type <b>stuff</b> <i>now</i> (North):");
        label->setIcon("triangle-icon.png");
        label->setColor(QGColor::GREEN);
        label->setBackground(QGColor::YELLOW);
        window->addToRegion(label, "North");
        cout << "label:     " << label->toString() << endl;

        static QGChooser* chooser = new QGChooser({"one", "two", "three four"});
        chooser->setColor(QGColor::RED);
        chooser->setBackground(QGColor::YELLOW);
        chooser->setChangeHandler([]() {
            cout << "changeHandler: chooser was clicked!" << endl;
            cout << "selected: " << chooser->getSelectedIndex() << " : "
                 << chooser->getSelectedItem() << endl;
            cout << "size: " << chooser->size() << endl << endl;
        });

        window->addToRegion(chooser, "South");
        cout << "chooser:   " << chooser->toString() << endl;

        static QGCheckBox* checkBox = new QGCheckBox("Question?", true);
        checkBox->setChangeHandler([](const QGEvent&) {
            cout << "checkbox clicked! " << boolalpha << checkBox->isChecked() << endl;
        });
        window->addToRegion(checkBox, "West");
        window->addToRegion(new QGLabel("Hi!"), "West");
        window->addToRegion(new QGLabel("^_^"), "West");
        window->setRegionAlignment("West", "Top Right");
        cout << "checkbox:  " << checkBox->toString() << endl;

        static QGRadioButton* radio1group1 = new QGRadioButton("A", "group1");
        static QGRadioButton* radio2group1 = new QGRadioButton("B", "group1", true);
        static QGRadioButton* radio3group1 = new QGRadioButton("C", "group1");
        static QGRadioButton* radio1group2 = new QGRadioButton("XX", "group2", true);
        static QGRadioButton* radio2group2 = new QGRadioButton("YY", "group2");

        QGEventHandlerVoid radioChangeHandler = []() {
            cout << "checkbox clicked! " << boolalpha
                 << radio1group1->isChecked() << " "
                 << radio2group1->isChecked() << " "
                 << radio3group1->isChecked() << " "
                 << radio1group2->isChecked() << " "
                 << radio2group2->isChecked() << endl;
        };
        radio1group1->setChangeHandler(radioChangeHandler);
        radio2group1->setChangeHandler(radioChangeHandler);
        radio3group1->setChangeHandler(radioChangeHandler);
        radio1group2->setChangeHandler(radioChangeHandler);
        radio2group2->setChangeHandler(radioChangeHandler);

        window->addToRegion(radio1group1, "East");
        window->addToRegion(radio2group1, "East");
        window->addToRegion(radio3group1, "East");
        window->addToRegion(radio1group2, "East");
        window->addToRegion(radio2group2, "East");
        window->setRegionAlignment("East", "Bottom Right");
        cout << "radio:     " << radio1group1->toString() << endl;

        static QGTextField* textField = new QGTextField("Marty");
        textField->setPlaceholder("type your name");
        // textField->setEditable(false);
        textField->setAutocompleteList({"matt", "Marty", "Stuart", "steve", "yana", "yes", "no"});
        textField->setTextChangeHandler([]() {
            cout << "textfield text changed! text is:" << endl << textField->getText() << endl;
        });
        window->addToRegion(textField, "North");
        cout << "textfield: " << textField->toString() << endl;

        // drawing directly onto window  (will go away when text area is added)
//        window->setLineWidth(10);
//        window->setColor("blue");
//        window->setFillColor("yellow");
//        window->fillOval(20, 120, 40, 60);
//        window->setColor("red");
//        window->setFillColor("green");
//        window->fillRect(10, 30, 120, 70);
//        window->drawLine(100, 100, 200, 150);

    //    static QGTextArea* textArea = new QGTextArea("This is \na multi-line\n\ntext area");
    //    textArea->setRowsColumns(4, 30);
    //    textArea->setPlaceholder("type some text");
    //    textArea->setTextChangeHandler([](const QGEvent&) {
    //        cout << "textarea text changed! text is:" << endl << textArea->getText() << endl;
    //    });
    //    window->addToRegion(textArea, "West");
    //    cout << "textarea:  " << textArea->toString() << endl;

        // TODO: debug crash
//        static QGBufferedImage* image = new QGBufferedImage("lego.png");
//        window->addToRegion(image, "East");

        QGButton* button = new QGButton("Triforce");
        button->setColor(QGColor::RED);
        button->setBackground(QGColor::YELLOW);
        button->setIcon("triangle-icon.png");
        button->setTextPosition(QGInteractor::TEXT_BESIDE_ICON);
        button->setClickHandler([]() {
            window->setResizable(!window->isResizable());
            cout << "clickHandler: button was clicked!" << endl;
            cout << "location:  " << window->getLocation() << endl;
            cout << "size:      " << window->getSize() << endl;
            cout << "visible:   " << boolalpha << window->isVisible() << endl;
            cout << "resizable: " << boolalpha << window->isResizable() << endl << endl;

            // test QGOptionPane
            QGOptionPane::showMessageDialog("I love Yana! <3");

            Vector<string> choices = {"One", "Two", "Three"};
            string result = QGOptionPane::showOptionDialog("Pick a thing", choices);
            cout << "You chose: " << result << endl;

            //    int result = QGOptionPane::showConfirmDialog("Is Yana the most beautiful?", "Important Question", QGOptionPane::YES_NO_CANCEL);
            //    cout << "You chose: " << result << endl;
            //    std::string answer = QGOptionPane::showInputDialog("Who is my baby?", "Baby's name?", "bozo");
            //    cout << "You typed: " << answer << endl;

            //    string filename = QGFileChooser::showOpenDialog("", "*.txt, *.cpp, *.h");
            //    cout << "You chose: " << filename << endl;
            window->clear();
        });
        button->setClickHandler([]() {
            // grayscale(image);
        });
        button->setAccelerator("Ctrl-T");
        window->addToRegion(button, "South");
        cout << "button:    " << button->toString() << endl;
        cout << "button accelerator: " << button->getAccelerator() << endl;
        cout << "button font: " << button->getFont() << endl;
        button->setFont("Monospaced-Bold-14");

        static QGSlider* slider = new QGSlider();
        slider->setMinorTickSpacing(20);
        slider->setPaintLabels(true);
        slider->setPaintTicks(true);
        slider->setChangeHandler([](QGEvent event) {
            cout << "sliderChangeHandler: slider was slid!" << endl;
            cout << "value: " << slider->getValue() << endl;
            cout << "event: " << event << endl;
            window->removeTimerHandler();
        });

        window->addToRegion(slider, "North");
        cout << "slider:    " << slider->toString() << endl;

        // window->pause(500);

        // todo

        window->setWindowHandler([](QGEvent event) {
            // cout << "window! event=" << event << endl;
            if (event.getEventType() == QGEvent::WINDOW_CLOSED) {
                // cout << "window closed!" << endl;
                QGui::instance()->exitGraphics();
            }
        });

        window->setTimerHandler(1000, [](QGEvent event) {
            cout << "timer! event=" << event << endl;
        });
    });
}

QGOval* ball = nullptr;
QGButton* button = nullptr;
QGButton* button2 = nullptr;
QGButton* button3 = nullptr;
QGButton* button4 = nullptr;
QGButton* button5 = nullptr;
QGButton* saveButton = nullptr;
QGLabel* label = nullptr;
double dx = 5;
double dy = 3;

void testQwindowDrawing() {
    static QGWindow* window = nullptr;

    QGui::instance()->runOnQtGuiThread([]() {
        window = new QGWindow(900, 500);
        window->setResizable(false);
        window->setTitle("QtGui Drawing Window");
        // window->setBackground("yellow");
        window->center();

        QGLabel* label = new QGLabel("QtGui <b>AWESOME</b> <i>cool</i> window");
        window->addToRegion(label, "North");

        ball = new QGOval(20, 20, 50, 50);
        ball->setFillColor("red");
        ball->setLineWidth(2);
        ball->setLineStyle(QGObject::LINE_DOT);
        window->add(ball);

        window->setRegionAlignment("North", "Left");
        window->setRegionAlignment("South", "Right");
        button = new QGButton("Tick");
        window->addToRegion(button, "South");

        button2 = new QGButton("Pixels");
        window->addToRegion(button2, "South");

        button3 = new QGButton("Repaint");
        window->addToRegion(button3, "South");

        button4 = new QGButton("Clear");
        window->addToRegion(button4, "South");

        button5 = new QGButton("BG");
        window->addToRegion(button5, "South");

        saveButton = new QGButton("Save");
        window->addToRegion(saveButton, "South");

        label = new QGLabel("????????????");
        window->addToRegion(label, "North");


//        window->setBackground("yellow");
//        window->setColor("red");
//        window->setFillColor("red");
    });

    // drawing directly onto window
//    window->setColor("blue");
//    window->setFillColor("yellow");
//    window->fillOval(20, 120, 40, 60);
//    window->setColor("red");
//    window->setFillColor("green");
//    window->setLineWidth(3);
//    window->fillRect(10, 30, 120, 70);
//    window->drawLine(100, 100, 200, 150);

    // pixely stuff

    window->setColor("black");
    window->setFillColor("red");
    window->setLineWidth(5);   // BUG: affects later shapes
    window->setLineStyle(QGObject::LINE_DASH);
    window->drawRect(20, 220, 100, 100);
    window->fillArc(20, 220, 100, 100, 45, 120);

    window->setFillColor("purple");
    window->setFont("Monospaced-bold-16");
    window->fillPolygon({200, 200, 250, 300, 150, 300});

    window->setColor("green");
    QGString* qgstring = new QGString("Hello, Qt!", 200, 80);
    qgstring->setColor("green");
    qgstring->rotate(10);
    window->add(qgstring);

    QGString* qgstring2 = new QGString("Bye, Felicia!", 220, 100);
    qgstring2->setColor("red");
    qgstring2->scale(1.5);
    window->add(qgstring2);

    QGImage* qgimage = new QGImage("triangle-icon.png", 200, 40);
    window->add(qgimage);

    window->setColor("blue");
    QGString* qgstring3 = new QGString("Third string", 240, 120);
    qgstring3->setColor("blue");
    window->add(qgstring3);

//    // animation loop
//    cout << "testQwindowDrawing: What thread am I? " << QGui::instance()->getCurrentThread() << endl;
//    cout << "testQwindowDrawing: Qt gui main thread = " << QGui::instance()->getQtMainThread()
//         << ", student thread = " << QGui::instance()->getStudentThread() << endl;

    std::function<void()> tickFunc = []() {
//        cout << "tick func: What thread am I? " << QGui::instance()->getCurrentThread() << endl;
//        cout << "tick func: dx=" << dx << ",dy=" << dy << endl;
//        cout << "tick func: ball = " << ball->toString() << endl;
        window->pause(20);
        ball->move(dx, dy);
        double cw = window->getCanvasWidth();
        double ch = window->getCanvasHeight();
        if (ball->getX() < 0 || ball->getRightX() >= cw) {
            dx = -dx;
        }
        if (ball->getY() < 0 || ball->getBottomY() >= ch) {
            dy = -dy;
        }
        // cout << "ball loc is now: " << ball->getLocation() << endl;
    };
    button->setClickHandler(tickFunc);

    button2->setClickHandler([]() {
        for (int y = 3*window->getCanvasHeight()/4; y < window->getCanvasHeight(); y++) {
            for (int x = 3*window->getCanvasWidth()/4; x < window->getCanvasWidth(); x++) {
                int r = (x * 2) % 256;
                int g = (y * 2) % 256;
                int b = ((x+y) * 2) % 256;
                window->setPixel(x, y, r, g, b);
            }
        }
    });

    button3->setClickHandler([]() {
        window->repaint();
    });

    button4->setClickHandler([]() {
        window->clearCanvasPixels();
    });

    button5->setClickHandler([]() {
        window->setBackground("yellow");
    });

    saveButton->setClickHandler([]() {
        string filename = QGFileChooser::showSaveDialog();
        window->saveCanvasPixels(filename);
    });

    window->setMouseHandler([](QGEvent event) {
        // cout << "mouse! event=" << event << endl;
        // cout << "mouse handler: What thread am I? " << QGui::instance()->getCurrentThread() << endl;
        if (event.getType() == QGEvent::MOUSE_DRAGGED) {
            window->setColor("blue");
            window->setFillColor("blue");
            window->setLineStyle(QGObject::LINE_SOLID);
            window->setLineWidth(1);
            window->fillOval(event.getX() - 5, event.getY() - 5, 10, 10);
        } else if (event.getType() == QGEvent::MOUSE_MOVED) {
            label->setText(event.getLocation().toString());
        }
    });

    window->setKeyHandler([](QGEvent event) {
        // cout << "key! event=" << event << endl;
        double balldx = 0;
        double balldy = 0;
        if (event.getKeyCode() == QGEvent::LEFT_ARROW_KEY) {
            balldx = -2;
        } else if (event.getKeyCode() == QGEvent::RIGHT_ARROW_KEY) {
            balldx = 2;
        } else if (event.getKeyCode() == QGEvent::UP_ARROW_KEY) {
            balldy = -2;
        } else if (event.getKeyCode() == QGEvent::DOWN_ARROW_KEY) {
            balldy = 2;
        } else if (event.getKeyCode() == 'f') {
            ball->sendToFront();
        }
        ball->move(balldx, balldy);
    });

//    window->setWindowHandler([](QGEvent /* event */) {
//        // cout << "window! event=" << event << endl;
//    });

//    for (int i = 0; i < 1000; i++) {
//        tickFunc();
//    }
}


int main() {
    testQwindowDrawing();
    return 0;
}

int otherMain() {
    fact(5);

    Vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector<int> v2 = functional::filter(v, is_even);
    cout << "v  : " << v << endl;
    cout << "v2 : " << v2 << endl;

    Vector<int> v3 = functional::map(v, squared);
    cout << "v3 : " << v3 << endl;

    Vector<string> names {"Cynthia", "Marty", "Ed", "", "Bob"};
    Vector<int> lengths;
    functional::map(names, len, lengths);
    cout << "names  : " << names << endl;
    cout << "lengths: " << lengths << endl;

    int result = functional::reduce(v, add);
    cout << "sum: " << result << endl;

    BasicGraph graph;
    graph.addVertex("b");
    graph.addVertex("c");
    graph.addVertex("a");
    graph.addVertex("e");
    graph.addVertex("d");
    graph.addEdge("a", "d");
    graph.addEdge("c", "d");
    graph.addEdge("b", "c");
    graph.addEdge("a", "e");
    graph.addEdge("b", "d");
    graph.addEdge("a", "b");
    graph.addEdge("a", "c");
    graph.addEdge("c", "e");
    graph.addEdge("b", "e");
    graph.addEdge("d", "e");
    Set<Edge*> edges;
    edges = graph.getEdgeSet();
    cout << "graph: " << graph << endl;
    cout << "edges: " << edges << endl;

    Map<string, Set<string>> m = graph.toMap();
    cout << "map: " << m << endl;

    cout << "getVertexSet: " << graph.getVertexSet() << endl;
    cout << "getEdgeSet  : " << graph.getEdgeSet() << endl;

    Vector<Vertex*> vec;
    for (Vertex* vertex : graph) {
        vec.add(vertex);
    }

    cout << "vector: " << vec << endl;
    cout << endl;

    // arcs ordering
    for (Vertex* v : graph) {
        cout << "arcs      of " << v->name << ": " << v->arcs << endl;
        cout << "neighbors of " << v->name << ": " << graph.getNeighbors(v) << endl;
    }


    Grid<int> g;
    g.resize(4, 3);



    GridLocation loc {2, 1};
    g.set(loc, 42);
    cout << "Grid set: " << g << endl;
    cout << "Grid get: " << g.get(loc) << endl;

    g[loc] = 19;
    cout << "Grid [] set: " << g << endl;
    cout << "Grid [] get: " << g[loc] << endl;

    cout << "Grid inBounds t: " << boolalpha << g.inBounds({0, 0}) << endl;
    cout << "Grid inBounds f: " << boolalpha << g.inBounds({5, -1}) << endl;

    for (const GridLocation& loc : g.locations()) {
        cout << "loc " << loc << ": " << g[loc] << endl;
        for (const GridLocation& neigh : loc.neighbors()) {
            if (neigh == loc || !g.inBounds(neigh)) continue;
            cout << "    neighbor = " << neigh << endl;
        }
    }


//    g.unset(loc);
//    cout << "Grid [] unset: " << g << endl;

    // for (GridLocation loc; loc <= g.)

    for (int i : range(10)) {
        cout << "range i = " << i << endl;
    }

    for (Point p : range2d(2, 3, 6, 5)) {
        cout << "range 2d p = " << p << endl;
    }

    Set<string> set({"hello", "ok", "byebye", "A", "Marty", "beautiful", "Marty", "x", "hello", "hi"}, strLenLess);
//    set.add("hello");
//    set.add("ok");
//    set.add("byebye");
//    set.add("A");
//    set.add("Marty");
//    set.add("beautiful");
//    set.add("Marty");
//    set.add("x");
//    set.add("hello");
//    set.add("hi");

    cout << "set: " << set << endl;

    return 0;
}
