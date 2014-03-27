/*
 * File: HelloWorld1.cpp
 * ---------------------
 * This program draws the message "hello, world" on top of a
 * magenta oval in the center of the graphics window.  This
 * version uses the simple gwindow.h model that draws on a
 * persistent canvas.
 */

using namespace std;
#include "gwindow.h"

int main() {
   initGraphics();
   double xc = getWidth() / 2;
   double yc = getHeight() / 2;
   string msg = "hello, world";
   setColor("MAGENTA");
   fillOval(xc - 100, yc - 50, 200, 100);
   setColor("BLACK");
   setFont("Helvetica-24");
   drawString(msg, xc - getStringWidth(msg) / 2, yc + 0.40 * 24);
   return 0;
}
