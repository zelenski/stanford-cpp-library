/*
 * File: gobjects.cpp
 * ------------------
 * This file implements the ginteractors.h interface.
 * 
 * @version 2016/09/27
 * - added setText method to GButton, GCheckBox, GRadioButton
 * @version 2016/07/07
 * - added getText method to GButton, GCheckBox, GRadioButton
 * @version 2015/12/01
 * - added GInteractor::setBackground
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
#include "private/platform.h"

/*
 * Implementation notes: GInteractor class
 * ---------------------------------------
 */

GInteractor::GInteractor() {
    actionCommand = "";
}

void GInteractor::setActionCommand(std::string cmd) {
    actionCommand = cmd;
    stanfordcpplib::getPlatform()->ginteractor_setActionCommand(this, cmd);
}

std::string GInteractor::getActionCommand() {
    return actionCommand;
}

void GInteractor::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

void GInteractor::setSize(double width, double height) {
    stanfordcpplib::getPlatform()->gobject_setSize(this, width, height);
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
    GDimension size = stanfordcpplib::getPlatform()->ginteractor_getSize((GObject *) this);
    return GRectangle(x, y, size.getWidth(), size.getHeight());
}

bool GInteractor::isEnabled() {
    return stanfordcpplib::getPlatform()->ginteractor_isEnabled(this);
}

void GInteractor::setBackground(int rgb) {
    std::string color = convertRGBToColor(rgb);
    stanfordcpplib::getPlatform()->ginteractor_setBackground(this, color);
}

void GInteractor::setBackground(std::string color) {
    stanfordcpplib::getPlatform()->ginteractor_setBackground(this, color);
}

void GInteractor::setEnabled(bool value) {
    stanfordcpplib::getPlatform()->ginteractor_setEnabled(this, value);
}

std::string GInteractor::getIcon() const {
    return icon;
}

void GInteractor::setIcon(std::string filename) {
    this->icon = filename;
    stanfordcpplib::getPlatform()->ginteractor_setIcon(this, filename);
}

void GInteractor::setTextPosition(SwingConstants horizontal, SwingConstants vertical) {
    stanfordcpplib::getPlatform()->ginteractor_setTextPosition(this, horizontal, vertical);
}

/*
 * Implementation notes: GButton class
 * -----------------------------------
 */

GButton::GButton(std::string label) {
    this->label = label;
    stanfordcpplib::getPlatform()->gbutton_constructor(this, label);
}

std::string GButton::getText() const {
    return this->label;
}

void GButton::setText(std::string text) {
    this->label = text;
    stanfordcpplib::getPlatform()->ginteractor_setText(this, text);
    setActionCommand(text);
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
    stanfordcpplib::getPlatform()->gcheckbox_constructor(this, label);
}

std::string GCheckBox::getText() const {
    return this->label;
}

void GCheckBox::setText(std::string text) {
    this->label = text;
    stanfordcpplib::getPlatform()->ginteractor_setText(this, text);
    setActionCommand(text);
}

bool GCheckBox::isSelected() {
    return stanfordcpplib::getPlatform()->gcheckbox_isSelected(this);
}

bool GCheckBox::isChecked() {
    return isSelected();
}

void GCheckBox::setSelected(bool state) {
    stanfordcpplib::getPlatform()->gcheckbox_setSelected(this, state);
}

void GCheckBox::setChecked(bool state) {
    setSelected(state);
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
    stanfordcpplib::getPlatform()->gradiobutton_constructor(this, label, group);
    if (selected) {
        setSelected(true);
    }
}

std::string GRadioButton::getText() const {
    return this->label;
}

void GRadioButton::setText(std::string text) {
    this->label = text;
    stanfordcpplib::getPlatform()->ginteractor_setText(this, text);
    setActionCommand(text);
}

bool GRadioButton::isSelected() {
    return stanfordcpplib::getPlatform()->gradiobutton_isSelected(this);
}

bool GRadioButton::isChecked() {
    return isSelected();
}

void GRadioButton::setSelected(bool state) {
    stanfordcpplib::getPlatform()->gradiobutton_setSelected(this, state);
}

void GRadioButton::setChecked(bool state) {
    setSelected(state);
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
    return stanfordcpplib::getPlatform()->gslider_getMajorTickSpacing(this);
}

int GSlider::getMinorTickSpacing() const {
    return stanfordcpplib::getPlatform()->gslider_getMinorTickSpacing(this);
}

bool GSlider::getPaintLabels() const {
    return stanfordcpplib::getPlatform()->gslider_getPaintLabels(this);
}

bool GSlider::getPaintTicks() const {
    return stanfordcpplib::getPlatform()->gslider_getPaintTicks(this);
}

bool GSlider::getSnapToTicks() const {
    return stanfordcpplib::getPlatform()->gslider_getSnapToTicks(this);
}

std::string GSlider::getType() const {
    return "GSlider";
}

int GSlider::getValue() {
    return stanfordcpplib::getPlatform()->gslider_getValue(this);
}

void GSlider::setMajorTickSpacing(int value) {
    stanfordcpplib::getPlatform()->gslider_setMajorTickSpacing(this, value);
}

void GSlider::setMinorTickSpacing(int value) {
    stanfordcpplib::getPlatform()->gslider_setMinorTickSpacing(this, value);
}

void GSlider::setPaintLabels(bool value) {
    stanfordcpplib::getPlatform()->gslider_setPaintLabels(this, value);
}

void GSlider::setPaintTicks(bool value) {
    stanfordcpplib::getPlatform()->gslider_setPaintTicks(this, value);
}

void GSlider::setSnapToTicks(bool value) {
    stanfordcpplib::getPlatform()->gslider_setSnapToTicks(this, value);
}

void GSlider::setValue(int value) {
    stanfordcpplib::getPlatform()->gslider_setValue(this, value);
}

std::string GSlider::toString() const {
    std::ostringstream oss;
    oss << "GSlider()";
    return oss.str();
}

void GSlider::create(int min, int max, int value) {
    this->min = min;
    this->max = max;
    stanfordcpplib::getPlatform()->gslider_constructor(this, min, max, value);
}

/*
 * Implementation notes: GTextField class
 * --------------------------------------
 */

GTextField::GTextField() {
    stanfordcpplib::getPlatform()->gtextfield_constructor(this, 10);
}

GTextField::GTextField(int nChars) {
    stanfordcpplib::getPlatform()->gtextfield_constructor(this, nChars);
}

std::string GTextField::getText() {
    return stanfordcpplib::getPlatform()->gtextfield_getText(this);
}

bool GTextField::isEditable() const {
    return stanfordcpplib::getPlatform()->gtextfield_isEditable(this);
}

void GTextField::setEditable(bool value) {
    stanfordcpplib::getPlatform()->gtextfield_setEditable(this, value);
}

void GTextField::setText(std::string str) {
    stanfordcpplib::getPlatform()->gtextfield_setText(this, str);
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
    stanfordcpplib::getPlatform()->gchooser_constructor(this);
}

void GChooser::addItem(std::string item) {
    stanfordcpplib::getPlatform()->gchooser_addItem(this, item);
}

std::string GChooser::getSelectedItem() {
    return stanfordcpplib::getPlatform()->gchooser_getSelectedItem(this);
}

void GChooser::setSelectedItem(std::string item) {
    stanfordcpplib::getPlatform()->gchooser_setSelectedItem(this, item);
}

std::string GChooser::getType() const {
    return "GChooser";
}

std::string GChooser::toString() const {
    std::ostringstream oss;
    oss << "GChooser()";
    return oss.str();
}
