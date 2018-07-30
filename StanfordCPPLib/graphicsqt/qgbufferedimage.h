/*
 * File: qgbufferedimage.h
 * -----------------------
 * This file exports the QGBufferedImage class for per-pixel graphics.
 * See qgbufferedimage.cpp for implementation of each member.
 *
 * @author Marty Stepp
 * @version 2018/07/12
 * - refactored to be an alias for QGCanvas, which contains a superset of its functionality
 * @version 2018/07/07
 * - initial version based on GBufferedImage source
 */

#ifdef SPL_QT_GUI
#ifndef _qgbufferedimage_h
#define _qgbufferedimage_h

#include "qgcanvas.h"

// QGBufferedImage is just an alias for QGCanvas
typedef QGCanvas QGBufferedImage;

// default color used to highlight pixels that do not match between two images
#define QGBUFFEREDIMAGE_DEFAULT_DIFF_PIXEL_COLOR 0xdd00dd

//    // allow operators to see private data inside image
//    friend bool operator ==(const QGBufferedImage& img1, const QGBufferedImage& img2);
//    friend bool operator !=(const QGBufferedImage& img1, const QGBufferedImage& img2);

//bool operator ==(const QGBufferedImage& img1, const QGBufferedImage& img2);
//bool operator !=(const QGBufferedImage& img1, const QGBufferedImage& img2);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgbufferedimage_h
#endif // SPL_QT_GUI
