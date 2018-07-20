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

//void testQwindowDrawing();

//int mainQt2dGraphics() {
//    testQwindowDrawing();
//    return 0;
//}

//void grayscale(QGBufferedImage* image) {
//    // convert image to grayscale
//    image->setAutoRepaint(false);
//    Timer tim;
//    tim.start();
//    for (int x = 0; x < image->getWidth(); x++) {
//        for (int y = 0; y < image->getHeight(); y++) {
//            int rgb = image->getRGB(x, y);
//            int r = QGBufferedImage::getRed(rgb);
//            int g = QGBufferedImage::getGreen(rgb);
//            int b = QGBufferedImage::getBlue(rgb);
//            int avg = (r + g + b) / 3;
//            rgb = QGBufferedImage::createRgbPixel(avg, avg, avg);
//            image->setRGB(x, y, rgb);
//        }
//    }
//    image->repaint();
//    tim.stop();
//    cout << "took " << tim.elapsed() << "ms" << endl;
//}

//void testQwindowDrawing() {
//    static QGWindow* window = nullptr;
//    static QGOval* ball = nullptr;
//    static QGButton* button = nullptr;
//    static QGButton* button2 = nullptr;
//    static QGButton* button3 = nullptr;
//    static QGButton* button4 = nullptr;
//    static QGButton* button5 = nullptr;
//    static QGButton* saveButton = nullptr;
//    static QGLabel* label = nullptr;
//    static QGLabel* label2 = nullptr;
//    static double dx = 5;
//    static double dy = 3;

//    QGui::instance()->runOnQtGuiThread([]() {
//        window = new QGWindow(900, 500);
//        window->setTitle("QtGui Drawing Window");
//        // window->setResizable(false);
//        // window->setBackground("yellow");
//        window->center();

//        label = new QGLabel("QtGui <b>AWESOME</b> <i>cool</i> window");
//        label2 = new QGLabel("????????????");
//        window->addToRegion(label2, "North");
//        window->addToRegion(label, "North");

//        ball = new QGOval(20, 20, 50, 50);
//        ball->setFillColor("red");
//        ball->setLineWidth(2);
//        ball->setLineStyle(QGObject::LINE_DOT);
//        window->add(ball);

//        window->setRegionAlignment("North", "Left");
//        window->setRegionAlignment("South", "Right");

//        button = new QGButton("Tick");
//        button2 = new QGButton("Pixels");
//        button3 = new QGButton("Repaint");
//        button4 = new QGButton("Clear");
//        button5 = new QGButton("BG");
//        saveButton = new QGButton("Save");

//        window->addToRegion(button, "South");
//        window->addToRegion(button2, "South");
//        window->addToRegion(button3, "South");
//        window->addToRegion(button4, "South");
//        window->addToRegion(button5, "South");
//        window->addToRegion(saveButton, "South");

//        window->setCanvasSize(900, 500);


////        window->setBackground("yellow");
////        window->setColor("red");
////        window->setFillColor("red");
//    });

//    // drawing directly onto window
////    window->setColor("blue");
////    window->setFillColor("yellow");
////    window->fillOval(20, 120, 40, 60);
////    window->setColor("red");
////    window->setFillColor("green");
////    window->setLineWidth(3);
////    window->fillRect(10, 30, 120, 70);
////    window->drawLine(100, 100, 200, 150);

//    // pixely stuff

//    window->setColor("black");
//    window->setFillColor("red");
//    window->setLineWidth(5);   // BUG: affects later shapes
//    window->setLineStyle(QGObject::LINE_DASH);
//    window->drawRect(20, 220, 100, 100);
//    window->fillArc(20, 220, 100, 100, 45, 120);

//    window->setFillColor("purple");
//    window->setFont("Monospaced-bold-16");
//    window->fillPolygon({200, 200, 250, 300, 150, 300});

//    window->setColor("green");
//    QGString* qgstring = new QGString("Hello, Qt!", 200, 80);
//    qgstring->setColor("green");
//    qgstring->rotate(10);
//    window->add(qgstring);

//    QGString* qgstring2 = new QGString("Bye, Felicia!", 220, 100);
//    qgstring2->setColor("red");
//    qgstring2->scale(1.5);
//    window->add(qgstring2);

//    QGImage* qgimage = new QGImage("triangle-icon.png", 200, 40);
//    window->add(qgimage);

//    window->setColor("blue");
//    QGString* qgstring3 = new QGString("Third string", 240, 120);
//    qgstring3->setColor("blue");
//    window->add(qgstring3);

////    // animation loop
////    cout << "testQwindowDrawing: What thread am I? " << QGui::instance()->getCurrentThread() << endl;
////    cout << "testQwindowDrawing: Qt gui main thread = " << QGui::instance()->getQtMainThread()
////         << ", student thread = " << QGui::instance()->getStudentThread() << endl;

//    std::function<void()> tickFunc = []() {
////        cout << "tick func: What thread am I? " << QGui::instance()->getCurrentThread() << endl;
////        cout << "tick func: dx=" << dx << ",dy=" << dy << endl;
////        cout << "tick func: ball = " << ball->toString() << endl;
//        window->pause(20);
//        ball->move(dx, dy);
//        double cw = window->getCanvasWidth();
//        double ch = window->getCanvasHeight();
//        if (ball->getX() < 0 || ball->getRightX() >= cw) {
//            dx = -dx;
//        }
//        if (ball->getY() < 0 || ball->getBottomY() >= ch) {
//            dy = -dy;
//        }
//        // cout << "ball loc is now: " << ball->getLocation() << endl;
//    };
//    button->setClickHandler(tickFunc);

//    button2->setClickHandler([]() {
//        for (int y = 3*window->getCanvasHeight()/4; y < window->getCanvasHeight(); y++) {
//            for (int x = 3*window->getCanvasWidth()/4; x < window->getCanvasWidth(); x++) {
//                int r = (x * 2) % 256;
//                int g = (y * 2) % 256;
//                int b = ((x+y) * 2) % 256;
//                window->setPixel(x, y, r, g, b);
//            }
//        }
//    });

//    button3->setClickHandler([]() {
//        window->repaint();
//    });

//    button4->setClickHandler([]() {
//        window->clearCanvasPixels();
//    });

//    button5->setClickHandler([]() {
//        window->setBackground("yellow");
//    });

//    saveButton->setClickHandler([]() {
//        string filename = QGFileChooser::showSaveDialog();
//        window->saveCanvasPixels(filename);
//    });

//    window->setMouseHandler([](QGEvent event) {
//        // cout << "mouse! event=" << event << endl;
//        // cout << "mouse handler: What thread am I? " << QGui::instance()->getCurrentThread() << endl;
//        if (event.getType() == QGEvent::MOUSE_DRAGGED) {
//            window->setColor("blue");
//            window->setFillColor("blue");
//            window->setLineStyle(QGObject::LINE_SOLID);
//            window->setLineWidth(1);
//            window->fillOval(event.getX() - 5, event.getY() - 5, 10, 10);
//        } else if (event.getType() == QGEvent::MOUSE_MOVED) {
//            label->setText(event.getLocation().toString());
//            cout << "mouse moved: " << event.getLocation().toString() << endl;
//        }
//    });

//    window->setKeyHandler([](QGEvent event) {
//        // cout << "key! event=" << event << endl;
//        double balldx = 0;
//        double balldy = 0;
//        if (event.getKeyCode() == QGEvent::LEFT_ARROW_KEY) {
//            balldx = -2;
//        } else if (event.getKeyCode() == QGEvent::RIGHT_ARROW_KEY) {
//            balldx = 2;
//        } else if (event.getKeyCode() == QGEvent::UP_ARROW_KEY) {
//            balldy = -2;
//        } else if (event.getKeyCode() == QGEvent::DOWN_ARROW_KEY) {
//            balldy = 2;
//        } else if (event.getKeyCode() == 'f') {
//            ball->sendToFront();
//        }
//        ball->move(balldx, balldy);
//    });

////    window->setWindowHandler([](QGEvent /* event */) {
////        // cout << "window! event=" << event << endl;
////    });

////    for (int i = 0; i < 1000; i++) {
////        tickFunc();
////    }
//}
