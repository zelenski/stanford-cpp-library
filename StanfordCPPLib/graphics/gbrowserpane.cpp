/*
 * File: gbrowserpane.cpp
 * ----------------------
 *
 * @version 2018/08/23
 * - renamed to gbrowserpane.h to replace Java version
 * @version 2018/07/15
 * - initial version
 */

#include "gbrowserpane.h"
#include <fstream>
#include <iostream>
#include "filelib.h"
#include "server.h"
#include "strlib.h"

_Internal_QTextBrowser::_Internal_QTextBrowser(GBrowserPane* gbrowserpane, QWidget* parent)
        : QTextBrowser(parent),
          _gbrowserpane(gbrowserpane) {
    // empty
}

QSize _Internal_QTextBrowser::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QTextBrowser::sizeHint();
    }
}


GBrowserPane::GBrowserPane(const std::string& url, QWidget* parent) {
    _iqtextbrowser = new _Internal_QTextBrowser(this, getInternalParent(parent));
    if (!url.empty()) {
        readTextFromUrl(url);
    }
}

GBrowserPane::~GBrowserPane() {
    // TODO: delete _iqtextbrowser;
    _iqtextbrowser = nullptr;
}

std::string GBrowserPane::getContentType() const {
    // TODO
    return "";
}

std::string GBrowserPane::getPageUrl() const {
    return _pageUrl;
}

_Internal_QWidget* GBrowserPane::getInternalWidget() const {
    return _iqtextbrowser;
}

std::string GBrowserPane::getText() const {
    return _iqtextbrowser->toHtml().toStdString();
}

std::string GBrowserPane::getType() const {
    return "GBrowserPane";
}

QWidget* GBrowserPane::getWidget() const {
    return static_cast<QWidget*>(_iqtextbrowser);
}

void GBrowserPane::readTextFromFile(std::istream& file) {
    std::string fileText = readEntireStream(file);
    setText(fileText);
}

void GBrowserPane::readTextFromFile(const std::string& filename) {
    std::ifstream input;
    input.open(filename.c_str());
    if (!input.fail()) {
        _pageUrl = filename;
        std::string extension = getExtension(filename);
        std::string contentType = HttpServer::getContentType(extension);
        if (!contentType.empty()) {
            setContentType(contentType);
        }
        readTextFromFile(input);
    }
}

void GBrowserPane::readTextFromUrl(const std::string& url) {
    QUrl qurl(QString::fromStdString(url));
    //_iqtextbrowser->
    _iqtextbrowser->setSource(qurl);
    this->_pageUrl = url;
}

void GBrowserPane::setContentType(const std::string& /* contentType */) {
    // TODO
}

void GBrowserPane::setText(const std::string& text) {
    _iqtextbrowser->setText(QString::fromStdString(text));
}
