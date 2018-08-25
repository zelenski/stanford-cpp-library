/*
 * File: gdownloader.cpp
 * ---------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gdownloader.cpp to replace Java version
 * @version 2018/08/03
 * - initial version
 */

#include "gdownloader.h"
#include <iomanip>
#include <iostream>
#include <QFile>
#include <QIODevice>
#include <QTimer>
#include "error.h"
#include "gthread.h"
#include "strlib.h"

GDownloader::GDownloader()
        : _manager(nullptr),
          _reply(nullptr),
          _lastError(0),
          _downloadComplete(false) {
}

GDownloader::~GDownloader() {
    // empty
}

void GDownloader::downloadToFile(const std::string& url, const std::string& file) {
    // std::cout << "  DEBUG: downloadToFile " << url << " => " << file << std::endl;
    _url = url;
    _filename = file;

    // TODO: fix (does not work)
    // QTimer::singleShot(0, this, SLOT(downloadInternal()));
    // QGThread::runInNewThread([this]() {
    //     this->downloadInternal();
    // });
}

void GDownloader::downloadInternal() {
    // std::cout << "  DEBUG: downloadInternal " << _url << " => " << _filename << std::endl;
    if (!_manager) {
        _manager = new QNetworkAccessManager();
        connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));
    }
    QNetworkRequest* request = new QNetworkRequest(QUrl(QString::fromStdString(_url)));
    for (std::string headerKey : _headers) {
        request->setRawHeader(QByteArray(headerKey.c_str()), QByteArray(_headers[headerKey].c_str()));
    }
    // std::cout << "  DEBUG: downloadToFile about to get()" << std::endl;
    this->_reply = this->_manager->get(*request);
    this->_reply->connect(this->_reply, SIGNAL(finished()), this, SLOT(fileDownloadFinished()));
    this->_reply->connect(this->_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(fileDownloadError(QNetworkReply::NetworkError)));
    // std::cout << "  DEBUG: downloadToFile errorString = " << this->_reply->error() << " " << this->_reply->errorString().toStdString() << std::endl;
    // std::cout << "  DEBUG: downloadToFile done with get(), waiting for finish" << std::endl;

    // wait for download to finish
    GThread::yield();
    while (!_downloadComplete) {
        GThread::sleep(10);
    }
}

std::string GDownloader::downloadAsString(const std::string& url) {
    downloadToFile(url, /* filename */ "");
    // TODO
    return "";
}

void GDownloader::fileDownloaded(QNetworkReply* reply) {
    // std::cout << "  DEBUG: fileDownloaded " << reply->url().toString().toStdString() << std::endl;
    if (_filename.empty()) {
        _filedata = reply->readAll().toStdString();
    } else {
        QFile localFile(QString::fromStdString(_filename));
        if (!localFile.open(QIODevice::WriteOnly)) {
            return;
        }
        localFile.write(reply->readAll());
        localFile.close();
    }
    reply->deleteLater();
    _downloadComplete = true;
}

void GDownloader::fileDownloadError(QNetworkReply::NetworkError /*error*/) {
    // std::cout << "  DEBUG: fileDownloadError " << error << std::endl;
}

void GDownloader::fileDownloadFinished() {
    // std::cout << "  DEBUG: fileDownloadFinished " << _reply->url().toString().toStdString() << std::endl;
    if (_filename.empty()) {
        _filedata = _reply->readAll().toStdString();
    } else {
        QFile localFile(QString::fromStdString(_filename));
        if (!localFile.open(QIODevice::WriteOnly)) {
            return;
        }
        localFile.write(_reply->readAll());
        localFile.close();
    }
    _reply->deleteLater();
    _reply = nullptr;
    _downloadComplete = true;
}

int GDownloader::getHttpStatusCode() const {
    // all HTTP status codes are between 1xx and 5xx, inclusive
    return _lastError >= 100 && _lastError <= 599 ? _lastError : 0;
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

void GDownloader::setHeader(const std::string& name, const std::string& value) {
    _headers[name] = value;
}

void GDownloader::setUserAgent(const std::string& userAgent) {
    setHeader("User-Agent", userAgent);
}
