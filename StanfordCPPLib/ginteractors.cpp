/*
 * File: gobjects.cpp
 * ------------------
 * This file implements the ginteractors.h interface.
 */

#include <iostream>
#include <sstream>
#include "gevents.h"
#include "ginteractors.h"
#include "gobjects.h"
#include "gtypes.h"
#include "gwindow.h"
#include "platform.h"

static Platform *pp = getPlatform();

/*
 * Implementation notes: GInteractor class
 * ---------------------------------------
 */

GInteractor::GInteractor() {
    actionCommand = "";
}

void GInteractor::setActionCommand(string cmd) {
    actionCommand = cmd;
    pp->ginteractor_setActionCommand(this, cmd);
}

string GInteractor::getActionCommand() {
    return actionCommand;
}

void GInteractor::setSize(const GDimension & size) {
    setSize(size.getWidth(), size.getHeight());
}

void GInteractor::setSize(double width, double height) {
    pp->gobject_setSize(this, width, height);
}

void GInteractor::setBounds(const GRectangle & rect) {
    setLocation(rect.getX(), rect.getY());
    setSize(rect.getWidth(), rect.getHeight());
}

void GInteractor::setBounds(double x, double y, double width, double height) {
    setLocation(x, y);
    setSize(width, height);
}

GRectangle GInteractor::getBounds() const {
    GDimension size = pp->ginteractor_getSize((GObject *) this);
    return GRectangle(x, y, size.getWidth(), size.getHeight());
}

bool GInteractor::isEnabled() {
    return pp->ginteractor_isEnabled(this);
}

void GInteractor::setEnabled(bool value) {
    pp->ginteractor_setEnabled(this, value);
}

/*
 * Implementation notes: GButton class
 * -----------------------------------
 */

GButton::GButton(string label) {
    this->label = label;
    pp->gbutton_constructor(this, label);
}

string GButton::getType() const {
    return "GButton";
}

string GButton::toString() const {
    ostringstream oss;
    oss << "GButton(\"" << label << "\")";
    return oss.str();
}

/*
 * Implementation notes: GCheckBox class
 * -------------------------------------
 */

GCheckBox::GCheckBox(string label) {
    this->label = label;
    pp->gcheckbox_constructor(this, label);
}

bool GCheckBox::isSelected() {
    return pp->gcheckbox_isSelected(this);
}

void GCheckBox::setSelected(bool state) {
    pp->gcheckbox_setSelected(this, state);
}

string GCheckBox::getType() const {
    return "GCheckBox";
}

string GCheckBox::toString() const {
    ostringstream oss;
    oss << "GCheckBox(\"" << label << "\")";
    return oss.str();
}

/*
 * Implementation notes: GSlider class
 * -----------------------------------
 */

GSlider::GSlider() {
    create(0, 100, 50);
}

GSlider::GSlider(int min, int max, int value) {
    create(min, max, value);
}

int GSlider::getMajorTickSpacing() const {
    return pp->gslider_getMajorTickSpacing(this);
}

int GSlider::getMinorTickSpacing() const {
    return pp->gslider_getMinorTickSpacing(this);
}

bool GSlider::getPaintLabels() const {
    return pp->gslider_getPaintLabels(this);
}

bool GSlider::getPaintTicks() const {
    return pp->gslider_getPaintTicks(this);
}

bool GSlider::getSnapToTicks() const {
    return pp->gslider_getSnapToTicks(this);
}

string GSlider::getType() const {
    return "GSlider";
}

int GSlider::getValue() {
    return pp->gslider_getValue(this);
}

void GSlider::setMajorTickSpacing(int value) {
    pp->gslider_setMajorTickSpacing(this, value);
}

void GSlider::setMinorTickSpacing(int value) {
    pp->gslider_setMinorTickSpacing(this, value);
}

void GSlider::setPaintLabels(bool value) {
    pp->gslider_setPaintLabels(this, value);
}

void GSlider::setPaintTicks(bool value) {
    pp->gslider_setPaintTicks(this, value);
}

void GSlider::setSnapToTicks(bool value) {
    pp->gslider_setSnapToTicks(this, value);
}

void GSlider::setValue(int value) {
    pp->gslider_setValue(this, value);
}

string GSlider::toString() const {
    ostringstream oss;
    oss << "GSlider()";
    return oss.str();
}

void GSlider::create(int min, int max, int value) {
    this->min = min;
    this->max = max;
    pp->gslider_constructor(this, min, max, value);
}

/*
 * Implementation notes: GTextField class
 * --------------------------------------
 */

GTextField::GTextField() {
    pp->gtextfield_constructor(this, 10);
}

GTextField::GTextField(int nChars) {
    pp->gtextfield_constructor(this, nChars);
}

string GTextField::getText() {
    return pp->gtextfield_getText(this);
}

bool GTextField::isEditable() const {
    return pp->gtextfield_isEditable(this);
}

void GTextField::setEditable(bool value) {
    pp->gtextfield_setEditable(this, value);
}

void GTextField::setText(string str) {
    pp->gtextfield_setText(this, str);
}

string GTextField::getType() const {
    return "GTextField";
}

string GTextField::toString() const {
    ostringstream oss;
    oss << "GTextField()";
    return oss.str();
}

/*
 * Implementation notes: GChooser class
 * ------------------------------------
 */

GChooser::GChooser() {
    pp->gchooser_constructor(this);
}

void GChooser::addItem(string item) {
    pp->gchooser_addItem(this, item);
}

string GChooser::getSelectedItem() {
    return pp->gchooser_getSelectedItem(this);
}

void GChooser::setSelectedItem(string item) {
    pp->gchooser_setSelectedItem(this, item);
}

string GChooser::getType() const {
    return "GChooser";
}

string GChooser::toString() const {
    ostringstream oss;
    oss << "GChooser()";
    return oss.str();
}
