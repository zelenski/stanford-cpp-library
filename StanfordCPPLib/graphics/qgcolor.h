/*
 * File: qgcolor.h
 * ---------------
 *
 * @version 2018/06/30
 * - initial version
 */

#ifndef _qgcolor_h
#define _qgcolor_h

#include <string>
#include "map.h"

/*
 * ...
 */
class QGColor {
public:
    /*
     * Function: convertRGBToColor
     * Usage: int colorName = convertRGBToColor(a, r, g, b);
     * -----------------------------------------------------
     * Converts four integer RGB values from 0-255 into a color name in the
     * form <code>"#aarrggbb"</code>.  Each of the <code>aa</code>, <code>rr</code>,
     * <code>gg</code>, and <code>bb</code> values are two-digit
     * hexadecimal numbers indicating the intensity of that component.
     * If any of a, r, g, or b is outside the range of 0-255, throws an error.
     */
    static std::string convertARGBToColor(int a, int r, int g, int b);

    /*
     * Function: convertColorToRGB
     * Usage: int rgb = convertColorToRGB(colorName);
     * ----------------------------------------------
     * Converts a color name into an integer that encodes the
     * red, green, and blue components of the color.
     */
    static int convertColorToRGB(const std::string& colorName);

    /*
     * Function: convertRGBToColor
     * Usage: int colorName = convertRGBToColor(rgb);
     * ----------------------------------------------
     * Converts an <code>rgb</code> value into a color name in the
     * form <code>"#rrggbb"</code>.  Each of the <code>rr</code>,
     * <code>gg</code>, and <code>bb</code> values are two-digit
     * hexadecimal numbers indicating the intensity of that component.
     */
    static std::string convertRGBToColor(int rgb);

    /*
     * Function: convertRGBToColor
     * Usage: int colorName = convertRGBToColor(r, g, b);
     * --------------------------------------------------
     * Converts three integer RGB values from 0-255 into a color name in the
     * form <code>"#rrggbb"</code>.  Each of the <code>rr</code>,
     * <code>gg</code>, and <code>bb</code> values are two-digit
     * hexadecimal numbers indicating the intensity of that component.
     * If any of r, g, or b is outside the range of 0-255, throws an error.
     */
    static std::string convertRGBToColor(int r, int g, int b);

    static int convertARGBToARGB(int a, int r, int g, int b);
    static int convertRGBToRGB(int r, int g, int b);

private:
    QGColor();

    static std::string canonicalColorName(const std::string& str);
    static Map<std::string, int>& colorTable();
    static int fixAlpha(int argb);

    static Map<std::string, int> _colorTable;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgcolor_h
