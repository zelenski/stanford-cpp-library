/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include <iostream>
#include "gcolor.h"
#include "gdiffimage.h"
#include "gobjects.h"
#include "ginteractors.h"
#include "gtimer.h"
#include "timer.h"
using namespace std;

void testDiffImage();
void testQwindow();

int mainQtWidgets() {
    testQwindow();
    // testDiffImage();
    return 0;
}

void c() {
    Vector<int> v;
    v[5];   // will crash
}

void b() {
    c();
}

void a() {
    b();
}

void testQwindow() {
    static GWindow* window = new GWindow(900, 300);
    window->setTitle("QtGui Window");
    window->setResizable(true);
    window->setExitOnClose(true);
    window->center();

//    window->setColor("red");
//    window->fillRect(0, 0, 900, 300);

    // NORTH AREA

    GLabel* label = new GLabel("Type <b>stuff</b> <i>now</i> (North):");
    // label->setIcon("triangle-icon.png");
    label->setColor(GColor::GREEN);
    // label->setBackground(GColor::YELLOW);
    label->setActionListener([=]() {
        std::cout << "label clicked!" << std::endl;
        label->setBackground(label->getBackground() == "cyan" ? "yellow" : "cyan");
    });
    label->setDoubleClickListener(GEvent::LOG_EVENT);
    window->addToRegion(label, GWindow::REGION_NORTH);
    cout << "label:     " << label->toString() << endl;

    //        static GTextField* textField = new GTextField(42.0);
    static GTextField* textField = new GTextField("Marty");
    textField->setPlaceholder("type your name");
    // textField->setEditable(false);
    textField->setAutocompleteList({"matt", "Marty", "Stuart", "steve", "yana", "yes", "no"});
    textField->setTextChangeListener([]() {
        cout << "textfield text changed! text is:" << endl << textField->getText() << endl;
    });
    textField->setActionListener([]() {
        cout << "textfield action performed! text is:" << endl << textField->getText() << endl;
    });
    window->addToRegion(textField, GWindow::REGION_NORTH);
    cout << "textfield: " << textField->toString() << endl;

    static GSlider* slider = new GSlider();
    slider->setMinorTickSpacing(20);
    slider->setPaintLabels(true);
    slider->setPaintTicks(true);
    slider->setActionListener([](GEvent event) {
        cout << "sliderChangeHandler: slider was slid!" << endl;
        cout << "value: " << slider->getValue() << endl;
        cout << "event: " << event << endl;
        window->removeTimerListener();
    });

    window->addToRegion(slider, GWindow::REGION_NORTH);
    cout << "slider:    " << slider->toString() << endl;


    // WEST AREA

    static GCheckBox* checkBox = new GCheckBox("Question?", true);
    checkBox->setActionListener([](const GEvent&) {
        cout << "checkbox clicked! " << boolalpha << checkBox->isChecked() << endl;
    });
    window->addToRegion(checkBox, GWindow::REGION_WEST);
    window->addToRegion(new GLabel("Hi!"), GWindow::REGION_WEST);
    window->addToRegion(new GLabel("^_^"), GWindow::REGION_WEST);
    // window->setRegionAlignment(GWindow::REGION_WEST, "Top Right");
    cout << "checkbox:  " << checkBox->toString() << endl;


    // EAST AREA

    static GRadioButton* radio1group1 = new GRadioButton("A", "group1");
    static GRadioButton* radio2group1 = new GRadioButton("B", "group1", true);
    static GRadioButton* radio3group1 = new GRadioButton("C", "group1");
    static GRadioButton* radio1group2 = new GRadioButton("XX", "group2", true);
    static GRadioButton* radio2group2 = new GRadioButton("YY", "group2");

    GEventListenerVoid radioChangeHandler = []() {
        cout << "checkbox clicked! " << boolalpha
             << radio1group1->isChecked() << " "
             << radio2group1->isChecked() << " "
             << radio3group1->isChecked() << " "
             << radio1group2->isChecked() << " "
             << radio2group2->isChecked() << endl;
    };
    radio1group1->setActionListener(radioChangeHandler);
    radio1group1->setDoubleClickListener(GEvent::LOG_EVENT);
    radio2group1->setActionListener(radioChangeHandler);
    radio2group1->setDoubleClickListener(GEvent::LOG_EVENT);
    radio3group1->setActionListener(radioChangeHandler);
    radio3group1->setDoubleClickListener(GEvent::LOG_EVENT);
    radio1group2->setActionListener(radioChangeHandler);
    radio2group2->setActionListener(radioChangeHandler);

//    static QGScrollBar* scrollBar = new QGScrollBar(QGScrollBar::VERTICAL, 0, 10, 0, 500);
//    scrollBar->setValueChangeHandler([]() {
//        cout << "value: " << scrollBar->getValue() << endl;
//    });
//    window->addToRegion(scrollBar, GWindow::REGION_EAST);

    window->addToRegion(radio1group1, GWindow::REGION_EAST);
    window->addToRegion(radio2group1, GWindow::REGION_EAST);
    window->addToRegion(radio3group1, GWindow::REGION_EAST);
    window->addToRegion(radio1group2, GWindow::REGION_EAST);
    window->addToRegion(radio2group2, GWindow::REGION_EAST);
    // window->setRegionAlignment(GWindow::REGION_EAST, "Bottom Left");
//    cout << "radio:     " << radio1group1->toString() << endl;


    // SOUTH AREA

    static GChooser* chooser = new GChooser({"one", "two", "three four"});
    chooser->setColor(GColor::RED);
    chooser->setBackground(GColor::CYAN);
    chooser->setActionListener([]() {
        cout << "changeHandler: chooser was clicked!" << endl;
        cout << "selected: " << chooser->getSelectedIndex() << " : "
             << chooser->getSelectedItem() << endl;
        cout << "size: " << chooser->size() << endl << endl;
    });
    window->addToRegion(chooser, GWindow::REGION_SOUTH);
    cout << "chooser:   " << chooser->toString() << endl;

    GButton* button = new GButton("Triforce");
    button->setColor(GColor::RED);
    button->setBackground(GColor::YELLOW);
    button->setIcon("triangle-icon.png");
    button->setTextPosition(GInteractor::TEXT_BESIDE_ICON);
    button->setActionListener([](GEvent event) {
        cout << "button click! event = " << event << endl;
        cout.flush();
        window->restore();

        a();

//        window->setResizable(!window->isResizable());
//        cout << "clickHandler: button was clicked!" << endl;
//        cout << "location:  " << window->getLocation() << endl;
//        cout << "size:      " << window->getSize() << endl;
//        cout << "visible:   " << boolalpha << window->isVisible() << endl;
//        cout << "resizable: " << boolalpha << window->isResizable() << endl << endl;

//        // test GOptionPane
//        GOptionPane::showMessageDialog("I love Yana! <3");

//        Vector<string> choices = {"One", "Two", "Three"};
//        string result = GOptionPane::showOptionDialog("Pick a thing", choices);
//        cout << "You chose: " << result << endl;

        //    int result = GOptionPane::showConfirmDialog("Is Yana the most beautiful?", "Important Question", GOptionPane::YES_NO_CANCEL);
        //    cout << "You chose: " << result << endl;
        //    std::string answer = GOptionPane::showInputDialog("Who is my baby?", "Baby's name?", "bozo");
        //    cout << "You typed: " << answer << endl;

        //    string filename = QGFileChooser::showOpenDialog("", "*.txt, *.cpp, *.h");
        //    cout << "You chose: " << filename << endl;
        // window->clear();
    });

    //        button->setClickHandler([]() {
    //            // grayscale(image);
    //        });
    button->setDoubleClickListener([](GEvent event) {
        cout << "button double-click! event = " << event << endl;
    });
    button->setAccelerator("Ctrl-T");
    window->addToRegion(button, GWindow::REGION_SOUTH);
    cout << "button:    " << button->toString() << endl;
    cout << "button accelerator: " << button->getAccelerator() << endl;
    cout << "button font: " << button->getFont() << endl;
    button->setFont("Monospaced-Bold-14");

    static GButton* button4 = new GButton("HI!");
    window->addToRegion(button4, GWindow::REGION_SOUTH);

    static GCheckBox* checkboxs = new GCheckBox("&Visible?", /* checked */ true);
    checkboxs->setActionListener([]() {
        std::cout << "checkbox clicked!" << std::endl;
        // button4->setVisible(checkboxs->isChecked());
        if (checkboxs->isChecked()) {
            window->addToRegion(button4, GWindow::REGION_SOUTH);
        } else {
            window->removeFromRegion(button4, GWindow::REGION_SOUTH);
        }
    });
    checkboxs->setDoubleClickListener([]() {
        std::cout << "checkbox double-clicked!" << std::endl;
    });
    window->addToRegion(checkboxs, GWindow::REGION_SOUTH);

    GButton* timerButton = new GButton("Timer");
    GTimer* timer = nullptr;
    timerButton->setActionListener([&timer]() {
        if (timer) {
            timer->stop();
            delete timer;
            timer = nullptr;
        } else {
            timer = new GTimer(1000);
            timer->start();
        }
    });
    window->addToRegion(timerButton, GWindow::REGION_SOUTH);

//    window->setRegionAlignment(GWindow::REGION_SOUTH, "Center");

    // GLabel* oopsButton = new GLabel("I should not show up!!!!!");
    // oopsButton->setVisible(true);


    // CENTER AREA

//    static GTextArea* textArea = new GTextArea("This is \na multi-line\n\ntext area");
//    textArea->setPlaceholder("type some text");
//    textArea->setTextChangeListener([](GEvent) {
//        cout << "textarea text changed! text is:" << endl << textArea->getText() << endl;
//    });
//    window->addToRegion(textArea, "Center");

    GBrowserPane* pane = new GBrowserPane();
    pane->readTextFromFile("resfile3.html");
    pane->setLinkListener([](GEvent event) {
        cout << "event: " << event << ", url: " << event.getRequestURL() << endl;
    });
    window->addToRegion(pane, GWindow::REGION_CENTER);
    cout << "browser:  " << pane->toString() << endl;

    // window->pack();

//    while (true) {
//        GEvent event = waitForEvent(TIMER_EVENT);
//        cout << "event: " << event << endl;
//    }

    // will crash
    // a();

//    int* x = nullptr;
//    (*x)++;   // boom
}

void testDiffImage() {
    GDiffImage::showDialog("ponies1", "my-little-pony-small.png", "legos2", "lego.png");
}
