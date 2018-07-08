/*
 * File: urlstream.h
 * -----------------
 * This file exports the <code>iurlstream</code> class, which
 * implements an input stream for reading data from URLs.
 * It is implemented as a thin wrapper around a file input stream (ifstream).
 * The implementation simply downloads the URL's data using Java's java.net.URL class
 * and saves it into a temporary file, then reads from a stream on that file.
 * Upon close()ing the iurlstream, the temporary file is deleted.
 * This is not an ideal implementation, but it is platform independent and easier to get working.
 * 
 * @author Marty Stepp
 * @version 2018/06/20
 * - support for setting headers such as user agent
 * - https URL support
 * - changed string to const string&
 * @version 2014/10/08
 * - initial version
 * @since 2014/10/08
 */

#ifndef _urlstream_h
#define _urlstream_h

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "map.h"

class iurlstream : public std::ifstream {
public:
    /*
     * Constructor: iurlstream
     * Usage: iurlstream stream;
     * -------------------------
     * Initializes a new iurlstream that is not attached to any source.
     * Use this constructor if you want to set various properties of the URL
     * connection (such as user agent or other headers) before downloading.
     */
    iurlstream();

    /*
     * Constructor: iurlstream
     * Usage: iurlstream stream("http://www.google.com/");
     * ---------------------------------------------------
     * Initializes a new iurlstream that is attached to the given source URL.
     * The data from that URL is downloaded immediately.
     */
    iurlstream(const std::string& url);

    /*
     * Closes the currently-opened URL connection, if it is open.
     * If the stream is not open, puts the stream into a fail state.
     */
    void close();

    /*
     * Returns the most recent error code received, if any.
     * Returns 0 if no error codes have been issued.
     */
    int getErrorCode() const;

    /*
     * Returns the most recent HTTP status code, which may be a successful
     * code (e.g. 200) or an error (e.g 404). If there is no HTTP status
     * code to return, returns 0.
     */
    int getHttpStatusCode() const;

    /*
     * Returns the value of the given HTTP header for this URL request.
     * If the given header is not defined, returns an empty string.
     */
    std::string getHeader(const std::string& name) const;

    /*
     * Returns the URL sent to the stream's constructor or to the last call
     * to open(...).
     */
    std::string getUrl() const;

    /*
     * Returns the value of the HTTP "User-Agent" header for this URL request,
     * or an empty string if the user agent has not been set.
     */
    std::string getUserAgent() const;

    /*
     * Opens the given URL for reading.
     */
    void open(const std::string& url = "");

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

private:
    std::string m_url;            // URL to be opened
    std::string m_tempFilePath;   // local temporary file path where the URL data is downloaded
    int m_lastError;              // most recent HTTP error seen, if any (initially 0)
    Map<std::string, std::string> m_headers;   // HTTP headers to send (name => value)
    
    /*
     * Returns the 'tail' of the given URL, for use as a file name.
     * Slightly different from filelib's getTail because it strips URL query strings.
     * e.g. getUrlFilename("http://foobar.com/junk/page.php?foo=bar&baz=qux") returns "page.php".
     */
    std::string getUrlFilename(const std::string& url) const;
};

// return type
typedef enum {
    // client side errors
    ERRHOST = -1,    // no such host
    ERRSOCK = -2,    // can't create socket
    ERRCONN = -3,    // can't connect to host
    ERRWRHD = -4,    // write error on socket while writing header
    ERRWRDT = -5,    // write error on socket while writing data
    ERRRDHD = -6,    // read error on socket while reading result
    ERRPAHD = -7,    // invalid answer from data server
    ERRNULL = -8,    // null data pointer
    ERRNOLG = -9,    // no/bad length in header
    ERRMEM  = -10,   // can't allocate memory
    ERRRDDT = -11,   // read error while reading data
    ERRURLH = -12,   // invalid url - must start with 'http://'
    ERRURLP = -13,   // invalid port in url
    
    ERR_MALFORMED_URL = -42,
    ERR_IO_EXCEPTION = -43,


    // return code by the server
    ERR400 = 400,   // invalid query
    ERR403 = 403,   // forbidden
    ERR408 = 408,   // request timeout
    ERR500 = 500,   // server error
    ERR501 = 501,   // not implemented
    ERR503 = 503,   // service overloaded

    // successful results
    OK0 = 0,     // successful parse
    OK201 = 201, // resource succesfully created
    OK200 = 200  // resource succesfully read

} http_retcode;

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _urlstream_h
