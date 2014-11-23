/*
 * File: gbufferedimage.cpp
 * ------------------------
 * This file implements the gbufferedimage.h interface.
 * See that file for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2014/11/18
 * - fixes for bugs with resizing initially-size-0 buffered images
 * @version 2014/10/22
 * - added load, save methods
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - added inBounds method, private resize, zero-arg ctor; slight refactor
 * @since 2014/08/04
 */

#include "gbufferedimage.h"
#include <iomanip>
#include "base64.h"
#include "filelib.h"
#include "gwindow.h"
#include "platform.h"

static Platform* pp = getPlatform();

GBufferedImage::GBufferedImage()
        : GInteractor(),
          m_width(0),
          m_height(0),
          m_backgroundColor(0) {
    init(0, 0, 0, 0, 0x000000);
}

GBufferedImage::GBufferedImage(double width, double height)
    : GInteractor(),
      m_width(width),
      m_height(height),
      m_backgroundColor(0) {
    init(0, 0, width, height, 0x000000);
}

GBufferedImage::GBufferedImage(double width, double height,
                               int rgbBackground)
    : GInteractor(),
      m_width(0),
      m_height(0),
      m_backgroundColor(rgbBackground) {
    init(0, 0, width, height, rgbBackground);
}

GBufferedImage::GBufferedImage(double x, double y, double width, double height,
                               int rgbBackground)
    : GInteractor(),
      m_width(width),
      m_height(height),
      m_backgroundColor(rgbBackground) {
    init(x, y, width, height, rgbBackground);
}

GBufferedImage::GBufferedImage(double x, double y, double width, double height,
                               std::string rgbBackground)
    : GInteractor(),
      m_width(width),
      m_height(height),
      m_backgroundColor(0) {
    init(x, y, width, height, convertColorToRGB(rgbBackground));
}

GRectangle GBufferedImage::getBounds() const {
    return GRectangle(x, y, m_width, m_height);
}

std::string GBufferedImage::getType() const {
    return "GBufferedImage";
}

std::string GBufferedImage::toString() const {
    return "GBufferedImage()";
}

void GBufferedImage::clear() {
    fill(m_backgroundColor);
}

int GBufferedImage::countDiffPixels(GBufferedImage& image) const {
    int w1 = (int) getWidth();
    int h1 = (int) getHeight();
    int w2 = (int) image.getWidth();
    int h2 = (int) image.getHeight();

    int wmin = std::min(w1, w2);
    int hmin = std::min(h1, h2);
    
    int overlap = std::min(w1, w2) * std::min(h1, h2);
    int diffPxCount = (w1 * h1 - overlap) + (w2 * h2 - overlap);

    for (int y = 0; y < hmin; y++) {
        for (int x = 0; x < wmin; x++) {
            int px1 = m_pixels[y][x];
            int px2 = image.m_pixels[y][x];
            if (px1 != px2) {
                diffPxCount++;
            }
        }
    }

    return diffPxCount;
}

GBufferedImage* GBufferedImage::diff(GBufferedImage& image, int diffPixelColor) const {
    int w1 = (int) getWidth();
    int h1 = (int) getHeight();
    int w2 = (int) image.getWidth();
    int h2 = (int) image.getHeight();
    int wmin = std::min(w1, w2);
    int hmin = std::min(h1, h2);
    int wmax = std::max(w1, w2);
    int hmax = std::max(h1, h2);
    
    GBufferedImage* result = new GBufferedImage(wmax, hmax, diffPixelColor);
    result->fillRegion(0, 0, w1, h1, m_backgroundColor);
    for (int y = 0; y < hmin; y++) {
        for (int x = 0; x < wmin; x++) {
            int px1 = m_pixels[y][x];
            int px2 = image.m_pixels[y][x];
            if (px1 != px2) {
                result->setRGB(x, y, diffPixelColor);
            }
        }
    }
    return result;
}

void GBufferedImage::fill(int rgb) {
    checkColor("fill", rgb);
    m_pixels.fill(rgb);
    pp->gbufferedimage_fill(this, rgb);
}

void GBufferedImage::fill(std::string rgb) {
    fill(convertColorToRGB(rgb));
}

void GBufferedImage::fillRegion(double x, double y, double width, double height, int rgb) {
    checkIndex("fillRegion", x, y);
    checkIndex("fillRegion", x + width - 1, y + height - 1);
    checkColor("fillRegion", rgb);
    for (int r = (int) y; r < y + height; r++) {
        for (int c = (int) x; c < x + width; c++) {
            m_pixels[r][c] = rgb;
        }
    }
    pp->gbufferedimage_fillRegion(this, x, y, width, height, rgb);
}

void GBufferedImage::fillRegion(double x, double y, double width, double height, std::string rgb) {
    fillRegion(x, y, width, height, convertColorToRGB(rgb));
}

double GBufferedImage::getHeight() const {
    return m_height;
}

int GBufferedImage::getRGB(double x, double y) const {
    checkIndex("getRGB", x, y);
    return m_pixels[(int) y][(int) x];
}

std::string GBufferedImage::getRGBString(double x, double y) const {
    return convertRGBToColor(getRGB(x, y));
}

double GBufferedImage::getWidth() const {
    return m_width;
}

bool GBufferedImage::inBounds(double x, double y) const {
    return m_pixels.inBounds((int) y, (int) x);
}

void GBufferedImage::load(const std::string& filename) {
    // for efficiency, let's at least check whether the file exists
    // and throw error immediately rather than contacting the back-end
    if (!fileExists(filename)) {
        error("GBufferedImage::load: file not found: " + filename);
    }
    
    std::string result = pp->gbufferedimage_load(this, filename);
    result = Base64::decode(result);
    std::istringstream input(result);
    std::string line;
    if (!getline(input, line)) {
        error("GBufferedImage::load: image data does not contain valid width");
    }
    m_width = stringToInteger(line);
    if (!getline(input, line)) {
        error("GBufferedImage::load: image data does not contain valid height");
    }
    m_height = stringToInteger(line);
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            if (!getline(input, line)) {
                error("GBufferedImage::load: image data does not contain valid pixel (x="
                      + integerToString(x) + ", y=" + integerToString(y) + ")");
            }
            int px = convertColorToRGB(line);
            m_pixels[y][x] = px;
        }
    }
}

void GBufferedImage::resize(double width, double height, bool retain) {
    bool wasZero = (this->m_width == 0 && this->m_height == 0);
    this->m_width = width;
    this->m_height = height;
    if (wasZero) {
        if (width > 0 && height > 0) {
            init(this->x, this->y, this->m_width, this->m_height, this->m_backgroundColor);
        } // else still zero
    } else {
        // was non-zero
        this->m_pixels.resize((int) this->m_height, (int) this->m_width, retain);
        pp->gbufferedimage_resize(this, width, height, retain);
        if (!retain && m_backgroundColor != 0x0) {
            this->m_pixels.fill(m_backgroundColor);
        }
    }
}

void GBufferedImage::save(const std::string& filename) const {
    pp->gbufferedimage_save(this, filename);
}

void GBufferedImage::setRGB(double x, double y, int rgb) {
    checkIndex("setRGB", x, y);
    checkColor("setRGB", rgb);
    m_pixels[(int) y][(int) x] = rgb;
    pp->gbufferedimage_setRGB(this, x, y, rgb);
}

void GBufferedImage::setRGB(double x, double y, std::string rgb) {
    setRGB(x, y, convertColorToRGB(rgb));
}

void GBufferedImage::checkColor(std::string member, int rgb) const {
    if (rgb < 0x0 || rgb > 0xffffff) {
        error("GBufferedImage::" + member
              + ": color is outside of range 0x000000 through 0xffffff");
    }
}

void GBufferedImage::checkIndex(std::string member, double x, double y) const {
    if (!inBounds(x, y)) {
        error("GBufferedImage::" + member
              + ": (x=" + integerToString((int) x)
              + ", y=" + integerToString((int) y)
              + ") is out of valid range of (0, 0) through ("
              + integerToString((int) m_width) + ", "
              + integerToString((int) m_height) + ")");
    }
}

void GBufferedImage::checkSize(std::string member, double width, double height) const {
    if (width < 0 || height < 0) {
        error("GBufferedImage::" + member + ": width/height cannot be negative");
    }
}

void GBufferedImage::init(double x, double y, double width, double height,
                          int rgb) {
    checkSize("constructor", width, height);
    checkColor("constructor", rgb);
    this->x = x;
    this->y = y;
    this->m_width = width;
    this->m_height = height;
    if (width > 0 && height > 0) {
        this->m_pixels.resize((int) this->m_height, (int) this->m_width);
        pp->gbufferedimage_constructor(this, x, y, width, height, rgb);
    }

    if (x != 0 || y != 0) {
        setLocation(x, y);
    }
    
    this->m_backgroundColor = rgb;
    if (width > 0 && height > 0) {
        if (rgb != 0) {
            fill(rgb);
        }
    }
}
