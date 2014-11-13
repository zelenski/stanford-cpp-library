/*
 * File: gobjects.cpp
 * ------------------
 * This file implements the ginteractors.h interface.
 * 
 * @version 2014/10/31
 * - added get/setIcon to GInteractor
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include "ginteractors.h"
#include <iostream>
#include <sstream>
#include "gevents.h"
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

void GInteractor::setActionCommand(std::string cmd) {
    actionCommand = cmd;
    pp->ginteractor_setActionCommand(this, cmd);
}

std::string GInteractor::getActionCommand() {
    return actionCommand;
}

void GInteractor::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

void GInteractor::setSize(double width, double height) {
    pp->gobject_setSize(this, width, height);
}

void GInteractor::setBounds(const GRectangle& rect) {
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

std::string GInteractor::getIcon() const {
    return icon;
}

void GInteractor::setIcon(std::string filename) {
    this->icon = filename;
    pp->ginteractor_setIcon(this, filename);
}

void GInteractor::setTextPosition(SwingConstants horizontal, SwingConstants vertical) {
    pp->ginteractor_setTextPosition(this, horizontal, vertical);
}

/*
 * Implementation notes: GButton class
 * -----------------------------------
 */

GButton::GButton(std::string label) {
    this->label = label;
    pp->gbutton_constructor(this, label);
}

std::string GButton::getType() const {
    return "GButton";
}

std::string GButton::toString() const {
    std::ostringstream oss;
    oss << "GButton(\"" << label << "\")";
    return oss.str();
}

/*
 * Implementation notes: GCheckBox class
 * -------------------------------------
 */

GCheckBox::GCheckBox(std::string label) {
    this->label = label;
    pp->gcheckbox_constructor(this, label);
}

bool GCheckBox::isSelected() {
    return pp->gcheckbox_isSelected(this);
}

void GCheckBox::setSelected(bool state) {
    pp->gcheckbox_setSelected(this, state);
}

std::string GCheckBox::getType() const {
    return "GCheckBox";
}

std::string GCheckBox::toString() const {
    std::ostringstream oss;
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

std::string GSlider::getType() const {
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

std::string GSlider::toString() const {
    std::ostringstream oss;
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

std::string GTextField::getText() {
    return pp->gtextfield_getText(this);
}

bool GTextField::isEditable() const {
    return pp->gtextfield_isEditable(this);
}

void GTextField::setEditable(bool value) {
    pp->gtextfield_setEditable(this, value);
}

void GTextField::setText(std::string str) {
    pp->gtextfield_setText(this, str);
}

std::string GTextField::getType() const {
    return "GTextField";
}

std::string GTextField::toString() const {
    std::ostringstream oss;
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

void GChooser::addItem(std::string item) {
    pp->gchooser_addItem(this, item);
}

std::string GChooser::getSelectedItem() {
    return pp->gchooser_getSelectedItem(this);
}

void GChooser::setSelectedItem(std::string item) {
    pp->gchooser_setSelectedItem(this, item);
}

std::string GChooser::getType() const {
    return "GChooser";
}

std::string GChooser::toString() const {
    std::ostringstream oss;
    oss << "GChooser()";
    return oss.str();
}
