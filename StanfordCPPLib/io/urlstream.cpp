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
#include "strlib.h"
#include "private/platform.h"

namespace {
    /* Given a status code, determines whether it's successful. All successful HTTP
     * status codes are of the form 2xx.
     */
    bool isHttpSuccess(int code) {
        return code >= 200 && code <= 299;
    }
}

iurlstream::iurlstream()
        : m_url(""),
          m_tempFilePath(""),
          m_lastError(0) {
    // empty
}

iurlstream::iurlstream(const std::string& url)
        : m_url(url),
          m_tempFilePath(""),
          m_lastError(0) {
    open(url);
}

void iurlstream::close() {
    std::ifstream::close();
    if (!m_tempFilePath.empty() && fileExists(m_tempFilePath)) {
        deleteFile(m_tempFilePath);
    }
    m_tempFilePath = "";
    m_lastError = 0;
}

int iurlstream::getErrorCode() const {
    return isHttpSuccess(m_lastError)? 0 : m_lastError;
}

int iurlstream::getHttpStatusCode() const {
    /* All HTTP status codes are between 1xx and 5xx, inclusive. */
    return m_lastError >= 100 && m_lastError <= 599? m_lastError : 0;
}

std::string iurlstream::getHeader(const std::string& name) const {
    return m_headers[name];
}

std::string iurlstream::getUrl() const {
    return m_url;
}

std::string iurlstream::getUserAgent() const {
    if (m_headers.containsKey("User-Agent")) {
        return m_headers["User-Agent"];
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
        m_url = url;
    }
    
    // download the entire URL to a temp file, put into stringbuf for reading
    std::string tempDir = getTempDirectory();
    std::string filename = getUrlFilename(m_url);
    m_tempFilePath = tempDir + getDirectoryPathSeparator() + filename;
    
    // insert/send headers to backend if needed
    if (m_headers.isEmpty()) {
        m_lastError = stanfordcpplib::getPlatform()->url_download(m_url, filename);
    } else {
        m_lastError = stanfordcpplib::getPlatform()->url_downloadWithHeaders(m_url, filename, m_headers);
    }

    if (isHttpSuccess(m_lastError)) {
        std::ifstream::open(m_tempFilePath.c_str());
    } else {
        setstate(std::ios::failbit);
    }
}

void iurlstream::setHeader(const std::string& name, const std::string& value) {
    m_headers[name] = value;
}

void iurlstream::setUserAgent(const std::string& userAgent) {
    setHeader("User-Agent", userAgent);
}
