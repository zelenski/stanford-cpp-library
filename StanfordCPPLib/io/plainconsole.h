/*
 * File: plainconsole.h
 * --------------------
 * This file declares functions to add utility to the
 * C++ plain text console streams, cin/cout/cerr.
 * See plainconsole.cpp for implementation of each function.
 *
 * @author Marty Stepp
 * @version 2015/10/21
 * @since 2015/10/21
 */

#ifndef _plainconsole_h
#define _plainconsole_h

namespace plainconsole {
void setOutputLimit(int limit);
void setEcho(bool value);
} // namespace plainconsole

#endif // _plainconsole_h
