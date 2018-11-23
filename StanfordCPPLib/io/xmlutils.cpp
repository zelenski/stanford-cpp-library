/*
 * File: xmlutils.cpp
 * ------------------
 * This file contains implementations of utility functions related to processing
 * and extracting information from XML documents.
 * See xmlutils.h for documentation of each function.
 * 
 * @author Marty Stepp
 * @version 2016/10/22
 * - changed openXmlDocument to print error message rather than crash on file-not-found
 * @version 2016/10/14
 * - changed NULL to nullptr as appropriate
 * @version 2014/10/14
 * @since 2014/03/01
 */

#define INTERNAL_INCLUDE 1
#include "xmlutils.h"
#include <cstring>
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "rapidxml.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

namespace xmlutils {
int getAttributeInt(rapidxml::xml_node<>* node, const std::string& attrName, int defaultValue) {
    std::string value = getAttribute(node, attrName, std::to_string(defaultValue));
    return stringToInteger(value);
}

bool getAttributeBool(rapidxml::xml_node<>* node, const std::string& attrName, bool defaultValue) {
    std::string value = toLowerCase(getAttribute(node, attrName, boolToString(defaultValue)));
    return value == "true" || value == "t" || value == "1" || value == "on";
}

std::string getAttribute(rapidxml::xml_node<>* node, const std::string& attrName, const std::string& defaultValue) {
    rapidxml::xml_attribute<>* attr = node->first_attribute(attrName.c_str());
    if (!attr) {
        return defaultValue;
    } else {
        // RapidXML stores its value strings in a funny way, as non-zero-terminated
        // strings indexed into the original char buffer you passed it,
        // along with a size field.
        char* cstrVal = attr->value();
        return std::string(cstrVal);
    }
}

Vector<rapidxml::xml_node<>*> getChildNodes(rapidxml::xml_node<>* node, const std::string& nodeName) {
    Vector<rapidxml::xml_node<>*> v;
    for (rapidxml::xml_node<>* childNode = node->first_node(nodeName.c_str());
         childNode != nullptr;
         childNode = childNode->next_sibling(nodeName.c_str())) {
        v.push_back(childNode);
    }
    return v;
}

bool hasAttribute(rapidxml::xml_node<>* node, const std::string& attrName) {
    rapidxml::xml_attribute<>* attr = node->first_attribute(attrName.c_str());
    return (attr != nullptr);
}

rapidxml::xml_node<>* openXmlDocument(const std::string& filename, const std::string& documentNode) {
    std::string xmlFileText = readEntireFile(filename);
    char* buf = new char[xmlFileText.length() + 1024]();   // *** memory leak (but MUST be heap-allocated)
    memset(buf, 0, xmlFileText.length() + 1024);
    strcpy(buf, xmlFileText.c_str());
    rapidxml::xml_document<char>* xmlDoc = new rapidxml::xml_document<char>;   // *** memory leak (but MUST be heap-allocated)
    xmlDoc->parse<NULL>(buf);
    rapidxml::xml_node<>* node = xmlDoc->first_node(documentNode.c_str());
    return node;
}
} // namespace xmlutils
