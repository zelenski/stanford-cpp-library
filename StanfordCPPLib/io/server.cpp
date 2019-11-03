/*
 * File: server.cpp
 * ----------------
 * This file exports a set of functions that implement a simple HTTP server
 * that can listen for connections.
 *
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2016/03/16
 * - initial version
 */

#include "server.h"
#include "filelib.h"
#include "map.h"
#include "strlib.h"
#include "private/static.h"

namespace HttpServer {
STATIC_CONST_VARIABLE_DECLARE(std::string, CONTENT_TYPE_DEFAULT, "text/html")
// STATIC_CONST_VARIABLE_DECLARE(std::string, CONTENT_TYPE_ERROR, "text/plain")
STATIC_VARIABLE_DECLARE(bool, isRunning, false)

bool isRunning() {
    return STATIC_VARIABLE(isRunning);
}

std::string getContentType(const std::string& extension) {
    static Map<std::string, std::string> CONTENT_TYPE_MAP;   // extension => MIME type

    if (extension.empty()) {
        return STATIC_VARIABLE(CONTENT_TYPE_DEFAULT);
    }

    // populate map of content types, if needed
    if (CONTENT_TYPE_MAP.isEmpty()) {
        CONTENT_TYPE_MAP["bmp"] = "image/bmp";
        CONTENT_TYPE_MAP["bz"] = "application/x-bzip";
        CONTENT_TYPE_MAP["bz2"] = "application/x-bzip2";
        CONTENT_TYPE_MAP["c"] = "text/plain";
        CONTENT_TYPE_MAP["cc"] = "text/plain";
        CONTENT_TYPE_MAP["com"] = "application/octet-stream";
        CONTENT_TYPE_MAP["cpp"] = "text/plain";
        CONTENT_TYPE_MAP["css"] = "text/css";
        CONTENT_TYPE_MAP["doc"] = "application/msword";
        CONTENT_TYPE_MAP["dot"] = "application/msword";
        CONTENT_TYPE_MAP["exe"] = "application/octet-stream";
        CONTENT_TYPE_MAP["gif"] = "image/gif";
        CONTENT_TYPE_MAP["gz"] = "application/x-gzip";
        CONTENT_TYPE_MAP["gzip"] = "application/x-gzip";
        CONTENT_TYPE_MAP["h"] = "text/plain";
        CONTENT_TYPE_MAP["hh"] = "text/plain";
        CONTENT_TYPE_MAP["hpp"] = "text/plain";
        CONTENT_TYPE_MAP["htm"] = "text/html";
        CONTENT_TYPE_MAP["html"] = "text/html";
        CONTENT_TYPE_MAP["htmls"] = "text/html";
        CONTENT_TYPE_MAP["ico"] = "image/x-icon";
        CONTENT_TYPE_MAP["inf"] = "text/plain";
        CONTENT_TYPE_MAP["jar"] = "application/octet-stream";
        CONTENT_TYPE_MAP["jav"] = "text/plain";
        CONTENT_TYPE_MAP["java"] = "text/plain";
        CONTENT_TYPE_MAP["jpe"] = "image/jpeg";
        CONTENT_TYPE_MAP["jpeg"] = "image/jpeg";
        CONTENT_TYPE_MAP["jpg"] = "image/jpeg";
        CONTENT_TYPE_MAP["mid"] = "audio/midi";
        CONTENT_TYPE_MAP["midi"] = "audio/midi";
        CONTENT_TYPE_MAP["mod"] = "audio/mod";
        CONTENT_TYPE_MAP["mov"] = "video/quicktime";
        CONTENT_TYPE_MAP["mp3"] = "text/plain";
        CONTENT_TYPE_MAP["mpg"] = "video/mpeg";
        CONTENT_TYPE_MAP["o"] = "application/octet-stream";
        CONTENT_TYPE_MAP["odc"] = "application/vnd.oasis.opendocument.chart";
        CONTENT_TYPE_MAP["odp"] = "application/vnd.oasis.opendocument.presentation";
        CONTENT_TYPE_MAP["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
        CONTENT_TYPE_MAP["odt"] = "application/vnd.oasis.opendocument.text";
        CONTENT_TYPE_MAP["pct"] = "image/x-pict";
        CONTENT_TYPE_MAP["pcx"] = "image/x-pcx";
        CONTENT_TYPE_MAP["pdf"] = "application/pdf";
        CONTENT_TYPE_MAP["pl"] = "text/plain";
        CONTENT_TYPE_MAP["pm"] = "text/plain";
        CONTENT_TYPE_MAP["ppt"] = "application/powerpoint";
        CONTENT_TYPE_MAP["ps"] = "application/postscript";
        CONTENT_TYPE_MAP["psd"] = "application/octet-stream";
        CONTENT_TYPE_MAP["py"] = "text/plain";
        CONTENT_TYPE_MAP["qt"] = "video/quicktime";
        CONTENT_TYPE_MAP["ra"] = "audio/x-realaudio";
        CONTENT_TYPE_MAP["rb"] = "text/plain";
        CONTENT_TYPE_MAP["rm"] = "application/vnd.rn-realmedia";
        CONTENT_TYPE_MAP["rtf"] = "application/rtf";
        CONTENT_TYPE_MAP["s"] = "text/x-asm";
        CONTENT_TYPE_MAP["sh"] = "text/plain";
        CONTENT_TYPE_MAP["shtml"] = "text/html";
        CONTENT_TYPE_MAP["swf"] = "application/x-shockwave-flash";
        CONTENT_TYPE_MAP["tcl"] = "application/x-tcl";
        CONTENT_TYPE_MAP["tex"] = "application/x-tex";
        CONTENT_TYPE_MAP["tgz"] = "application/x-compressed";
        CONTENT_TYPE_MAP["tif"] = "image/tiff";
        CONTENT_TYPE_MAP["tiff"] = "image/tiff";
        CONTENT_TYPE_MAP["txt"] = "text/plain";
        CONTENT_TYPE_MAP["voc"] = "audio/voc";
        CONTENT_TYPE_MAP["wav"] = "audio/wav";
        CONTENT_TYPE_MAP["xls"] = "application/excel";
        CONTENT_TYPE_MAP["xlt"] = "application/excel";
        CONTENT_TYPE_MAP["xpm"] = "image/xpm";
        CONTENT_TYPE_MAP["z"] = "application/x-compressed";
        CONTENT_TYPE_MAP["zip"] = "application/zip";
    }

    // "foo.BAZ.BaR" => "bar"
    std::string ext = toLowerCase(extension);
    int dot = stringLastIndexOf(ext, ".");
    if (dot >= 0) {
        ext = ext.substr(dot + 1);
    }

    if (CONTENT_TYPE_MAP.containsKey(ext)) {
        return CONTENT_TYPE_MAP[ext];
    } else {
        return STATIC_VARIABLE(CONTENT_TYPE_DEFAULT);
    }
}

std::string getErrorMessage(int httpErrorCode) {
    static Map<int, std::string> ERROR_MESSAGE_MAP;   // 404 => "File not found"

    // populate map of content types, if needed
    if (ERROR_MESSAGE_MAP.isEmpty()) {
        ERROR_MESSAGE_MAP[200] = "HTTP ERROR 200: OK";
        ERROR_MESSAGE_MAP[201] = "HTTP ERROR 201: Created";
        ERROR_MESSAGE_MAP[202] = "HTTP ERROR 202: Accepted";
        ERROR_MESSAGE_MAP[204] = "HTTP ERROR 204: No content";
        ERROR_MESSAGE_MAP[301] = "HTTP ERROR 301: Moved permanently";
        ERROR_MESSAGE_MAP[302] = "HTTP ERROR 302: Found";
        ERROR_MESSAGE_MAP[303] = "HTTP ERROR 303: See other";
        ERROR_MESSAGE_MAP[304] = "HTTP ERROR 304: Not modified";
        ERROR_MESSAGE_MAP[305] = "HTTP ERROR 305: Use proxy";
        ERROR_MESSAGE_MAP[307] = "HTTP ERROR 307: Temporary redirect";
        ERROR_MESSAGE_MAP[308] = "HTTP ERROR 308: Permanent redirect";
        ERROR_MESSAGE_MAP[400] = "HTTP ERROR 400: Bad request";
        ERROR_MESSAGE_MAP[401] = "HTTP ERROR 401: Unauthorized";
        ERROR_MESSAGE_MAP[402] = "HTTP ERROR 402: Payment required";
        ERROR_MESSAGE_MAP[403] = "HTTP ERROR 403: Forbidden";
        ERROR_MESSAGE_MAP[404] = "HTTP ERROR 404: Not found";
        ERROR_MESSAGE_MAP[405] = "HTTP ERROR 405: Request method not allowed";
        ERROR_MESSAGE_MAP[406] = "HTTP ERROR 406: Not acceptable";
        ERROR_MESSAGE_MAP[407] = "HTTP ERROR 407: Proxy authentication failed";
        ERROR_MESSAGE_MAP[408] = "HTTP ERROR 408: Request timeout";
        ERROR_MESSAGE_MAP[409] = "HTTP ERROR 409: Conflict";
        ERROR_MESSAGE_MAP[410] = "HTTP ERROR 410: Gone";
        ERROR_MESSAGE_MAP[413] = "HTTP ERROR 413: Payload too large";
        ERROR_MESSAGE_MAP[415] = "HTTP ERROR 415: Unsupported media type";
        ERROR_MESSAGE_MAP[420] = "HTTP ERROR 420: Enhance your calm; hey whatever man";
        ERROR_MESSAGE_MAP[429] = "HTTP ERROR 429: Too many requests";
        ERROR_MESSAGE_MAP[500] = "HTTP ERROR 500: Internal server error";
        ERROR_MESSAGE_MAP[501] = "HTTP ERROR 501: Not implemented";
        ERROR_MESSAGE_MAP[502] = "HTTP ERROR 502: Bad gateway";
        ERROR_MESSAGE_MAP[503] = "HTTP ERROR 503: Service unavailable";
        ERROR_MESSAGE_MAP[504] = "HTTP ERROR 504: Gateway timeout";
        ERROR_MESSAGE_MAP[508] = "HTTP ERROR 508: Loop detected";
        ERROR_MESSAGE_MAP[511] = "HTTP ERROR 511: Network authentication required";
    }

    if (ERROR_MESSAGE_MAP.containsKey(httpErrorCode)) {
        return ERROR_MESSAGE_MAP[httpErrorCode];
    } else {
        return "HTTP ERROR " + std::to_string(httpErrorCode) + ": Unknown error";
    }
}

// "http://foo.bar.com/a/b/c.txt?a=b&c=d#lol" => "txt"
// "http://foo.bar.com/index.html" => "html"
// "http://foo.bar.com/a/b/" => ""
std::string getUrlExtension(const std::string& url) {
    std::string url2 = toLowerCase(trim(url));
    int questionMark = stringIndexOf(url2, "?");
    if (questionMark >= 0) {
        url2 = url2.substr(0, questionMark);
    }
    int hash = stringIndexOf(url2, "#");
    if (hash >= 0) {
        url2 = url2.substr(0, hash);
    }
    int slash = stringLastIndexOf(url2, "/");
    if (slash >= 0) {
        url2 = url2.substr(slash + 1);
    }
    int dot = stringLastIndexOf(url2, ".");
    if (dot >= 0) {
        url2 = url2.substr(dot + 1);
    }
    return url2;
}

void sendResponse(const GEvent& /*event*/, const std::string& /* responseText */,
                  const std::string& contentType) {
    if (!isRunning()) {
        error("HttpServer::sendResponse: server is not running");
    }
    std::string contentTypeActual = contentType;
    if (contentTypeActual.empty()) {
        // TODO
        // contentTypeActual = getContentType(getUrlExtension(event.getRequestURL()));
    }
    // TODO
    // stanfordcpplib::getPlatform()->httpserver_sendResponse(event.getRequestID(), HTTP_ERROR_OK, contentTypeActual, responseText);
}

void sendResponseError(const GEvent& /*event*/, int httpErrorCode,
                       const std::string& errorMessage) {
    std::string errorMessageActual = errorMessage;
    if (errorMessageActual.empty()) {
        errorMessageActual = getErrorMessage(httpErrorCode);
    }
    // TODO
    // stanfordcpplib::getPlatform()->httpserver_sendResponse(event.getRequestID(), httpErrorCode, STATIC_VARIABLE(CONTENT_TYPE_ERROR), errorMessageActual);
}

void sendResponseFile(const GEvent& /*event*/, const std::string& responseFilePath,
                      const std::string& contentType) {
    if (!isRunning()) {
        error("HttpServer::sendResponse: server is not running");
    }
    std::string contentTypeActual = contentType;
    if (contentTypeActual.empty()) {
        contentTypeActual = getContentType(getExtension(responseFilePath));
    }
    // TODO
    // stanfordcpplib::getPlatform()->httpserver_sendResponseFile(event.getRequestID(), contentType, responseFilePath);
}


void startServer(int /*port*/) {
    if (!STATIC_VARIABLE(isRunning)) {
        // stanfordcpplib::getPlatform()->httpserver_start(port);
        STATIC_VARIABLE(isRunning) = true;
    }
}

void stopServer() {
    if (STATIC_VARIABLE(isRunning)) {
        STATIC_VARIABLE(isRunning) = false;
        // stanfordcpplib::getPlatform()->httpserver_stop();
    }
}
} // namespace HttpServer
