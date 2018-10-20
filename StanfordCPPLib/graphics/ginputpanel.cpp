/*
 * File: ginputpanel.cpp
 * ---------------------
 * This file contains declarations of functions that can pop up an
 * "input panel" of easy clickable buttons on the screen.
 * See ginputpanel.h for documentation of each function.
 * 
 * @author Marty Stepp
 * @version 2018/08/28
 * - refactored from free functions to GInputPanel class
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2014/10/14
 * - initial version
 * @since 2014/10/14
 */

#define INTERNAL_INCLUDE 1
#include "ginputpanel.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "xmlutils.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

GInputPanel* GInputPanel::_instance = nullptr;

GInputPanel* GInputPanel::instance() {
    if (!_instance) {
        _instance = new GInputPanel();
    }
    return _instance;
}

GInputPanel::GInputPanel()
        : _loaded(false) {
    // empty
}

void GInputPanel::addButton(const std::string& /*text*/, const std::string& /*input*/) {
    // TODO
}

void GInputPanel::addCategory(const std::string& /*name*/) {
    // TODO
}

void GInputPanel::removeCategory(const std::string& /*name*/) {
    // TODO
}

void GInputPanel::removeButton(const std::string& /*text*/) {
    // TODO
}

void GInputPanel::setVisible(bool /*value*/) {
    // TODO
}

bool GInputPanel::isLoaded() const {
    return _loaded;
}

void GInputPanel::load(const std::string& xmlFilename) {
    if (isLoaded() || !fileExists(xmlFilename)) {
        return;
    }

    _loaded = true;
    rapidxml::xml_node<>* doc = xmlutils::openXmlDocument(xmlFilename, "inputpanel");
    for (rapidxml::xml_node<>* category : xmlutils::getChildNodes(doc, "category")) {
        std::string categoryName = xmlutils::getAttribute(category, "name");
        addCategory(categoryName);
        for (rapidxml::xml_node<>* button : xmlutils::getChildNodes(category, "button")) {
            std::string text = xmlutils::getAttribute(button, "text");
            std::string input = text;
            if (xmlutils::hasAttribute(button, "input")) {
                input = xmlutils::getAttribute(button, "input");
                
                // preprocess text to allow stuff like \n, \\, &quot; from XML to C++
                input = stringReplace(input, "\\n", "\n");
                input = stringReplace(input, "\\t", "\t");
                input = stringReplace(input, "\\r", "\r");
                input = stringReplace(input, "\\f", "\f");
                input = stringReplace(input, "\\\\", "\\");
                input = stringReplace(input, "&nbsp;", " ");
                input = stringReplace(input, "&lt;", "<");
                input = stringReplace(input, "&gt;", ">");
                input = stringReplace(input, "&amp;", "&");
                input = stringReplace(input, "&quot;", "\"");
            }
            addButton(text, input);
        }
    }
}
