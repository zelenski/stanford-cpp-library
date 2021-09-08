/*
 * File: urlstream.cpp
 * -------------------
 * This file contains the implementation of the iurlstream class.
 * Please see urlstream.h for information about how to use these classes.
 *
 * @author Marty Stepp
 * @version 2018/10/02
 * - added close() method for backward compatibility (does nothing)
 * @version 2018/09/18
 * - refactored to integrate with pure-C++ GDownloader implementation
 * - added getErrorMessage method
 * @version 2018/06/20
 * - support for setting headers such as user agent
 * - https URL support
 * - changed string to const string&
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/14
 * - fixed .c_str() Mac bug on ifstream::open() call
 * @since 2014/10/08
 */

#include "urlstream.h"
#include <sstream>
#include <string>
#include "error.h"
#include "filelib.h"
#include "gdownloader.h"
#include "strlib.h"

namespace {
    /*
     * Given a status code, determines whether it's successful.
     * All successful HTTP status codes are of the form 2xx.
     */
    bool isHttpSuccess(int code) {
        return code >= 200 && code <= 299;
    }
}

iurlstream::iurlstream()
        : _url(""),
          _httpStatusCode(0) {
    // empty
}

iurlstream::iurlstream(const std::string& url)
        : _url(url),
          _httpStatusCode(0) {
    open(url);
}

void iurlstream::close() {
    // empty
}

int iurlstream::getErrorCode() const {
    return isHttpSuccess(_httpStatusCode)? 0 : _httpStatusCode;
}

std::string iurlstream::getErrorMessage() const {
    return _errorMessage;
}

int iurlstream::getHttpStatusCode() const {
    /* All HTTP status codes are between 1xx and 5xx, inclusive. */
    return _httpStatusCode >= 100 && _httpStatusCode <= 599? _httpStatusCode : 0;
}

std::string iurlstream::getHeader(const std::string& name) const {
    return _headers[name];
}

std::string iurlstream::getUrl() const {
    return _url;
}

std::string iurlstream::getUserAgent() const {
    if (_headers.containsKey("User-Agent")) {
        return _headers["User-Agent"];
    } else {
        return "";
    }
}

void iurlstream::open(const std::string& url) {
    if (!url.empty()) {
        _url = url;
    }
    _errorMessage = "";
    
    // GDownloader does the heavy lifting of downloading the file for us
    GDownloader downloader;

    // insert/send headers if needed
    if (!_headers.isEmpty()) {
        for (std::string headerName : _headers) {
            downloader.setHeader(headerName, _headers[headerName]);
        }
    }
    std::string urlData = downloader.downloadAsString(_url);
    _httpStatusCode = downloader.getHttpStatusCode();

    if (downloader.hasError()) {
        setstate(std::ios::failbit);
        _errorMessage = downloader.getErrorMessage();
    } else {
        clear();
        this->write(urlData.c_str(), static_cast<std::streamsize>(urlData.length()));
        this->seekg(0);
    }
}

void iurlstream::setHeader(const std::string& name, const std::string& value) {
    _headers[name] = value;
}

void iurlstream::setUserAgent(const std::string& userAgent) {
    setHeader("User-Agent", userAgent);
}
