/*
 * File: base64.h
 * --------------
 * This file declares a set of functions for encoding and decoding binary data
 * in the base64 format.  See:
 * http://en.wikipedia.org/wiki/Base64
 *
 * @author Marty Stepp, based upon open-source Apache Base64 en/decoder
 * @version 2014/08/03
 * @since 2014/08/03
 */

#ifndef _base64_h
#define _base64_h


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int Base64encode_len(int len);
int Base64encode(char* coded_dst, const char* plain_src, int len_plain_src);

int Base64decode_len(const char* coded_src);
int Base64decode(char* plain_dst, const char* coded_src);

#ifdef __cplusplus
}

#include <string>

namespace Base64 {
/*
 * Returns a Base64-encoded equivalent of the given string.
 */
std::string encode(const std::string& s);

/*
 * Decodes the given Base64-encoded string and returns the decoded
 * original contents.
 */
std::string decode(const std::string& s);
}
#endif // __cplusplus


#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _base64_h
