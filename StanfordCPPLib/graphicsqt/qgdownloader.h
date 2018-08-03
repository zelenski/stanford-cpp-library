/*
 * File: qgdownloader.h
 * --------------------
 *
 * Based somewhat on this source:
 * https://wiki.qt.io/Download_Data_from_URL
 *
 * @version 2018/08/03
 * - initial version
 */

#ifdef SPL_QT_GUI
#ifndef _qgdownloader_h
#define _qgdownloader_h

#include <string>
#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include "map.h"

/*
 * ...
 */
class QGDownloader : public QObject {
    Q_OBJECT

public:
    QGDownloader();
    virtual ~QGDownloader();

    std::string downloadAsString(const std::string& url);

    void downloadToFile(const std::string& url, const std::string& file);

    /*
     * Returns the value of the given HTTP header for this URL request.
     * If the given header is not defined, returns an empty string.
     */
    std::string getHeader(const std::string& name) const;

    /*
     * Returns the most recent HTTP status code, which may be a successful
     * code (e.g. 200) or an error (e.g 404). If there is no HTTP status
     * code to return, returns 0.
     */
    int getHttpStatusCode() const;

    /*
     * Returns the value of the HTTP "User-Agent" header for this URL request,
     * or an empty string if the user agent has not been set.
     */
    std::string getUserAgent() const;

    void httpGet(const std::string& url);
    void httpPost(const std::string& url);

    /*
     * Sets the value of the given HTTP header for this URL request.
     * Must be called before open(), and the stream must have been created
     * with the parameterless constructor.
     *
     * Example:
     * stream.setHeader("Referer", "http://cs106b.stanford.edu/");
     */
    void setHeader(const std::string& name, const std::string& value);

    /*
     * Sets the value of the HTTP "User-Agent" header for this URL request.
     * Must be called before open(), and the stream must have been created
     * with the parameterless constructor.
     * Equivalent to calling setHeader with "User-Agent" as the header's name.
     *
     * Example:
     * stream.setUserAgent("Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.4; en-US; rv:1.9.2.2) Gecko/20100316 Firefox/3.6.2");
     */
    void setUserAgent(const std::string& userAgent);

signals:
    void downloaded();

private slots:
    void downloadInternal();
    void fileDownloaded(QNetworkReply* reply);
    void fileDownloadError(QNetworkReply::NetworkError);
    void fileDownloadFinished();

private:
    QNetworkAccessManager* _manager;
    QNetworkReply* _reply;
    Map<std::string, std::string> _headers;   // HTTP headers to send (name => value)
    int _lastError;
    bool _downloadComplete;
    std::string _url;
    std::string _filename;
    std::string _filedata;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgdownloader_h
#endif // SPL_QT_GUI
