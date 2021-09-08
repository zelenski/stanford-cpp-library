/*
 * File: xmlutils.h
 * ----------------
 * This file contains declarations of utility functions related to processing
 * and extracting information from XML documents.
 * It is a thin wrapper around the third-party RapidXML library (rapidxml*.{h,cpp})
 * so that we don't have to use or remember its unusual templatey syntax.
 * 
 * @author Marty Stepp
 * @version 2014/10/14
 * @since 2014/03/01
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _xmlutils_h
#define _xmlutils_h

#include <string>

#define INTERNAL_INCLUDE 1
#include "rapidxml.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

/**
 * @private
 */
namespace xmlutils {
    int getAttributeInt(rapidxml::xml_node<>* node, const std::string& attrName, int defaultValue = 0);
    bool getAttributeBool(rapidxml::xml_node<>* node, const std::string& attrName, bool defaultValue = false);
    std::string getAttribute(rapidxml::xml_node<>* node, const std::string& attrName, const std::string& defaultValue = "");
    Vector<rapidxml::xml_node<>*> getChildNodes(rapidxml::xml_node<>* node, const std::string& nodeName = "*");
    bool hasAttribute(rapidxml::xml_node<>* node, const std::string& attrName);
    rapidxml::xml_node<>* openXmlDocument(const std::string& filename, const std::string& documentNode = "*");
} // namespace xmlutils

#endif // _xmlutils_h
