/*
 * File: gfont.h
 * -------------
 *
 * @author Marty Stepp
 * @version 2019/04/30
 * - added changeFontSize for a GText*
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gfont.h to replace Java version
 * @version 2018/07/05
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gfont_h
#define _gfont_h

#include <string>
#include <QFont>

#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#define INTERNAL_INCLUDE 1
#include "gobjects.h"
#undef INTERNAL_INCLUDE

/**
 * This class contains static methods for dealing with fonts in our GUI system.
 * A font string has the following format:
 *
 * <pre>
 * "family-style-size"
 * </pre>
 *
 * where both <code>style</code> and <code>size</code> are optional.
 * If any of these elements are missing or specified as an asterisk,
 * the existing value is retained.
 */
class GFont {
public:
    /**
     * Makes the given interactor's font bold.
     * The font name and size are unchanged.
     * @throws ErrorException if the interactor is null
     */
    static void boldFont(GInteractor* interactor);

    /**
     * Modifies the font of the given interactor, changing its size by the given
     * number of points.  The change in size can be positive or negative.
     * @throws ErrorException if the interactor is null
     */
    static void changeFontSize(GInteractor* interactor, int dsize);

    /**
     * Modifies the font of the given label, changing its size by the given
     * number of points.  The change in size can be positive or negative.
     * @throws ErrorException if the interactor is null
     */
    static void changeFontSize(GText* label, int dsize);

    /**
     * Modifies the size of the given Qt font object, changing its size by the given
     * number of points, and returning the new modified font.
     * The change in size can be positive or negative.
     */
    static QFont changeFontSize(const QFont& font, int dsize);

    /**
     * Modifies the given font object, changing its weight and/or size to the
     * given values, and returning the new modified font.
     */
    static QFont deriveQFont(const QFont& font, QFont::Weight weight = QFont::Normal, int size = -1);

    /**
     * Modifies the given font object, changing its font family, weight, and/or
     * size to the given values, and returning the new modified font.
     */
    static QFont deriveQFont(const QFont& font, const std::string& fontFamily, QFont::Weight weight = QFont::Normal, int size = -1);

    /**
     * Modifies the given font object, changing its weight and/or size to the
     * given values, and returning the new modified font.
     * The font you pass should be a font string such as "Helvetica-12-Bold".
     */
    static QFont deriveQFont(const std::string& font, QFont::Weight weight = QFont::Normal, int size = -1);

    /**
     * Modifies the given font object, changing its font family, weight, and/or
     * size to the given values, and returning the new modified font.
     * The font you pass should be a font string such as "Helvetica-12-Bold".
     */
    static QFont deriveQFont(const std::string& font, const std::string& fontFamily, QFont::Weight weight = QFont::Normal, int size = -1);

    /**
     * Makes the given interactor's font italic.
     * The font name and size are unchanged.
     * @throws ErrorException if the interactor is null
     */
    static void italicFont(GInteractor* interactor);

    /**
     * Converts the given Qt font object into a font string such as
     * "Helvetica-12-Bold".
     */
    static std::string toFontString(const QFont& font);

    /**
     * Converts a font string such as "Helvetica-12-Bold" into a Qt font object.
     */
    static QFont toQFont(const std::string& fontString);

    /**
     * Converts a font string such as "Helvetica-*-12" into a Qt font object,
     * using the given 'basis' object for any defaults that are not present in
     * the font string.
     */
    static QFont toQFont(const QFont& basisFont, const std::string& fontString);

private:
    GFont();   // prevent construction
    static QFont::StyleHint getStyleHint(const std::string& fontFamily);
};

#endif // _gfont_h
