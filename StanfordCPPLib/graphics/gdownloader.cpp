/*
 * File: gdownloader.cpp
 * ---------------------
 * This file implements the GDownloader class as declared in gdownloader.h.
 * See the .h file for the declarations of each member and comments.
 *
 * @author Marty Stepp
 * @version 2018/09/23
 * - added macro checks to improve compatibility with old Qt versions
 * @version 2018/09/18
 * - working version; had to fix various threading / Qt signal issues
 * @version 2018/08/23
 * - renamed to gdownloader.cpp to replace Java version
 * @version 2018/08/03
 * - initial version
 */

#include "gdownloader.h"
#include <iomanip>
#include <iostream>
#include <QtGlobal>
#include <QFile>
#include <QIODevice>
#include <QTimer>
#include "error.h"
#include "gthread.h"

GDownloader::GDownloader()
        : _manager(nullptr),
          _reply(nullptr),
          _httpStatusCode(0),
          _downloadComplete(false) {
}

GDownloader::~GDownloader() {
    // TODO: delete
    _manager = nullptr;
    _reply = nullptr;
}

std::string GDownloader::downloadAsString(const std::string& url) {
    _url = url;
    _filename = "";
    _httpStatusCode = 0;
    _lastErrorMessage = "";

    // actually download the file (block/wait for it to finish)
    downloadInternal();

    // save download to string
    saveDownloadedData("downloadAsString");

    // return downloaded text as string (saved in member variable)
    return _filedata;
}

void GDownloader::downloadToFile(const std::string& url, const std::string& file) {
    _url = url;
    _filename = file;
    _httpStatusCode = 0;
    _lastErrorMessage = "";

    // actually download the file (block/wait for it to finish)
    downloadInternal();

    // save download to file
    saveDownloadedData("downloadToFile", file);
}

void GDownloader::downloadInternal() {
    GThread::runOnQtGuiThreadAsync([this]() {
        if (!_manager) {
            _manager = new QNetworkAccessManager();
            // disabling Qt signal-handling because it doesn't seem to work at all
            // connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));
        }
        QNetworkRequest* request = new QNetworkRequest(QUrl(QString::fromStdString(_url)));

        // set up SSL / HTTPS settings, if possible
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
#if QT_CONFIG(ssl)
        request->setSslConfiguration(QSslConfiguration::defaultConfiguration());
#endif // QT_CONFIG(ssl)
#endif // QT_VERSION

        for (std::string headerKey : _headers) {
            request->setRawHeader(QByteArray(headerKey.c_str()), QByteArray(_headers[headerKey].c_str()));
        }

        _reply = _manager->get(*request);

        // disabling Qt signal-handling because it doesn't seem to work at all
        // _reply->connect(
        //         _reply, SIGNAL(finished()),
        //         this, SLOT(fileDownloadFinished()));

        // this seemingly won't be called ever (thanks, useless Qt networking tutorials!)
        _reply->connect(
                _reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(fileDownloadError(QNetworkReply::NetworkError)));

#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
#if QT_CONFIG(ssl)
        _reply->connect(
                _reply, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(sslErrors(QList<QSslError>)));
#endif // QT_CONFIG(ssl)
#endif // QT_VERSION
    });

    // wait for download to finish (in student thread)
    waitForDownload();
}

void GDownloader::fileDownloadError(QNetworkReply::NetworkError nerror) {
    error("file download error: " + std::to_string(nerror));
}

std::string GDownloader::getErrorMessage() const {
    return _lastErrorMessage;
}

int GDownloader::getHttpStatusCode() const {
    // all HTTP status codes are between 1xx and 5xx, inclusive
    return _httpStatusCode >= 100 && _httpStatusCode <= 599 ? _httpStatusCode : 0;
}

std::string GDownloader::getHeader(const std::string& name) const {
    return _headers[name];
}

std::string GDownloader::getUserAgent() const {
    if (_headers.containsKey("User-Agent")) {
        return _headers["User-Agent"];
    } else {
        return "";
    }
}

bool GDownloader::hasError() const {
    if (_httpStatusCode != 0) {
        // values 2xx indicate success
        return _httpStatusCode < 200 || _httpStatusCode > 299;
    } else {
        return _lastErrorMessage.empty();
    }
}

std::string GDownloader::qtNetworkErrorToString(QNetworkReply::NetworkError nerror) {
    // http://doc.qt.io/qt-5/qnetworkreply.html#NetworkError-enum
    switch (nerror) {
    case QNetworkReply::ConnectionRefusedError: return "the remote server refused the connection (the server is not accepting requests)";
    case QNetworkReply::RemoteHostClosedError: return "the remote server closed the connection prematurely, before the entire reply was received and processed";
    case QNetworkReply::HostNotFoundError: return "the remote host name was not found (invalid hostname)";
    case QNetworkReply::TimeoutError: return "the connection to the remote server timed out";
    case QNetworkReply::OperationCanceledError: return "the operation was canceled via calls to abort() or close() before it was finished.";
    case QNetworkReply::SslHandshakeFailedError: return "the SSL/TLS handshake failed and the encrypted channel could not be established. The sslErrors() signal should have been emitted.";
    case QNetworkReply::TemporaryNetworkFailureError: return "the connection was broken due to disconnection from the network, however the system has initiated roaming to another access point. The request should be resubmitted and will be processed as soon as the connection is re-established.";
    case QNetworkReply::NetworkSessionFailedError: return "the connection was broken due to disconnection from the network or failure to start the network.";
    case QNetworkReply::BackgroundRequestNotAllowedError: return "the background request is not currently allowed due to platform policy.";
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
    case QNetworkReply::TooManyRedirectsError: return "while following redirects, the maximum limit was reached. The limit is by default set to 50 or as set by QNetworkRequest::setMaxRedirectsAllowed(). (This value was introduced in 5.6.)";
    case QNetworkReply::InsecureRedirectError: return "while following redirects, the network access API detected a redirect from a encrypted protocol (https) to an unencrypted one (http). (This value was introduced in 5.6.)";
#endif // QT_VERSION
    case QNetworkReply::ProxyConnectionRefusedError: return "the connection to the proxy server was refused (the proxy server is not accepting requests)";
    case QNetworkReply::ProxyConnectionClosedError: return "the proxy server closed the connection prematurely, before the entire reply was received and processed";
    case QNetworkReply::ProxyNotFoundError: return "the proxy host name was not found (invalid proxy hostname)";
    case QNetworkReply::ProxyTimeoutError: return "the connection to the proxy timed out or the proxy did not reply in time to the request sent";
    case QNetworkReply::ProxyAuthenticationRequiredError: return "the proxy requires authentication in order to honour the request but did not accept any credentials offered (if any)";
    case QNetworkReply::ContentAccessDenied: return "access denied";
    case QNetworkReply::ContentOperationNotPermittedError: return "the operation requested on the remote content is not permitted";
    case QNetworkReply::ContentNotFoundError: return "the remote content was not found at the server";
    case QNetworkReply::AuthenticationRequiredError: return "the remote server requires authentication to serve the content but the credentials provided were not accepted (if any)";
    case QNetworkReply::ContentReSendError: return "the request needed to be sent again, but this failed for example because the upload data could not be read a second time.";
    case QNetworkReply::ContentConflictError: return "the request could not be completed due to a conflict with the current state of the resource.";
    case QNetworkReply::ContentGoneError: return "the requested resource is no longer available at the server.";
    case QNetworkReply::InternalServerError: return "the server encountered an unexpected condition which prevented it from fulfilling the request.";
    case QNetworkReply::OperationNotImplementedError: return "the server does not support the functionality required to fulfill the request.";
    case QNetworkReply::ServiceUnavailableError: return "the server is unable to handle the request at this time.";
    case QNetworkReply::ProtocolUnknownError: return "the Network Access API cannot honor the request because the protocol is not known";
    case QNetworkReply::ProtocolInvalidOperationError: return "the requested operation is invalid for this protocol";
    case QNetworkReply::UnknownNetworkError: return "an unknown network-related error was detected";
    case QNetworkReply::UnknownProxyError: return "an unknown proxy-related error was detected";
    case QNetworkReply::UnknownContentError: return "an unknown error related to the remote content was detected";
    case QNetworkReply::ProtocolFailure: return "a breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.)";
    case QNetworkReply::UnknownServerError: return "unknown server error";
    case QNetworkReply::NoError:
        break;
    }
    return "";
}

void GDownloader::saveDownloadedData(const std::string& member, const std::string& filename) {
    if (_reply) {
        QNetworkReply::NetworkError nerror = _reply->error();
        if (nerror) {
            // connection failed; log the error message
            _lastErrorMessage = qtNetworkErrorToString(nerror);
        } else if (filename.empty()) {
            // save to a string
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
            _filedata = _reply->readAll().toStdString();
#endif // QT_VERSION
        } else {
            // save to a file
            QFile outfile(QString::fromStdString(filename));
            if (!outfile.open(QIODevice::WriteOnly)) {
                error("GDownloader::" + member + ": cannot open file " + filename + " for writing");
            }
            outfile.write(_reply->readAll());
            outfile.close();
        }

        // clean up the connection
        _reply->deleteLater();
        _reply = nullptr;
        _downloadComplete = true;
    }
}

void GDownloader::setHeader(const std::string& name, const std::string& value) {
    _headers[name] = value;
}

void GDownloader::setUserAgent(const std::string& userAgent) {
    setHeader("User-Agent", userAgent);
}

void GDownloader::sslErrors(QList<QSslError>) {
    std::cout << "  DEBUG: sslErrors" << std::endl;
}

void GDownloader::waitForDownload() {
    // wait for download to finish
    while (!_downloadComplete) {
        GThread::getCurrentThread()->sleep(10);
        if (_reply && _reply->isFinished()) {
            _downloadComplete = true;
            break;
        }
    }

    // grab the HTTP status code
    QVariant statusCode = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (statusCode.isValid()) {
        _httpStatusCode = statusCode.toInt();
        _lastErrorMessage = _reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString().toStdString();
    } else {
        _httpStatusCode = -1;
        _lastErrorMessage = "Unable to connect to URL";
    }
}


