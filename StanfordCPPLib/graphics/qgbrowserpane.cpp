/*
 * File: qgbrowserpane.cpp
 * -----------------------
 *
 * @version 2018/07/15
 * - initial version
 */

#include "qgbrowserpane.h"
#include <fstream>
#include <iostream>
#include "filelib.h"
#include "server.h"
#include "strlib.h"

_Internal_QTextBrowser::_Internal_QTextBrowser(QGBrowserPane* qgbrowserpane, QWidget* parent)
        : QTextBrowser(parent),
          _qgbrowserpane(qgbrowserpane) {
    // empty
}

QSize _Internal_QTextBrowser::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QTextBrowser::sizeHint();
    }
}


QGBrowserPane::QGBrowserPane(const std::string& url, QWidget* parent) {
    _iqtextbrowser = new _Internal_QTextBrowser(this, getInternalParent(parent));
    if (!url.empty()) {
        readTextFromUrl(url);
    }
}

QGBrowserPane::~QGBrowserPane() {
    // TODO: delete _iqtextbrowser;
    _iqtextbrowser = nullptr;
}

std::string QGBrowserPane::getContentType() const {
    // TODO
    return "";
}

std::string QGBrowserPane::getPageUrl() const {
    return _pageUrl;
}

_Internal_QWidget* QGBrowserPane::getInternalWidget() const {
    return _iqtextbrowser;
}

std::string QGBrowserPane::getText() const {
    return _iqtextbrowser->toHtml().toStdString();
}

std::string QGBrowserPane::getType() const {
    return "QGBrowserPane";
}

QWidget* QGBrowserPane::getWidget() const {
    return static_cast<QWidget*>(_iqtextbrowser);
}

void QGBrowserPane::readTextFromFile(std::istream& file) {
    std::string fileText = readEntireStream(file);
    setText(fileText);
}

void QGBrowserPane::readTextFromFile(const std::string& filename) {
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

void QGBrowserPane::readTextFromUrl(const std::string& url) {
    QUrl qurl(QString::fromStdString(url));
    //_iqtextbrowser->
    _iqtextbrowser->setSource(qurl);
    this->_pageUrl = url;
}

void QGBrowserPane::setContentType(const std::string& /* contentType */) {
    // TODO
}

void QGBrowserPane::setText(const std::string& text) {
    _iqtextbrowser->setText(QString::fromStdString(text));
    // QGBorderLayout::forceUpdate(_iqtextbrowser);
}
