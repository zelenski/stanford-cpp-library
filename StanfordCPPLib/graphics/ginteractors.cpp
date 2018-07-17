/*
 * File: ginteractors.cpp
 * ----------------------
 * This file implements the ginteractors.h interface.
 * 
 * @version 2018/06/24
 * - added GFormattedPane
 * @version 2018/06/23
 * - added change listener functionality
 * @version 2017/11/18
 * - added GCheckBox constructor that takes bool for checked
 * @version 2017/10/12
 * - added GTextLabel
 * @version 2016/11/26
 * - added GInteractor::setForeground as alias for setColor
 * @version 2016/11/02
 * - added GTextField constructor that takes a string parameter
 * @version 2016/10/24
 * - added setFont, setMnemonic, setAccelerator to GInteractor
 * @version 2016/10/23
 * - added add/removeActionListener to GInteractor
 * @version 2016/10/15
 * - added setPlaceholder method to GTextField
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
#include "error.h"
#include "filelib.h"
#include "gevents.h"
#include "gobjects.h"
#include "gtypes.h"
#include "gwindow.h"
#include "server.h"
#include "strlib.h"
#include "private/platform.h"
#include "private/static.h"

// default # of characters visible in a GTextField
STATIC_CONST_VARIABLE_DECLARE(int, GTEXTFIELD_DEFAULT_NUM_CHARS, 10)

GInteractor::GInteractor() {
    actionCommand = "";
}

void GInteractor::addActionListener() {
    stanfordcpplib::getPlatform()->ginteractor_addActionListener(this);
}

void GInteractor::addChangeListener() {
    stanfordcpplib::getPlatform()->ginteractor_addChangeListener(this);
}

std::string GInteractor::getActionCommand() const {
    return actionCommand;
}

GRectangle GInteractor::getBounds() const {
    GDimension size = stanfordcpplib::getPlatform()->ginteractor_getSize((GObject *) this);
    return GRectangle(x, y, size.getWidth(), size.getHeight());
}

std::string GInteractor::getFont() const {
    return stanfordcpplib::getPlatform()->ginteractor_getFont((GObject *) this);
}

std::string GInteractor::getIcon() const {
    return icon;
}

char GInteractor::getMnemonic() const {
    return stanfordcpplib::getPlatform()->ginteractor_getMnemonic(this);
}

bool GInteractor::isEnabled() const {
    return stanfordcpplib::getPlatform()->ginteractor_isEnabled(this);
}

void GInteractor::removeActionListener() {
    stanfordcpplib::getPlatform()->ginteractor_removeActionListener(this);
}

void GInteractor::removeChangeListener() {
    stanfordcpplib::getPlatform()->ginteractor_removeChangeListener(this);
}

void GInteractor::requestFocus() {
    stanfordcpplib::getPlatform()->ginteractor_requestFocus(this);
}

void GInteractor::setAccelerator(const std::string& accelerator) {
    stanfordcpplib::getPlatform()->ginteractor_setAccelerator(this, accelerator);
}

void GInteractor::setActionCommand(const std::string& cmd) {
    actionCommand = cmd;
    stanfordcpplib::getPlatform()->ginteractor_setActionCommand(this, cmd);
}

void GInteractor::setBackground(int rgb) {
    std::string color = convertRGBToColor(rgb);
    stanfordcpplib::getPlatform()->ginteractor_setBackground(this, color);
}

void GInteractor::setBackground(const std::string& color) {
    stanfordcpplib::getPlatform()->ginteractor_setBackground(this, color);
}

void GInteractor::setBounds(double x, double y, double width, double height) {
    setLocation(x, y);
    setSize(width, height);
}

void GInteractor::setBounds(const GRectangle& rect) {
    setLocation(rect.getX(), rect.getY());
    setSize(rect.getWidth(), rect.getHeight());
}

void GInteractor::setEnabled(bool value) {
    stanfordcpplib::getPlatform()->ginteractor_setEnabled(this, value);
}

void GInteractor::setForeground(int rgb) {
    setColor(rgb);
}

void GInteractor::setForeground(const std::string& color) {
    setColor(color);
}


void GInteractor::setFont(const std::string& font) {
    stanfordcpplib::getPlatform()->ginteractor_setFont(this, font);
}

void GInteractor::setIcon(const std::string& filename) {
    if (!fileExists(filename)) {
        error("GInteractor::setIcon: icon file not found: " + filename);
    }
    this->icon = filename;
    stanfordcpplib::getPlatform()->ginteractor_setIcon(this, filename);
}

void GInteractor::setMnemonic(char mnemonic) {
    stanfordcpplib::getPlatform()->ginteractor_setMnemonic(this, mnemonic);
}

void GInteractor::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

void GInteractor::setSize(double width, double height) {
    stanfordcpplib::getPlatform()->gobject_setSize(this, width, height);
}

void GInteractor::setTextPosition(SwingConstants horizontal, SwingConstants vertical) {
    stanfordcpplib::getPlatform()->ginteractor_setTextPosition(this, horizontal, vertical);
}

void GInteractor::setTooltip(const std::string& tooltipText) {
    stanfordcpplib::getPlatform()->ginteractor_setTooltip(this, tooltipText);
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

std::string GButton::getType() const {
    return "GButton";
}

void GButton::setText(std::string text) {
    this->label = text;
    stanfordcpplib::getPlatform()->ginteractor_setText(this, text);
    setActionCommand(text);
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

GCheckBox::GCheckBox(std::string label, bool checked) {
    this->label = label;
    stanfordcpplib::getPlatform()->gcheckbox_constructor(this, label);
    if (checked) {
        setSelected(true);
    }
}

std::string GCheckBox::getText() const {
    return this->label;
}

std::string GCheckBox::getType() const {
    return "GCheckBox";
}

bool GCheckBox::isChecked() {
    return isSelected();
}

bool GCheckBox::isSelected() {
    return stanfordcpplib::getPlatform()->gcheckbox_isSelected(this);
}

void GCheckBox::setChecked(bool state) {
    setSelected(state);
}

void GCheckBox::setSelected(bool state) {
    stanfordcpplib::getPlatform()->gcheckbox_setSelected(this, state);
}

void GCheckBox::setText(std::string text) {
    this->label = text;
    stanfordcpplib::getPlatform()->ginteractor_setText(this, text);
    setActionCommand(text);
}

std::string GCheckBox::toString() const {
    std::ostringstream oss;
    oss << "GCheckBox(\"" << label << "\")";
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

void GChooser::addItems(const std::initializer_list<std::string>& items) {
    for (const std::string& item : items) {
        stanfordcpplib::getPlatform()->gchooser_addItem(this, item);
    }
}

void GChooser::addItems(const Vector<std::string>& items) {
    for (const std::string& item : items) {
        stanfordcpplib::getPlatform()->gchooser_addItem(this, item);
    }
}

std::string GChooser::getSelectedItem() {
    return stanfordcpplib::getPlatform()->gchooser_getSelectedItem(this);
}

std::string GChooser::getType() const {
    return "GChooser";
}

void GChooser::setSelectedItem(std::string item) {
    stanfordcpplib::getPlatform()->gchooser_setSelectedItem(this, item);
}

std::string GChooser::toString() const {
    std::ostringstream oss;
    oss << "GChooser()";
    return oss.str();
}

/*
 * Implementation notes: GFormattedPane class
 * ------------------------------------------
 * This class is based on Java's JEditorPane on the backend.
 */

GFormattedPane::GFormattedPane(const std::string& url) {
    stanfordcpplib::getPlatform()->gformattedpane_constructor(this);
    if (!url.empty()) {
        readTextFromUrl(url);
    }
}

std::string GFormattedPane::getContentType() const {
    return stanfordcpplib::getPlatform()->gformattedpane_getContentType(this);
}

std::string GFormattedPane::getPageUrl() const {
    return pageUrl;
}

std::string GFormattedPane::getText() const {
    return stanfordcpplib::getPlatform()->gformattedpane_getText(this);
}

void GFormattedPane::setContentType(const std::string& contentType) {
    stanfordcpplib::getPlatform()->gformattedpane_setContentType(this, contentType);
}

void GFormattedPane::readTextFromUrl(const std::string& url) {
    stanfordcpplib::getPlatform()->gformattedpane_setPage(this, url);
    this->pageUrl = url;
}

void GFormattedPane::setText(const std::string& text) {
    stanfordcpplib::getPlatform()->gformattedpane_setText(this, text);
}

void GFormattedPane::readTextFromFile(std::istream& file) {
    std::string fileText = readEntireStream(file);
    setText(fileText);
}

void GFormattedPane::readTextFromFile(const std::string& filename) {
    std::ifstream input;
    input.open(filename.c_str());
    if (!input.fail()) {
        pageUrl = filename;
        std::string extension = getExtension(filename);
        std::string contentType = HttpServer::getContentType(extension);
        if (!contentType.empty()) {
            setContentType(contentType);
        }
        readTextFromFile(input);
    }
}

/* Prototypes for the virtual methods */
std::string GFormattedPane::getType() const {
    return "GFormattedPane";
}

std::string GFormattedPane::toString() const {
    return "GFormattedPane(" + pageUrl + ")";
}


/*
 * Implementation notes: GRadioButton class
 * ----------------------------------------
 */

GRadioButton::GRadioButton(std::string label, std::string group, bool checked) {
    this->label = label;
    if (group.empty()) {
        group = "default";
    }
    this->group = group;
    stanfordcpplib::getPlatform()->gradiobutton_constructor(this, label, group);
    if (checked) {
        setSelected(true);
    }
}

std::string GRadioButton::getGroup() const {
    return group;
}

std::string GRadioButton::getText() const {
    return this->label;
}

std::string GRadioButton::getType() const {
    return "GRadioButton";
}

bool GRadioButton::isChecked() {
    return isSelected();
}

bool GRadioButton::isSelected() {
    return stanfordcpplib::getPlatform()->gradiobutton_isSelected(this);
}

void GRadioButton::setChecked(bool state) {
    setSelected(state);
}

void GRadioButton::setSelected(bool state) {
    stanfordcpplib::getPlatform()->gradiobutton_setSelected(this, state);
}

void GRadioButton::setText(std::string text) {
    this->label = text;
    stanfordcpplib::getPlatform()->ginteractor_setText(this, text);
    setActionCommand(text);
}

std::string GRadioButton::toString() const {
    std::ostringstream oss;
    oss << "GRadioButton(\"" << group << "\", \"" << label << "\")";
    return oss.str();
}


/*
 * Implementation notes: GScrollBar class
 * --------------------------------------
 * None.
 */

GScrollBar::GScrollBar(GScrollBar::Orientation orientation,
                       int value,
                       int extent,
                       int min,
                       int max)
        : _orientation(orientation),
          _value(value),
          _extent(extent),
          _min(min),
          _max(max) {
    stanfordcpplib::getPlatform()->gscrollbar_constructor(this, orientation, value, extent, min, max);
}

int GScrollBar::getExtent() const {
    return _extent;
}

int GScrollBar::getMax() const {
    return _max;
}

int GScrollBar::getMin() const {
    return _min;
}

GScrollBar::Orientation GScrollBar::getOrientation() const {
    return _orientation;
}

int GScrollBar::getValue() const {
    // return _value;
    // value may have changed on server, so always fetch from JBE
    return stanfordcpplib::getPlatform()->gscrollbar_getValue((GObject*) this);
}

void GScrollBar::setExtent(int extent) {
    _extent = extent;
    stanfordcpplib::getPlatform()->gscrollbar_setValues(this, _value, _extent, _min, _max);
}

void GScrollBar::setMax(int max) {
    _max = max;
    stanfordcpplib::getPlatform()->gscrollbar_setValues(this, _value, _extent, _min, _max);
}

void GScrollBar::setMin(int min) {
    _min = min;
    stanfordcpplib::getPlatform()->gscrollbar_setValues(this, _value, _extent, _min, _max);
}

void GScrollBar::setState(int value, int extent, int min, int max) {
    _value = value;
    _extent = extent;
    _min = min;
    _max = max;
    stanfordcpplib::getPlatform()->gscrollbar_setValues(this, _value, _extent, _min, _max);
}

void GScrollBar::setValue(int value) {
    _value = value;
    stanfordcpplib::getPlatform()->gscrollbar_setValues(this, _value, _extent, _min, _max);
}

std::string GScrollBar::getType() const {
    return "GScrollBar";
}

std::string GScrollBar::toString() const {
    return std::string("GScrollBar(orientation=")
            + (_orientation == VERTICAL ? "VERTICAL" : "HORIZONTAL")
            + ",value=" + integerToString(getValue())
            + ",extent=" + integerToString(_extent)
            + ",min=" + integerToString(_min)
            + ",max=" + integerToString(_max)
            + ")";
}


/*
 * Implementation notes: GSlider class
 * -----------------------------------
 */

GSlider::GSlider() {
    create(/* min */ 0, /* max */ 100, /* value */ 50);
}

GSlider::GSlider(int min, int max, int value) {
    create(min, max, value);
}

void GSlider::create(int min, int max, int value) {
    this->min = min;
    this->max = max;
    stanfordcpplib::getPlatform()->gslider_constructor(this, min, max, value);
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

/*
 * Implementation notes: GTextField class
 * --------------------------------------
 */

GTextField::GTextField() {
    stanfordcpplib::getPlatform()->gtextfield_constructor(this, STATIC_VARIABLE(GTEXTFIELD_DEFAULT_NUM_CHARS));
}

GTextField::GTextField(int nChars) {
    stanfordcpplib::getPlatform()->gtextfield_constructor(this, nChars);
}

GTextField::GTextField(const std::string& text) {
    int nChars = (int) text.length();
    stanfordcpplib::getPlatform()->gtextfield_constructor(this, nChars);
    setText(text);
}

GTextField::InputType GTextField::getInputType() const {
    return m_inputType;
}

std::string GTextField::getPlaceholder() const {
    return m_placeholder;
}

std::string GTextField::getText() const {
    return stanfordcpplib::getPlatform()->gtextfield_getText(this);
}

std::string GTextField::getType() const {
    return "GTextField";
}

double GTextField::getValueAsDouble() const {
    std::string text = trim(getText());
    return stringToDouble(text);
}

int GTextField::getValueAsInteger() const {
    std::string text = trim(getText());
    return stringToInteger(text);
}

bool GTextField::isEditable() const {
    return stanfordcpplib::getPlatform()->gtextfield_isEditable(this);
}

void GTextField::setEditable(bool value) {
    stanfordcpplib::getPlatform()->gtextfield_setEditable(this, value);
}

void GTextField::setInputType(GTextField::InputType inputType) {
    m_inputType = inputType;
}

void GTextField::setPlaceholder(const std::string& text) {
    m_placeholder = text;
    stanfordcpplib::getPlatform()->gtextfield_setPlaceholder(this, text);
}

void GTextField::setText(const std::string& str) {
    stanfordcpplib::getPlatform()->gtextfield_setText(this, str);
}

void GTextField::setValue(double value) {
    setText(doubleToString(value));
}

void GTextField::setValue(int value) {
    setText(integerToString(value));
}

bool GTextField::valueIsDouble() const {
    return stringIsDouble(trim(getText()));
}

bool GTextField::valueIsInteger() const {
    return stringIsInteger(trim(getText()));
}

bool GTextField::valueIsReal() const {
    return stringIsReal(trim(getText()));
}

std::string GTextField::toString() const {
    std::ostringstream oss;
    oss << "GTextField()";
    return oss.str();
}

/*
 * Implementation notes: GTextLabel class
 * --------------------------------------
 */

GTextLabel::GTextLabel(std::string label) {
    this->label = label;
    stanfordcpplib::getPlatform()->gtextlabel_constructor(this, label);
}

std::string GTextLabel::getText() const {
    return this->label;
}

std::string GTextLabel::getType() const {
    return "GTextLabel";
}

void GTextLabel::setText(std::string text) {
    this->label = text;
    stanfordcpplib::getPlatform()->ginteractor_setText(this, text);
    setActionCommand(text);
}

std::string GTextLabel::toString() const {
    std::ostringstream oss;
    oss << "GTextLabel(\"" << label << "\")";
    return oss.str();
}
