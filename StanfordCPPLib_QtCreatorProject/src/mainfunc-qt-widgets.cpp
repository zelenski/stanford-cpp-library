/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include <iostream>
#include "gcolor.h"
#include "gobjects.h"
#include "ginteractors.h"
#include "gtimer.h"
#include "timer.h"
using namespace std;

void testQwindow();

int mainQtWidgets() {
    testQwindow();
    return 0;
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
    window->addToRegion(label, "North");
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
    window->addToRegion(textField, "North");
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

    window->addToRegion(slider, "North");
    cout << "slider:    " << slider->toString() << endl;


    // WEST AREA

    static GCheckBox* checkBox = new GCheckBox("Question?", true);
    checkBox->setActionListener([](const GEvent&) {
        cout << "checkbox clicked! " << boolalpha << checkBox->isChecked() << endl;
    });
    window->addToRegion(checkBox, "West");
    window->addToRegion(new GLabel("Hi!"), "West");
    window->addToRegion(new GLabel("^_^"), "West");
    // window->setRegionAlignment("West", "Top Right");
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
//    window->addToRegion(scrollBar, "East");

    window->addToRegion(radio1group1, "East");
    window->addToRegion(radio2group1, "East");
    window->addToRegion(radio3group1, "East");
    window->addToRegion(radio1group2, "East");
    window->addToRegion(radio2group2, "East");
    // window->setRegionAlignment("East", "Bottom Left");
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
    window->addToRegion(chooser, "South");
    cout << "chooser:   " << chooser->toString() << endl;

    GButton* button = new GButton("Triforce");
    button->setColor(GColor::RED);
    button->setBackground(GColor::YELLOW);
    button->setIcon("triangle-icon.png");
    button->setTextPosition(GInteractor::TEXT_BESIDE_ICON);
    button->setActionListener([](GEvent event) {
        cout << "button click! event = " << event << endl;

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
    window->addToRegion(button, "South");
    cout << "button:    " << button->toString() << endl;
    cout << "button accelerator: " << button->getAccelerator() << endl;
    cout << "button font: " << button->getFont() << endl;
    button->setFont("Monospaced-Bold-14");

    static GButton* button4 = new GButton("HI!");
    window->addToRegion(button4, "South");

    static GCheckBox* checkboxs = new GCheckBox("&Visible?", /* checked */ true);
    checkboxs->setActionListener([]() {
        std::cout << "checkbox clicked!" << std::endl;
        // button4->setVisible(checkboxs->isChecked());
        if (checkboxs->isChecked()) {
            window->addToRegion(button4, "South");
        } else {
            window->removeFromRegion(button4, "South");
        }
    });
    checkboxs->setDoubleClickListener([]() {
        std::cout << "checkbox double-clicked!" << std::endl;
    });
    window->addToRegion(checkboxs, "South");

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
    window->addToRegion(timerButton, "South");

//    window->setRegionAlignment("South", "Center");

    // GLabel* oopsButton = new GLabel("I should not show up!!!!!");
    // oopsButton->setVisible(true);


    // CENTER AREA

    static GTextArea* textArea = new GTextArea("This is \na multi-line\n\ntext area");
    textArea->setRowsColumns(4, 30);
    textArea->setPlaceholder("type some text");
    textArea->setTextChangeListener([](GEvent) {
        cout << "textarea text changed! text is:" << endl << textArea->getText() << endl;
    });
    window->addToRegion(textArea, "Center");
    cout << "textarea:  " << textArea->toString() << endl;


    // window->pack();

//    while (true) {
//        GEvent event = waitForEvent(TIMER_EVENT);
//        cout << "event: " << event << endl;
//    }
}
