/*
 * File: HelloWorld2.cpp
 * ---------------------
 * This program draws the message "hello, world" on top of a
 * magenta oval in the center of the graphics window.  This
 * version uses the GObject model.
 */

using namespace std;
#include "graphics.h"
#include "gobjects.h"

int main() {
   initGraphics();
   double xc = getWindowWidth() / 2;
   double yc = getWindowHeight() / 2;
   GOval oval(xc - 100, yc - 50, 200, 100);
   oval.setFilled(true);
   oval.setColor("MAGENTA");
   add(oval);
   GLabel msg = GLabel("hello, world");
   msg.setFont("Helvetica-24");
   add(msg, xc - msg.getWidth() / 2, yc + 0.40 * 24);
   return 0;
}
