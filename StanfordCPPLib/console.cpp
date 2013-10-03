/*
 * File: console.cpp
 * -----------------
 * This file implements the console.h interface.
 */

#include <string>
#include "console.h"
#include "platform.h"
using namespace std;

static Platform *pp = getPlatform();

void clearConsole() {
   pp->clearConsole();
}

void setConsoleFont(const string & font) {
   pp->setConsoleFont(font);
}

void setConsoleSize(double width, double height) {
   pp->setConsoleSize(width, height);
}
