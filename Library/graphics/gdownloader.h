/*
 * File: gdownloader.h
 * -------------------
 * A GDownloader can download data from URLs and save them to files or return
 * the data as a string.
 *
 * Note that because the downloader uses a pure-C++ implementation, your project
 * must include the 'network' component of Qt to function properly.
 * If you get errors when trying to connect to HTTPS URLs, you may also need to
 * install various SSL packages on your system, such as openssl, libssl-dev,
 * libssl1.0, and so on.  This varies by operating system.
 *
 * Based somewhat on this source:
 * https://wiki.qt.io/Download_Data_from_URL
 *
 * @author Marty Stepp
 * @version 2018/09/18
 * - working version; had to fix various threading / Qt signal issues
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gdownloader.h to replace Java version
 * @version 2018/08/03
 * - initial version
 */


#ifndef _gdownloader_h
#define _gdownloader_h

#include <string>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "map.h"

/**
 * A GDownloader can download files and data over an internet connection.
 * It can save the data to a file or return the data as a string.
 */
class GDownloader : public QObject {
    Q_OBJECT

public:
    /**
     * Creates a new downloader.
     */
    GDownloader();

    /**
     * Frees memory allocated internally by the downloader.
     */
    virtual ~GDownloader();

    /**
     * Downloads the text contents of the given URL, returning them as a string.
     * This method blocks until the data is finished downloading.
     */
    std::string downloadAsString(const std::string& url);

    /**
     * Downloads the text contents of the given URL, saving it to the given output file.
     * This method blocks until the data is finished downloading.
     */
    void downloadToFile(const std::string& url, const std::string& file);

    /**
     * Returns the last HTTP error message that occurred.
     * If no HTTP errors have occurred, returns "".
     */
    std::string getErrorMessage() const;

    /**
     * Returns the value of the given HTTP header for this URL request.
     * If the given header is not defined, returns an empty string.
     */
    std::string getHeader(const std::string& name) const;

    /**
     * Returns the most recent HTTP status code, which may be a successful
     * code (e.g. 200) or an error (e.g 404). If there is no HTTP status
     * code to return, returns 0.
     */
    int getHttpStatusCode() const;

    /**
     * Returns the value of the HTTP "User-Agent" header for this URL request,
     * or an empty string if the user agent has not been set.
     */
    std::string getUserAgent() const;

    /**
     * Returns true if the HTTP connection failed and had an error.
     * You can see what the error was by calling getErrorMessage.
     */
    bool hasError() const;

    /**
     * Performs an HTTP GET request to the given URL.
     * along with any headers previously specified.
     */
    void httpGet(const std::string& url);

    /**
     * Performs an HTTP POST request to the given URL,
     * submitting any headers and query parameters previously specified.
     */
    void httpPost(const std::string& url);

    /**
     * Sets the value of the given HTTP header for this URL request.
     * Must be called before open(), and the stream must have been created
     * with the parameterless constructor.
     *
     * @example stream.setHeader("Referer", "http://cs106b.stanford.edu/");
     */
    void setHeader(const std::string& name, const std::string& value);

    /**
     * Sets the value of the HTTP "User-Agent" header for this URL request.
     * Must be called before open(), and the stream must have been created
     * with the parameterless constructor.
     * Equivalent to calling setHeader with "User-Agent" as the header's name.
     *
     * @example stream.setUserAgent("Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.4; en-US; rv:1.9.2.2) Gecko/20100316 Firefox/3.6.2");
     */
    void setUserAgent(const std::string& userAgent);

signals:
    /**
     * This Qt signal fires when the data is done downloading.
     */
    void downloaded();

private slots:
    void downloadInternal();
    void fileDownloadError(QNetworkReply::NetworkError);
    void reportNoSSL();
    void saveDownloadedData(const std::string& member, const std::string& filename = "");
    void sslErrorsNam(QNetworkReply*, QList<QSslError>);
    void sslErrorsReply(QList<QSslError>);
    void waitForDownload();
private:
    Q_DISABLE_COPY(GDownloader)

    static std::string qtNetworkErrorToString(QNetworkReply::NetworkError nerror);

    QNetworkAccessManager* _manager;
    QNetworkReply* _reply;
    Map<std::string, std::string> _headers;   // HTTP headers to send (name => value)
    int _httpStatusCode;
    bool _downloadComplete;
    std::string _url;
    std::string _filename;
    std::string _filedata;
    std::string _lastErrorMessage;
};

#endif // _gdownloader_h
