/*
 * File: require.cpp
 * -----------------
 * This file implements the require.h interface.
 *
 * @version 2018/09/05
 * - initial version
 */

#include "require.h"
#include <string>
#include "error.h"

namespace require {

static void _errorMessage(const std::string& caller, const std::string& valueName, const std::string& details) {
    std::string overallMessage = "";
    if (!caller.empty()) {
        overallMessage += caller;
    }
    if (!valueName.empty() || !details.empty()) {
        if (!overallMessage.empty()) {
            overallMessage += ": ";
        }
        if (!valueName.empty()) {
            if (!overallMessage.empty()) {
                overallMessage += " ";
            }
            overallMessage += valueName;
            if (!details.empty()) {
                overallMessage + " ";
            }
        }
        overallMessage += details;
    }
    error(overallMessage);
}

#define _spl_assert(test, caller, valueName, details) \
    if (!(test)) { \
        _errorMessage((caller), _default((valueName), "value"), (details)); \
    }

#define _default(value, defaultValue) ((value) == std::string("") ? (defaultValue) : (value))

void inRange(double value, double min, double max, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(min <= value && value <= max, caller, _default(valueName, "value"), _default(details, " must be between " + std::to_string(min) + " and " + std::to_string(max) + " inclusive but was " + std::to_string(value)));
}

void inRange(int value, int min, int max, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(min <= value && value <= max, caller, _default(valueName, "value"), _default(details, " must be between " + std::to_string(min) + " and " + std::to_string(max) + " inclusive but was " + std::to_string(value)));
}

void inRange2D(double x, double y, double maxX, double maxY, const std::string& caller, const std::string& xValueName, const std::string& yValueName, const std::string& details) {
    inRange2D(x, y, /* minX */ 0.0, /* minY */ 0.0, maxX, maxY, caller, xValueName, yValueName, details);
}

void inRange2D(double x, double y, double minX, double minY, double maxX, double maxY, const std::string& caller, const std::string& xValueName, const std::string& yValueName, const std::string& details) {
    inRange(x, minX, maxX, caller, xValueName, details);
    inRange(y, minY, maxY, caller, yValueName, details);
}

void inRange2D(int x, int y, int maxX, int maxY, const std::string& caller, const std::string& xValueName, const std::string& yValueName, const std::string& details) {
    inRange2D(x, y, /* minX */ 0, /* minY */ 0, maxX, maxY, caller, xValueName, yValueName, details);
}

void inRange2D(int x, int y, int minX, int minY, int maxX, int maxY, const std::string& caller, const std::string& xValueName, const std::string& yValueName, const std::string& details) {
    inRange(x, minX, maxX, caller, xValueName, details);
    inRange(y, minY, maxY, caller, yValueName, details);
}

void nonEmpty(const std::string& str, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(!str.empty(), caller, _default(valueName, "string"), _default(details, " must not be empty"));
}

void nonNegative(double value, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(value >= 0.0, caller, _default(valueName, "value"), _default(details, " must be non-negative but was " + std::to_string(value)));
}

void nonNegative(int value, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(value >= 0, caller, _default(valueName, "value"), _default(details, " must be non-negative but was " + std::to_string(value)));
}

void nonNegative(long value, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(value >= 0, caller, _default(valueName, "value"), _default(details, " must be non-negative but was " + std::to_string(value)));
}

void nonNegative2D(double x, double y, const std::string& caller, const std::string& xValueName, const std::string& yValueName, const std::string& details) {
    nonNegative(x, caller, xValueName, details);
    nonNegative(y, caller, yValueName, details);
}

void nonNegative2D(int x, int y, const std::string& caller, const std::string& xValueName, const std::string& yValueName, const std::string& details) {
    nonNegative(x, caller, xValueName, details);
    nonNegative(y, caller, yValueName, details);
}

void nonNull(const void* ptr, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(ptr != nullptr, caller, _default(valueName, "value"), _default(details, " must be non-null"));
}

void positive(double value, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(value > 0.0, caller, _default(valueName, "value"), _default(details, " must be positive but was " + std::to_string(value)));
}

void positive(int value, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(value > 0, caller, _default(valueName, "value"), _default(details, " must be positive but was " + std::to_string(value)));
}

void require(bool test, const std::string& caller, const std::string& details) {
    _spl_assert(test, caller, "", details);
}

} // namespace require
