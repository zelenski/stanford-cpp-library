/*
 * File: gbufferedimage.cpp
 * ------------------------
 * This file implements the gbufferedimage.h interface.
 * See that file for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2015/08/12
 * - added toGrid, fromGrid, createRgbPixel, getRed, getGreen, getBlue functions
 * - perf.optimizations to per-pixel stuff; now almost tolerable speed
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
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
#include "strlib.h"

#define CHAR_TO_HEX(ch) ((ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'a' + 10))

int GBufferedImage::createRgbPixel(int red, int green, int blue) {
    if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) {
        error("RGB values must be between 0-255");
    }
    return ((red << 16 & 0xff0000) | (green << 8 & 0x00ff00) | (blue & 0x0000ff)) & 0xffffff;
}

int GBufferedImage::getRed(int rgb) {
    return (rgb & 0xff0000) >> 16;
}

int GBufferedImage::getGreen(int rgb) {
    return (rgb & 0x00ff00) >> 8;
}

int GBufferedImage::getBlue(int rgb) {
    return rgb & 0x0000ff;
}

void GBufferedImage::getRedGreenBlue(int rgb, int& red, int& green, int& blue) {
    red = getRed(rgb);
    green = getGreen(rgb);
    blue = getBlue(rgb);
}

GBufferedImage::GBufferedImage()
        : GInteractor(),
          m_width(1),
          m_height(1),
          m_backgroundColor(0) {
    init(/* x */ 0, /* y */ 0, /* width */ 1, /* height */ 1, 0x000000);
}

GBufferedImage::GBufferedImage(double width, double height,
                               int rgbBackground)
    : GInteractor(),
      m_width(1),
      m_height(1),
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
    getPlatform()->gbufferedimage_fill(this, rgb);
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
    getPlatform()->gbufferedimage_fillRegion(this, x, y, width, height, rgb);
}

void GBufferedImage::fillRegion(double x, double y, double width, double height, std::string rgb) {
    fillRegion(x, y, width, height, convertColorToRGB(rgb));
}

void GBufferedImage::fromGrid(const Grid<int>& grid) {
    m_pixels = grid;
    m_width = grid.width();
    m_height = grid.height();
    
    // custom-encoded version (best so far, if it would work)
//    std::ostringstream ostr;
//    ostr << m_width << '#' << m_height << '#';
//    ostr << std::hex << std::setfill('0');
//    for (int row = 0; row < grid.height(); row++) {
//        for (int col = 0; col < grid.width(); col++) {
//            int rgb = grid[row][col];
//            ostr << '#'
//                 << std::setw(2) << (rgb >> 16 & 0xff)
//                 << std::setw(2) << (rgb >> 8 & 0xff)
//                 << std::setw(2) << (rgb & 0xff);
//        }
//    }
//    std::string text = ostr.str();
    
    // base64-encoded version (best)
    int bufLength = m_width * m_height * 3 + 32;
    char buf[bufLength];
    char* bufPtr = buf;
    bufPtr[0] = ((int) m_width & 0xff00) >> 8;
    bufPtr[1] = ((int) m_width & 0x00ff);
    bufPtr[2] = ((int) m_height & 0xff00) >> 8;
    bufPtr[3] = ((int) m_height & 0x00ff);
    bufPtr += 4;
    for (int row = 0; row < grid.height(); row++) {
        for (int col = 0; col < grid.width(); col++) {
            int rgb = grid[row][col];
            bufPtr[0] = ((rgb & 0xff0000) >> 16) & 0x000000ff;
            bufPtr[1] = ((rgb & 0x00ff00) >> 8) & 0x000000ff;
            bufPtr[2] = (rgb & 0x0000ff);
            bufPtr += 3;
        }
    }
    bufPtr[0] = '\0';

    char base64buf[Base64encode_len(bufLength) + 32];
    Base64encode(base64buf, buf, bufLength);
    
    // update the back-end with all of the pretty new pixels
    getPlatform()->gbufferedimage_updateAllPixels(this, base64buf);
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
    
    std::string result = getPlatform()->gbufferedimage_load(this, filename);
    printf("GBuf::load: string from pipe is %ld bytes.\n", result.length()); fflush(stdout);

    // newest 'base64-compressed' version
    const char* resultCstr = result.c_str();
    char buf[Base64decode_len(resultCstr) + 32];
    Base64decode(buf, resultCstr);
    char* bufPtr = buf;
    
    int w = (((bufPtr[0] & 0x000000ff) << 8) & 0x0000ff00) | (bufPtr[1] & 0x000000ff);
    int h = (((bufPtr[2] & 0x000000ff) << 8) & 0x0000ff00) | (bufPtr[3] & 0x000000ff);
    bufPtr += 4;
    printf("new w=%d, h=%d\n", w, h); fflush(stdout);
    if (w != m_pixels.width() || h != m_pixels.height()) {
        m_width = w;
        m_height = h;
        m_pixels.resize(m_height, m_width, /* retain */ false);
    }
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            m_pixels[y][x] = 
                    ((bufPtr[0] << 16 & 0x00ff0000)
                    | (bufPtr[1] << 8 & 0x0000ff00)
                    | (bufPtr[2] & 0x000000ff))
                    & 0x00ffffff;
            bufPtr += 3;
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
        getPlatform()->gbufferedimage_resize(this, width, height, retain);
        if (!retain && m_backgroundColor != 0x0) {
            this->m_pixels.fill(m_backgroundColor);
        }
    }
}

void GBufferedImage::save(const std::string& filename) const {
    getPlatform()->gbufferedimage_save(this, filename);
}

void GBufferedImage::setRGB(double x, double y, int rgb) {
    checkIndex("setRGB", x, y);
    checkColor("setRGB", rgb);
    m_pixels[(int) y][(int) x] = rgb;
    getPlatform()->gbufferedimage_setRGB(this, x, y, rgb);
}

void GBufferedImage::setRGB(double x, double y, std::string rgb) {
    setRGB(x, y, convertColorToRGB(rgb));
}

Grid<int> GBufferedImage::toGrid() const {
    return m_pixels;
}

void GBufferedImage::toGrid(Grid<int>& grid) const {
    grid = m_pixels;
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
        getPlatform()->gbufferedimage_constructor(this, x, y, width, height, rgb);
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
