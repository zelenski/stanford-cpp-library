/*
 * File: inputpanel.cpp
 * --------------------
 * This file contains declarations of functions that can pop up an
 * "input panel" of easy clickable buttons on the screen.
 * See inputpanel.h for documentation of each function.
 * 
 * @author Marty Stepp
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2014/10/14
 * - initial version
 * @since 2014/10/14
 */

#include "inputpanel.h"
#include "private/platform.h"
#include "private/static.h"
#include "xmlutils.h"

namespace inputpanel {
STATIC_VARIABLE_DECLARE(bool, inputPanelIsLoaded, false)

void addInputButton(const std::string& text, const std::string& input) {
    stanfordcpplib::getPlatform()->autograderinput_addButton(text, input);
}

void addInputCategory(const std::string& name) {
    stanfordcpplib::getPlatform()->autograderinput_addCategory(name);
}

void removeInputCategory(const std::string& name) {
    stanfordcpplib::getPlatform()->autograderinput_removeCategory(name);
}

void removeInputButton(const std::string& text) {
    stanfordcpplib::getPlatform()->autograderinput_removeButton(text);
}

void setVisible(bool value) {
    stanfordcpplib::getPlatform()->autograderinput_setVisible(value);
}

bool isLoaded() {
    return STATIC_VARIABLE(inputPanelIsLoaded);
}

void load(std::string xmlFilename) {
    if (isLoaded()) {
        return;
    }
    STATIC_VARIABLE(inputPanelIsLoaded) = true;
    rapidxml::xml_node<>* doc = xmlutils::openXmlDocument(xmlFilename, "inputpanel");
    for (rapidxml::xml_node<>* category : xmlutils::getChildNodes(doc, "category")) {
        std::string categoryName = xmlutils::getAttribute(category, "name");
        inputpanel::addInputCategory(categoryName);
        for (rapidxml::xml_node<>* button : xmlutils::getChildNodes(category, "button")) {
            std::string text = xmlutils::getAttribute(button, "text");
            std::string input = text;
            if (xmlutils::hasAttribute(button, "input")) {
                input = xmlutils::getAttribute(button, "input");
                
                // allow stuff like \n, \\, &quot; from XML to C++
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
            inputpanel::addInputButton(text, input);
        }
    }
}
} // namespace inputpanel
