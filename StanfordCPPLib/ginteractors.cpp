/*
 * File: gobjects.cpp
 * ------------------
 * This file implements the ginteractors.h interface.
 * 
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2015/06/20
 * - added GRadioButton class
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

/*
 * Implementation notes: GInteractor class
 * ---------------------------------------
 */

GInteractor::GInteractor() {
    actionCommand = "";
}

void GInteractor::setActionCommand(std::string cmd) {
    actionCommand = cmd;
    getPlatform()->ginteractor_setActionCommand(this, cmd);
}

std::string GInteractor::getActionCommand() {
    return actionCommand;
}

void GInteractor::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

void GInteractor::setSize(double width, double height) {
    getPlatform()->gobject_setSize(this, width, height);
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
    GDimension size = getPlatform()->ginteractor_getSize((GObject *) this);
    return GRectangle(x, y, size.getWidth(), size.getHeight());
}

bool GInteractor::isEnabled() {
    return getPlatform()->ginteractor_isEnabled(this);
}

void GInteractor::setEnabled(bool value) {
    getPlatform()->ginteractor_setEnabled(this, value);
}

std::string GInteractor::getIcon() const {
    return icon;
}

void GInteractor::setIcon(std::string filename) {
    this->icon = filename;
    getPlatform()->ginteractor_setIcon(this, filename);
}

void GInteractor::setTextPosition(SwingConstants horizontal, SwingConstants vertical) {
    getPlatform()->ginteractor_setTextPosition(this, horizontal, vertical);
}

/*
 * Implementation notes: GButton class
 * -----------------------------------
 */

GButton::GButton(std::string label) {
    this->label = label;
    getPlatform()->gbutton_constructor(this, label);
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
    getPlatform()->gcheckbox_constructor(this, label);
}

bool GCheckBox::isSelected() {
    return getPlatform()->gcheckbox_isSelected(this);
}

void GCheckBox::setSelected(bool state) {
    getPlatform()->gcheckbox_setSelected(this, state);
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
 * Implementation notes: GRadioButton class
 * ----------------------------------------
 */

GRadioButton::GRadioButton(std::string label, std::string group, bool selected) {
    this->label = label;
    if (group.empty()) {
        group = "default";
    }
    this->group = group;
    getPlatform()->gradiobutton_constructor(this, label, group);
    if (selected) {
        setSelected(true);
    }
}

bool GRadioButton::isSelected() {
    return getPlatform()->gradiobutton_isSelected(this);
}

void GRadioButton::setSelected(bool state) {
    getPlatform()->gradiobutton_setSelected(this, state);
}

std::string GRadioButton::getGroup() const {
    return group;
}

std::string GRadioButton::getType() const {
    return "GRadioButton";
}

std::string GRadioButton::toString() const {
    std::ostringstream oss;
    oss << "GRadioButton(\"" << group << "\", \"" << label << "\")";
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
    return getPlatform()->gslider_getMajorTickSpacing(this);
}

int GSlider::getMinorTickSpacing() const {
    return getPlatform()->gslider_getMinorTickSpacing(this);
}

bool GSlider::getPaintLabels() const {
    return getPlatform()->gslider_getPaintLabels(this);
}

bool GSlider::getPaintTicks() const {
    return getPlatform()->gslider_getPaintTicks(this);
}

bool GSlider::getSnapToTicks() const {
    return getPlatform()->gslider_getSnapToTicks(this);
}

std::string GSlider::getType() const {
    return "GSlider";
}

int GSlider::getValue() {
    return getPlatform()->gslider_getValue(this);
}

void GSlider::setMajorTickSpacing(int value) {
    getPlatform()->gslider_setMajorTickSpacing(this, value);
}

void GSlider::setMinorTickSpacing(int value) {
    getPlatform()->gslider_setMinorTickSpacing(this, value);
}

void GSlider::setPaintLabels(bool value) {
    getPlatform()->gslider_setPaintLabels(this, value);
}

void GSlider::setPaintTicks(bool value) {
    getPlatform()->gslider_setPaintTicks(this, value);
}

void GSlider::setSnapToTicks(bool value) {
    getPlatform()->gslider_setSnapToTicks(this, value);
}

void GSlider::setValue(int value) {
    getPlatform()->gslider_setValue(this, value);
}

std::string GSlider::toString() const {
    std::ostringstream oss;
    oss << "GSlider()";
    return oss.str();
}

void GSlider::create(int min, int max, int value) {
    this->min = min;
    this->max = max;
    getPlatform()->gslider_constructor(this, min, max, value);
}

/*
 * Implementation notes: GTextField class
 * --------------------------------------
 */

GTextField::GTextField() {
    getPlatform()->gtextfield_constructor(this, 10);
}

GTextField::GTextField(int nChars) {
    getPlatform()->gtextfield_constructor(this, nChars);
}

std::string GTextField::getText() {
    return getPlatform()->gtextfield_getText(this);
}

bool GTextField::isEditable() const {
    return getPlatform()->gtextfield_isEditable(this);
}

void GTextField::setEditable(bool value) {
    getPlatform()->gtextfield_setEditable(this, value);
}

void GTextField::setText(std::string str) {
    getPlatform()->gtextfield_setText(this, str);
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
    getPlatform()->gchooser_constructor(this);
}

void GChooser::addItem(std::string item) {
    getPlatform()->gchooser_addItem(this, item);
}

std::string GChooser::getSelectedItem() {
    return getPlatform()->gchooser_getSelectedItem(this);
}

void GChooser::setSelectedItem(std::string item) {
    getPlatform()->gchooser_setSelectedItem(this, item);
}

std::string GChooser::getType() const {
    return "GChooser";
}

std::string GChooser::toString() const {
    std::ostringstream oss;
    oss << "GChooser()";
    return oss.str();
}
