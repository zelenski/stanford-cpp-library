/*
 * Test drawing with colors having an alpha channel
 *
 * @version 2017/03/15
 */

#include "gwindow.h"
#include "gobjects.h"

void alphatest() {
    GWindow gw(500, 500);
    gw.drawLine(100, 0, 500, 500);
    gw.setColor("#0000ff");
    gw.fillRect(100, 100, 100, 100);
    gw.setColor(0xe0ee4411, true);
    gw.fillRect(150, 150, 100, 100);
    gw.setColor("#f811ee44");
    gw.fillOval(180, 180, 100, 100);
    gw.setColor("black");
    gw.drawLine(0, 100, 500, 500);

    // these five rectangles should all be the same color
    gw.setColor("LIGHT_GRAY");
    gw.fillRect(400, 100, 40, 40);
    gw.setColor("#bfbfbf");
    gw.fillRect(400, 150, 40, 40);
    gw.setColor("#ffbfbfbf");
    gw.fillRect(400, 200, 40, 40);
    gw.setColor(0xffbfbfbf);
    gw.fillRect(400, 250, 40, 40);
    gw.setColor(4290756543);
    gw.fillRect(400, 300, 40, 40);

    GOval *oval = new GOval(150, 150);
    oval->setFilled(true);
    oval->setColor("#c0ffff00");
    gw.add(oval, 200, 200);

    GRect *rect = new GRect(60, 60);
    rect->setColor("black");
    rect->setFillColor(oval->getColor());
    rect->setFilled(true);
    gw.add(rect, 320, 320);
}
