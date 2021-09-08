/*
 * File: require.h
 * ---------------
 *
 * This file contains assertion functions for argument checking within the
 * code of the Stanford C++ library itself.
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _require_h
#define _require_h

#include <string>

/**
 * @private
 */
namespace require {

void inRange(double value, double min, double max, const std::string& caller = "", const std::string& valueName = "", const std::string& details = "");
void inRange(int value, int min, int max, const std::string& caller = "", const std::string& valueName = "", const std::string& details = "");
void inRange2D(double x, double y, double maxX, double maxY, const std::string& caller = "", const std::string& xValueName = "", const std::string& yValueName = "", const std::string& details = "");
void inRange2D(double x, double y, double minX, double minY, double maxX, double maxY, const std::string& caller = "", const std::string& xValueName = "", const std::string& yValueName = "", const std::string& details = "");
void inRange2D(int x, int y, int maxX, int maxY, const std::string& caller = "", const std::string& xValueName = "", const std::string& yValueName = "", const std::string& details = "");
void inRange2D(int x, int y, int minX, int minY, int maxX, int maxY, const std::string& caller = "", const std::string& xValueName = "", const std::string& yValueName = "", const std::string& details = "");
void nonEmpty(const std::string& str, const std::string& caller = "", const std::string& valueName = "", const std::string& details = "");
void nonNegative(double value, const std::string& caller = "", const std::string& valueName = "", const std::string& details = "");
void nonNegative(int value, const std::string& caller = "", const std::string& valueName = "", const std::string& details = "");
void nonNegative(long value, const std::string& caller = "", const std::string& valueName = "", const std::string& details = "");
void nonNegative2D(double x, double y, const std::string& caller = "", const std::string& xValueName = "", const std::string& yValueName = "", const std::string& details = "");
void nonNegative2D(int x, int y, const std::string& caller = "", const std::string& xValueName = "", const std::string& yValueName = "", const std::string& details = "");
void nonNull(const void* ptr, const std::string& caller = "", const std::string& valueName = "", const std::string& details = "");
void positive(double value, const std::string& caller = "", const std::string& valueName = "", const std::string& details = "");
void positive(int value, const std::string& caller = "", const std::string& valueName = "", const std::string& details = "");
void require(bool test, const std::string& caller = "", const std::string& details = "");

} // namespace require

#endif // _require_h
