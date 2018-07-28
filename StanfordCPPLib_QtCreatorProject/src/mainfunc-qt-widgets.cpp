///*
// * Test file for verifying the Stanford C++ lib functionality.
// */

//#include <iostream>
//#include "qgcolor.h"
//#include "qgobjects.h"
//#include "qginteractors.h"
//#include "qgui.h"
//#include "timer.h"
//using namespace std;

//void testQwindow();

//int mainQtWidgets() {
//    testQwindow();
//    return 0;
//}

//void testQwindow() {
//    QGui::instance()->runOnQtGuiThread([]() {
//        static QGWindow* window = new QGWindow(700, 300);
//        window->setTitle("QtGui Window");
//        window->setResizable(true);
//        window->setExitOnClose(false);
//        window->center();

//        QGLabel* label = new QGLabel("Type <b>stuff</b> <i>now</i> (North):");
//        label->setIcon("triangle-icon.png");
//        label->setColor(QGColor::GREEN);
//        label->setBackground(QGColor::YELLOW);
//        window->addToRegion(label, "North");
//        cout << "label:     " << label->toString() << endl;

//        static QGChooser* chooser = new QGChooser({"one", "two", "three four"});
//        chooser->setColor(QGColor::RED);
//        chooser->setBackground(QGColor::YELLOW);
//        chooser->setActionListener([]() {
//            cout << "changeHandler: chooser was clicked!" << endl;
//            cout << "selected: " << chooser->getSelectedIndex() << " : "
//                 << chooser->getSelectedItem() << endl;
//            cout << "size: " << chooser->size() << endl << endl;
//        });

//        window->addToRegion(chooser, "South");
//        cout << "chooser:   " << chooser->toString() << endl;

//        static QGCheckBox* checkBox = new QGCheckBox("Question?", true);
//        checkBox->setActionListener([](const QGEvent&) {
//            cout << "checkbox clicked! " << boolalpha << checkBox->isChecked() << endl;
//        });
//        window->addToRegion(checkBox, "West");
//        window->addToRegion(new QGLabel("Hi!"), "West");
//        window->addToRegion(new QGLabel("^_^"), "West");
//        window->setRegionAlignment("West", "Top Right");
//        cout << "checkbox:  " << checkBox->toString() << endl;

//        static QGRadioButton* radio1group1 = new QGRadioButton("A", "group1");
//        static QGRadioButton* radio2group1 = new QGRadioButton("B", "group1", true);
//        static QGRadioButton* radio3group1 = new QGRadioButton("C", "group1");
//        static QGRadioButton* radio1group2 = new QGRadioButton("XX", "group2", true);
//        static QGRadioButton* radio2group2 = new QGRadioButton("YY", "group2");

//        QGEventListenerVoid radioChangeHandler = []() {
//            cout << "checkbox clicked! " << boolalpha
//                 << radio1group1->isChecked() << " "
//                 << radio2group1->isChecked() << " "
//                 << radio3group1->isChecked() << " "
//                 << radio1group2->isChecked() << " "
//                 << radio2group2->isChecked() << endl;
//        };
//        radio1group1->setActionListener(radioChangeHandler);
//        radio2group1->setActionListener(radioChangeHandler);
//        radio3group1->setActionListener(radioChangeHandler);
//        radio1group2->setActionListener(radioChangeHandler);
//        radio2group2->setActionListener(radioChangeHandler);

////        static QGScrollBar* scrollBar = new QGScrollBar(QGScrollBar::VERTICAL, 0, 10, 0, 500);
////        scrollBar->setValueChangeHandler([]() {
////            cout << "value: " << scrollBar->getValue() << endl;
////        });
////        window->addToRegion(scrollBar, "East");

////        window->addToRegion(radio1group1, "East");
////        window->addToRegion(radio2group1, "East");
////        window->addToRegion(radio3group1, "East");
////        window->addToRegion(radio1group2, "East");
////        window->addToRegion(radio2group2, "East");
////        window->setRegionAlignment("East", "Bottom Right");
////        cout << "radio:     " << radio1group1->toString() << endl;

//        //        static QGTextField* textField = new QGTextField(42.0);
//        static QGTextField* textField = new QGTextField("Marty");
//        textField->setPlaceholder("type your name");
//        // textField->setEditable(false);
//        textField->setAutocompleteList({"matt", "Marty", "Stuart", "steve", "yana", "yes", "no"});
//        textField->setTextChangeListener([]() {
//            cout << "textfield text changed! text is:" << endl << textField->getText() << endl;
//        });
//        textField->setActionListener([]() {
//            cout << "textfield action performed! text is:" << endl << textField->getText() << endl;
//        });
//        window->addToRegion(textField, "North");
//        cout << "textfield: " << textField->toString() << endl;


////        static QGTextArea* textArea = new QGTextArea("This is \na multi-line\n\ntext area");
////        textArea->setRowsColumns(4, 30);
////        textArea->setPlaceholder("type some text");
////        textArea->setTextChangeHandler([](const QGEvent&) {
////            cout << "textarea text changed! text is:" << endl << textArea->getText() << endl;
////        });
////        window->addToRegion(textArea, "West");
////        cout << "textarea:  " << textArea->toString() << endl;

//        // TODO: debug crash
////        static QGBufferedImage* image = new QGBufferedImage("lego.png");
////        window->addToRegion(image, "East");

//        QGButton* button = new QGButton("Triforce");
//        button->setColor(QGColor::RED);
//        button->setBackground(QGColor::YELLOW);
//        button->setIcon("triangle-icon.png");
//        button->setTextPosition(QGInteractor::TEXT_BESIDE_ICON);
//        button->setActionListener([](QGEvent event) {
//            cout << "button click! event = " << event << endl;

//            window->setResizable(!window->isResizable());
//            cout << "clickHandler: button was clicked!" << endl;
//            cout << "location:  " << window->getLocation() << endl;
//            cout << "size:      " << window->getSize() << endl;
//            cout << "visible:   " << boolalpha << window->isVisible() << endl;
//            cout << "resizable: " << boolalpha << window->isResizable() << endl << endl;

//            // test QGOptionPane
//            QGOptionPane::showMessageDialog("I love Yana! <3");

//            Vector<string> choices = {"One", "Two", "Three"};
//            string result = QGOptionPane::showOptionDialog("Pick a thing", choices);
//            cout << "You chose: " << result << endl;

//            //    int result = QGOptionPane::showConfirmDialog("Is Yana the most beautiful?", "Important Question", QGOptionPane::YES_NO_CANCEL);
//            //    cout << "You chose: " << result << endl;
//            //    std::string answer = QGOptionPane::showInputDialog("Who is my baby?", "Baby's name?", "bozo");
//            //    cout << "You typed: " << answer << endl;

//            //    string filename = QGFileChooser::showOpenDialog("", "*.txt, *.cpp, *.h");
//            //    cout << "You chose: " << filename << endl;
//            // window->clear();
//        });
////        button->setClickHandler([]() {
////            // grayscale(image);
////        });
//        button->setAccelerator("Ctrl-T");
//        window->addToRegion(button, "South");
//        cout << "button:    " << button->toString() << endl;
//        cout << "button accelerator: " << button->getAccelerator() << endl;
//        cout << "button font: " << button->getFont() << endl;
//        button->setFont("Monospaced-Bold-14");

//        int rows = 4;
//        int cols = 5;
//        static QGTable* table = new QGTable(rows, cols);
//        table->setColumnHeaderStyle(QGTable::COLUMN_HEADER_EXCEL);
//        for (int r = 0; r < rows; r++) {
//            for (int c = 0; c < cols; c++) {
//                table->set(r, c, "hello");
//            }
//        }

//        // styles
//        // 1) global
//        table->setColor("magenta");

//        // 2) row
//        table->setRowBackground(1, "green");
//        table->setRowFont(1, "Monospaced-12-Bold");
//        table->setRowForeground(3, "Blue");

//        // 3) column
//        table->setColumnAlignment(1, qgenum::ALIGN_RIGHT);
//        table->setColumnBackground(1, "Cyan");
//        table->setColumnFont(2, "*-*-Italic");
//        table->setColumnForeground(3, "Red");

//        // 4) cell
//        table->setCellForeground(0, 0, "Red");
//        table->setCellAlignment(1, 1, qgenum::ALIGN_CENTER);
//        table->setCellBackground(2, 2, "Blue");

//        // 1) global (again) - should override 2)-4)
//        table->setBackground("yellow");

//        // 2) row (again) - should set cell (1,1) to bold, not italic
//        // table->setRowFont(1, "Monospaced-12-Bold");

//        table->setTableListener([](QGEvent event) {
//            cout << "table event: " << event.toString() << endl;
//        });
//        window->addToRegion(table, "Center");
//        // window->pack();

//        QGButton* button2 = new QGButton("Resize");
//        button2->setActionListener([]() {
//            table->resize(table->numRows() + 1, table->numCols() + 1);
//        });
//        window->addToRegion(button2, "South");

//        QGButton* button3 = new QGButton("ClearF");
//        button3->setActionListener([]() {
//            // table->clearCellFormatting(1, 1);
//            table->clearFormatting();
//        });
//        window->addToRegion(button3, "South");

//        static QGSlider* slider = new QGSlider();
//        slider->setMinorTickSpacing(20);
//        slider->setPaintLabels(true);
//        slider->setPaintTicks(true);
//        slider->setActionListener([](QGEvent event) {
//            cout << "sliderChangeHandler: slider was slid!" << endl;
//            cout << "value: " << slider->getValue() << endl;
//            cout << "event: " << event << endl;
//            window->removeTimerListener();
//        });

//        window->addToRegion(slider, "North");
//        cout << "slider:    " << slider->toString() << endl;

//        // window->pause(500);

//        // todo

//        window->setWindowListener([](QGEvent event) {
//            // cout << "window! event=" << event << endl;
//            if (event.getEventType() == QGEvent::WINDOW_CLOSED) {
//                // cout << "window closed!" << endl;
//                QGui::instance()->exitGraphics();
//            }
//        });

////        window->setTimerHandler(1000, [](QGEvent event) {
////            cout << "timer! event=" << event << endl;
////        });

////        static QGBrowserPane* browserPane = new QGBrowserPane("resfile3.html");
////        window->addToRegion(browserPane, "Center");

//        // window->setCanvasSize(900, 500);

//        window->setMouseListener([](QGEvent event) {
//            if (event.getType() == QGEvent::MOUSE_MOVED) {
//                // cout << "mouse moved: " << event.getLocation().toString() << endl;
//            }
//        });

//    });
//}
