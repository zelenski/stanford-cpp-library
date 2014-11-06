/*
 * File: xmlutils.cpp
 * ------------------
 * This file contains implementations of utility functions related to processing
 * and extracting information from XML documents.
 * See xmlutils.h for documentation of each function.
 * 
 * @author Marty Stepp
 * @version 2014/10/14
 * @since 2014/03/01
 */

#include "xmlutils.h"
#include <cstring>
#include "filelib.h"
#include "rapidxml.h"
#include "strlib.h"

namespace xmlutils {
int getAttributeInt(rapidxml::xml_node<>* node, std::string attrName, int defaultValue) {
    rapidxml::xml_attribute<>* attr = node->first_attribute(attrName.c_str());
    if (attr == NULL) {
        return defaultValue;
    } else {
        return stringToInteger(attr->value());
    }
}

bool getAttributeBool(rapidxml::xml_node<>* node, std::string attrName, bool defaultValue) {
    rapidxml::xml_attribute<>* attr = node->first_attribute(attrName.c_str());
    if (attr == NULL) {
        return defaultValue;
    } else {
        std::string result = toLowerCase(std::string(attr->value()));
        return result == "true" || result == "t" || result == "1" || result == "on";
    }
}

std::string getAttribute(rapidxml::xml_node<>* node, std::string attrName, std::string defaultValue) {
    rapidxml::xml_attribute<>* attr = node->first_attribute(attrName.c_str());
    if (attr == NULL) {
        return defaultValue;
    } else {
        return attr->value();
    }
}

std::vector<rapidxml::xml_node<>*> getChildNodes(rapidxml::xml_node<>* node, std::string nodeName) {
    std::vector<rapidxml::xml_node<>*> v;
    for (rapidxml::xml_node<>* childNode = node->first_node(nodeName.c_str());
         childNode != NULL;
         childNode = childNode->next_sibling(nodeName.c_str())) {
        v.push_back(childNode);
    }
    return v;
}

bool hasAttribute(rapidxml::xml_node<>* node, std::string attrName) {
    rapidxml::xml_attribute<>* attr = node->first_attribute(attrName.c_str());
    return (attr != NULL);
}

rapidxml::xml_node<>* openXmlDocument(std::string filename, std::string documentNode) {
    std::string xmlFileText = readEntireFile(filename);
    char buf[xmlFileText.length() + 10];
    strcpy(buf, xmlFileText.c_str());
    rapidxml::xml_document<char> xmlDoc;
    xmlDoc.parse<0>(buf);
    rapidxml::xml_node<>* node = xmlDoc.first_node(documentNode.c_str());
    return node;
}
}
