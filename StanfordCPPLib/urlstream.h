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
 * @version 2014/10/08
 * @since 2014/10/08
 */

#ifndef _urlstream_h
#define _urlstream_h

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class iurlstream : public std::ifstream {
public:
    /*
     * Constructor: iurlstream
     * Usage: iurlstream stream;
     * -------------------------
     * Initializes a new iurlstream that is not attached to any source.
     */
    iurlstream();

    /*
     * Constructor: iurlstream
     * Usage: iurlstream stream("http://www.google.com/");
     * ---------------------------------------------------
     * Initializes a new iurlstream that is attached to the given source URL.
     */
    iurlstream(std::string url);

    /*
     * Closes the currently-opened URL connection, if it is open.
     * If the stream is not open, puts the stream into a fail state.
     */
    void close();

    /*
     * Returns the most recent HTTP error code received, if any.
     * Returns 0 if no error codes have been issued.
     */
    int getErrorCode() const;

    /*
     * Opens the given URL for reading.
     */
    void open(std::string url = "");

private:
    std::string m_url;            // URL to be opened
    std::string m_tempFilePath;   // local temporary file path where the URL data is downloaded
    int m_lastError;              // most recent HTTP error seen, if any (initially 0)
    
    /*
     * Returns the 'tail' of the given URL, for use as a file name.
     * Slightly different from filelib's getTail because it strips URL query strings.
     * e.g. getUrlFilename("http://foobar.com/junk/page.php?foo=bar&baz=qux") returns "page.php".
     */
    std::string getUrlFilename(std::string url) const;
};

/* return type */
typedef enum {
    /* Client side errors */
    ERRHOST=-1, /* No such host */
    ERRSOCK=-2, /* Can't create socket */
    ERRCONN=-3, /* Can't connect to host */
    ERRWRHD=-4, /* Write error on socket while writing header */
    ERRWRDT=-5, /* Write error on socket while writing data */
    ERRRDHD=-6, /* Read error on socket while reading result */
    ERRPAHD=-7, /* Invalid answer from data server */
    ERRNULL=-8, /* Null data pointer */
    ERRNOLG=-9, /* No/Bad length in header */
    ERRMEM=-10, /* Can't allocate memory */
    ERRRDDT=-11,/* Read error while reading data */
    ERRURLH=-12,/* Invalid url - must start with 'http://' */
    ERRURLP=-13,/* Invalid port in url */
    
    ERR_MALFORMED_URL = -42,
    ERR_IO_EXCEPTION = -43,


    /* Return code by the server */
    ERR400=400, /* Invalid query */
    ERR403=403, /* Forbidden */
    ERR408=408, /* Request timeout */
    ERR500=500, /* Server error */
    ERR501=501, /* Not implemented */
    ERR503=503, /* Service overloaded */

    /* Succesful results */
    OK0 = 0,   /* successfull parse */
    OK201=201, /* Ressource succesfully created */
    OK200=200  /* Ressource succesfully read */

} http_retcode;

#endif // _urlstream_h
