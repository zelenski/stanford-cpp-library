/*
 * File: gcolor.h
 * --------------
 *
 * @author Marty Stepp
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gcolor.h to replace Java version
 * @version 2018/06/30
 * - initial version
 */

#ifndef _gcolor_h
#define _gcolor_h

#include <string>
#include "map.h"
#include <QColor>

/**
 * This class provides static methods for dealing with colors.
 *
 * Many graphical interactors and objects accept colors specified as strings
 * or as RGB integers.  An RGB integer is an integer with red, green, and blue
 * values from 0-255 packed into its bits as follows:
 *
 * <pre>
 * 00000000 00000000 00000000 00000000
 *    ^        ^        ^        ^
 *    |        |        |        |
 *  alpha     red     green     blue
 * </pre>
 *
 * A color can also be specified as a string.
 * A color string can be one of the predefined color names:
 *
 *    <code>BLACK</code>,
 *    <code>BLUE</code>,
 *    <code>CYAN</code>,
 *    <code>DARK_GRAY</code>,
 *    <code>GRAY</code>,
 *    <code>GREEN</code>,
 *    <code>LIGHT_GRAY</code>,
 *    <code>MAGENTA</code>,
 *    <code>ORANGE</code>,
 *    <code>PINK</code>,
 *    <code>RED</code>,
 *    <code>WHITE</code>, and
 *    <code>YELLOW</code>.
 *
 * The case of the individual letters in the color name is ignored, as
 * are spaces and underscores, so that the color <code>DARK_GRAY</code>
 * can be written as <code>"Dark Gray"</code>.
 *
 * Lastly, the color can also be specified as a string in the form
 * <code>"#rrggbb"</code> where <code>rr</code>, <code>gg</code>, and
 * <code>bb</code> are pairs of hexadecimal digits indicating the
 * red, green, and blue components of the color, respectively.
 */
class GColor {
public:
    /**
     * Constants representing common system color names.
     */
    enum {
        BLACK = 0x000000,
        BLUE = 0x0000FF,
        BROWN = 0x926239,
        CYAN = 0x00FFFF,
        DARKGRAY = 0x595959,
        GRAY = 0x999999,
        GREEN = 0x00FF00,
        LIGHTGRAY = 0xBFBFBF,
        MAGENTA = 0xFF00FF,
        ORANGE = 0xFFC800,
        PINK = 0xFFAFAF,
        PURPLE = 0xFF00FF,
        RED = 0xFF0000,
        WHITE = 0xFFFFFF,
        YELLOW = 0xFFFF00
    } Color;

    /**
     * Converts four integer RGB values from 0-255 into a color name in the
     * form <code>"#aarrggbb"</code>.  Each of the <code>aa</code>, <code>rr</code>,
     * <code>gg</code>, and <code>bb</code> values are two-digit
     * hexadecimal numbers indicating the intensity of that component.
     * If any of a, r, g, or b is outside the range of 0-255, throws an error.
     */
    static std::string convertARGBToColor(int a, int r, int g, int b);

    /**
     * Converts four integer RGB values from 0-255 into a color name in the
     * form <code>"#aarrggbb"</code>.  Each of the <code>aa</code>, <code>rr</code>,
     * <code>gg</code>, and <code>bb</code> values are two-digit
     * hexadecimal numbers indicating the intensity of that component.
     * If any of a, r, g, or b is outside the range of 0-255, throws an error.
     */
    static int convertARGBToARGB(int a, int r, int g, int b);

    /**
     * Converts a color name into an integer that encodes the
     * red, green, and blue components of the color.
     */
    static int convertColorToRGB(const std::string& colorName);

    /**
     * Converts a Qt color object into a color string.
     */
    static std::string convertQColorToColor(const QColor& color);

    /**
     * Converts a Qt color object into an RGB integer.
     */
    static int convertQColorToRGB(const QColor& color);

    /**
     * Converts an <code>rgb</code> value into a color name in the
     * form <code>"#rrggbb"</code>.  Each of the <code>rr</code>,
     * <code>gg</code>, and <code>bb</code> values are two-digit
     * hexadecimal numbers indicating the intensity of that component.
     */
    static std::string convertRGBToColor(int rgb);

    /**
     * Converts three integer RGB values from 0-255 into a color name in the
     * form <code>"#rrggbb"</code>.  Each of the <code>rr</code>,
     * <code>gg</code>, and <code>bb</code> values are two-digit
     * hexadecimal numbers indicating the intensity of that component.
     * If any of r, g, or b is outside the range of 0-255, throws an error.
     */
    static std::string convertRGBToColor(int r, int g, int b);

    /**
     * Converts three integer RGB values from 0-255 into a single RGB integer.
     * Each of the <code>rr</code>, <code>gg</code>, and <code>bb</code> values
     * are two-digit hexadecimal numbers indicating the intensity of that component.
     * If any of r, g, or b is outside the range of 0-255, throws an error.
     */
    static int convertRGBToRGB(int r, int g, int b);

    /**
     * Converts a color string into a Qt color object.
     */
    static QColor toQColor(const std::string& color);

private:
    GColor();   // forbid construction

    /*
     * Strips casing and punctuation; e.g. "Dark Gray" => "darkgray"
     */
    static std::string canonicalColorName(const std::string& str);

    /*
     * Returns a reference to the table of common colors,
     * mapping from names to RGB integers.
     */
    static const Map<std::string, int>& colorTable();

    /*
     * Returns a reference to the table of common colors,
     * mapping from names to color strings.
     */
    static const Map<std::string, std::string>& colorNameTable();

    /*
     * Sets the 'alpha' (high order bits) of the given integer to ff.
     * If RGB is not completely black, but alpha is 0, assumes that the
     * client meant to use an opaque color and add ff as alpha channel.
     */
    static int fixAlpha(int argb);

    // internal color tables
    static Map<std::string, int> _colorTable;
    static Map<std::string, std::string> _colorNameTable;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gcolor_h
