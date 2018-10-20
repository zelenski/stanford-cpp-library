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

int multimain(diffImage) {
    testDiffImage();
    return 0;
}

void testDiffImage() {
    GDiffImage::showDialog("bender1", "bender.png", "bender2", "bender2.png");
}
