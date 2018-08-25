/*
 * File: qbufferedimage.h
 * ----------------------
 * This file exports the GBufferedImage class for per-pixel graphics.
 * See qgbufferedimage.cpp for implementation of each member.
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gbufferedimage.h to replace Java version
 * @version 2018/07/12
 * - refactored to be an alias for GCanvas, which contains a superset of its functionality
 * @version 2018/07/07
 * - initial version based on GBufferedImage source
 */

#ifndef _gbufferedimage_h
#define _gbufferedimage_h

#include "gcanvas.h"

// QGBufferedImage is just an alias for QGCanvas
typedef GCanvas GBufferedImage;

// default color used to highlight pixels that do not match between two images
#define GBUFFEREDIMAGE_DEFAULT_DIFF_PIXEL_COLOR 0xdd00dd

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gbufferedimage_h
