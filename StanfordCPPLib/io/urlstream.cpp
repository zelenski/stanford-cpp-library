/*
 * File: urlstream.cpp
 * -------------------
 * This file contains the implementation of the iurlstream class.
 * Please see urlstream.h for information about how to use these classes.
 *
 * @author Marty Stepp
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
    /* Given a status code, determines whether it's successful. All successful HTTP
     * status codes are of the form 2xx.
     */
    bool isHttpSuccess(int code) {
        return code >= 200 && code <= 299;
    }
}

iurlstream::iurlstream()
        : _url(""),
          _tempFilePath(""),
          _lastError(0) {
    // empty
}

iurlstream::iurlstream(const std::string& url)
        : _url(url),
          _tempFilePath(""),
          _lastError(0) {
    open(url);
}

void iurlstream::close() {
    std::ifstream::close();
    if (!_tempFilePath.empty() && fileExists(_tempFilePath)) {
        deleteFile(_tempFilePath);
    }
    _tempFilePath = "";
    _lastError = 0;
}

int iurlstream::getErrorCode() const {
    return isHttpSuccess(_lastError)? 0 : _lastError;
}

int iurlstream::getHttpStatusCode() const {
    /* All HTTP status codes are between 1xx and 5xx, inclusive. */
    return _lastError >= 100 && _lastError <= 599? _lastError : 0;
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

std::string iurlstream::getUrlFilename(const std::string& url) const {
    std::string filename = url;

    // strip query string, anchor from URL
    int questionmark = stringIndexOf(url, "?");
    if (questionmark >= 0) {
        filename = filename.substr(0, questionmark);
    }
    int hash = stringIndexOf(url, "#");
    if (hash >= 0) {
        filename = filename.substr(0, hash);
    }

    filename = getTail(filename);
    if (filename.empty()) {
        filename = "index.tmp";   // for / urls like http://google.com/
    }

    return filename;
}

void iurlstream::open(const std::string& url) {
    if (!url.empty()) {
        _url = url;
    }
    
    // download the entire URL to a temp file, put into stringbuf for reading
    std::string tempDir = getTempDirectory();
    std::string filename = getUrlFilename(_url);
    _tempFilePath = tempDir + getDirectoryPathSeparator() + filename;
    
    // insert/send headers if needed
    if (_headers.isEmpty()) {
        // TODO
        // _lastError = url_download(_url, filename);
    } else {
        // TODO
        // _lastError = url_downloadWithHeaders(_url, filename, _headers);
    }

    if (isHttpSuccess(_lastError)) {
        std::ifstream::open(_tempFilePath.c_str());
    } else {
        setstate(std::ios::failbit);
    }
}

void iurlstream::setHeader(const std::string& name, const std::string& value) {
    _headers[name] = value;
}

void iurlstream::setUserAgent(const std::string& userAgent) {
    setHeader("User-Agent", userAgent);
}
