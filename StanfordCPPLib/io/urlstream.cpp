/*
 * File: urlstream.cpp
 * -------------------
 * This file contains the implementation of the iurlstream class.
 * Please see urlstream.h for information about how to use these classes.
 *
 * @author Marty Stepp
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

iurlstream::iurlstream() : m_url(""), m_tempFilePath(""), m_lastError(0) {
    // empty
}

iurlstream::iurlstream(std::string url) : m_url(url), m_tempFilePath(""), m_lastError(0) {
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
    return m_lastError;
}

void iurlstream::open(std::string url) {
    if (url.empty()) {
        url = m_url;
    }
    
    // download the entire URL to a temp file, put into stringbuf for reading
    std::string tempDir = getTempDirectory();
    std::string filename = getUrlFilename(url);
    m_tempFilePath = tempDir + getDirectoryPathSeparator() + filename;
    
    m_lastError = stanfordcpplib::getPlatform()->url_download(url, filename);
    
    if (m_lastError == ERR_MALFORMED_URL) {
        error("iurlstream::open: malformed URL when downloading " + url + " to " + m_tempFilePath);
    } else if (m_lastError == ERR_IO_EXCEPTION) {
        error("iurlstream::open: network I/O error when downloading " + url + " to " + m_tempFilePath);
    }
    if (m_lastError == 200) {
        std::ifstream::open(m_tempFilePath.c_str());
    } else {
        setstate(std::ios::failbit);
    }
}

std::string iurlstream::getUrlFilename(std::string url) const {
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
