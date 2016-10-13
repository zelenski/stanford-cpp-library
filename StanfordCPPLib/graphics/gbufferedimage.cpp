/*
 * File: gbufferedimage.cpp
 * ------------------------
 * This file implements the gbufferedimage.h interface.
 * See that file for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2016/07/30
 * - added constructor that takes a file name
 * - converted all occurrences of string parameters to const string&
 * - added operators ==, !=
 * @version 2015/10/08
 * - bug fixes and refactoring for pixel-based functions such as fromGrid, load
 *   to help fix bugs with Base64 encoding/decoding
 * @version 2015/10/05
 * - removed extraneous debug printf statements left in during development
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
#include <cstring>
#include <iomanip>
#include "base64.h"
#include "filelib.h"
#include "gwindow.h"
#include "strlib.h"
#include "private/platform.h"

#define CHAR_TO_HEX(ch) ((ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'a' + 10))

const int GBufferedImage::WIDTH_HEIGHT_MAX = 65535;

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

GBufferedImage::GBufferedImage(const std::string& filename)
    : GInteractor(),
      m_width(1),
      m_height(1),
      m_backgroundColor(0) {
    init(/* x */ 0, /* y */ 0, /* width */ 1, /* height */ 1, 0x000000);
    load(filename);
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
                               const std::string& rgbBackground)
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
    
    Grid<int> resultGrid;
    resultGrid.resize(hmax, wmax);
    resultGrid.fill(diffPixelColor);
    for (int r = 0; r < h1; r++) {
        for (int c = 0; c < w1; c++) {
            resultGrid[r][c] = m_backgroundColor;
        }
    }
    for (int y = 0; y < hmin; y++) {
        for (int x = 0; x < wmin; x++) {
            int px1 = m_pixels[y][x];
            int px2 = image.m_pixels[y][x];
            if (px1 != px2) {
                resultGrid[y][x] = diffPixelColor;
            }
        }
    }
    GBufferedImage* result = new GBufferedImage(wmax, hmax);
    result->fromGrid(resultGrid);
    return result;
}

void GBufferedImage::fill(int rgb) {
    checkColor("fill", rgb);
    m_pixels.fill(rgb);
    stanfordcpplib::getPlatform()->gbufferedimage_fill(this, rgb);
}

void GBufferedImage::fill(const std::string& rgb) {
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
    stanfordcpplib::getPlatform()->gbufferedimage_fillRegion(this, x, y, width, height, rgb);
}

void GBufferedImage::fillRegion(double x, double y, double width, double height, const std::string& rgb) {
    fillRegion(x, y, width, height, convertColorToRGB(rgb));
}

void GBufferedImage::fromGrid(const Grid<int>& grid) {
    checkSize("fromGrid", grid.width(), grid.height());
    m_pixels = grid;
    m_width = grid.width();
    m_height = grid.height();
    
    // output a base64-encoded version of the image pixels
    std::ostringstream out;
    
    // output width as 2 bytes, then height as 2 bytes
    out << (char) (((((int) m_width) & 0x0000ff00) >> 8) & 0x000000ff);
    out << (char)  ((((int) m_width) & 0x000000ff));
    out << (char) (((((int) m_height) & 0x0000ff00) >> 8) & 0x000000ff);
    out << (char)  ((((int) m_height) & 0x000000ff));
    
    // output each pixel as 3 bytes (R,G,B)
    for (int row = 0; row < m_height; row++) {
        for (int col = 0; col < m_width; col++) {
            int rgb = grid[row][col];
            out << (char) (((rgb & 0x00ff0000) >> 16) & 0x000000ff);
            out << (char) (((rgb & 0x0000ff00) >> 8) & 0x000000ff);
            out << (char)   (rgb & 0x000000ff);
        }
    }

    // encode the bytes into a base64 string so it can go through
    // the process pipe to the Java back-end
    std::string result = out.str();
    std::string encoded = Base64::encode(result);
    
    // update the back-end with all of the pretty new pixels
    stanfordcpplib::getPlatform()->gbufferedimage_updateAllPixels(this, encoded);
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
    
    // read Base64-compressed pixel data from Java back-end
    std::string result = stanfordcpplib::getPlatform()->gbufferedimage_load(this, filename);
    std::string decoded = Base64::decode(result);
    
    // read width (2-byte) and height (2-byte)
    int w = (((decoded[0] & 0x000000ff) << 8) & 0x0000ff00) | (decoded[1] & 0x000000ff);
    int h = (((decoded[2] & 0x000000ff) << 8) & 0x0000ff00) | (decoded[3] & 0x000000ff);
    if (w != m_pixels.width() || h != m_pixels.height()) {
        m_width = w;
        m_height = h;
        m_pixels.resize(m_height, m_width, /* retain */ false);
    }
    int expectedLength = (w * h * 3) + 4;
    int actualLength = (int) decoded.length();
    
    // crash if number of bytes that arrive are way off from what's expected
    if (actualLength < expectedLength || actualLength > expectedLength + 10) {
        std::string errorMessage = std::string("GBufferedImage::load(\"" + filename + "\"): expected ")
                + integerToString(expectedLength) + std::string(" bytes but saw ")
                + integerToString(actualLength);
        error(errorMessage);
    }
    
    // read each pixel (3-byte: R,G,B)
    int i = 4;
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            if (i + 2 < actualLength) {
                m_pixels[y][x] = 
                         ((((decoded[i]   & 0x000000ff) << 16) & 0x00ff0000)   // red
                        | (((decoded[i+1] & 0x000000ff) <<  8) & 0x0000ff00)   // green
                        | (decoded[i+2]   & 0x000000ff))                       // blue
                        & 0x00ffffff;                                          // alpha mask
                i += 3;
            }
        }
    }
}

void GBufferedImage::resize(double width, double height, bool retain) {
    checkSize("resize", width, height);
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
        stanfordcpplib::getPlatform()->gbufferedimage_resize(this, width, height, retain);
        if (!retain && m_backgroundColor != 0x0) {
            this->m_pixels.fill(m_backgroundColor);
        }
    }
}

void GBufferedImage::save(const std::string& filename) const {
    stanfordcpplib::getPlatform()->gbufferedimage_save(this, filename);
}

void GBufferedImage::setRGB(double x, double y, int rgb) {
    checkIndex("setRGB", x, y);
    checkColor("setRGB", rgb);
    m_pixels[(int) y][(int) x] = rgb;
    stanfordcpplib::getPlatform()->gbufferedimage_setRGB(this, x, y, rgb);
}

void GBufferedImage::setRGB(double x, double y, const std::string& rgb) {
    setRGB(x, y, convertColorToRGB(rgb));
}

Grid<int> GBufferedImage::toGrid() const {
    return m_pixels;
}

void GBufferedImage::toGrid(Grid<int>& grid) const {
    grid = m_pixels;
}


void GBufferedImage::checkColor(const std::string& member, int rgb) const {
    if (rgb < 0x0 || rgb > 0xffffff) {
        error("GBufferedImage::" + member
              + ": color is outside of range 0x000000 through 0xffffff");
    }
}

void GBufferedImage::checkIndex(const std::string& member, double x, double y) const {
    if (!inBounds(x, y)) {
        error("GBufferedImage::" + member
              + ": (x=" + integerToString((int) x)
              + ", y=" + integerToString((int) y)
              + ") is out of valid range of (0, 0) through ("
              + integerToString((int) m_width - 1) + ", "
              + integerToString((int) m_height - 1) + ")");
    }
}

void GBufferedImage::checkSize(const std::string& member, double width, double height) const {
    if (width < 0 || height < 0) {
        error("GBufferedImage::" + member + ": width/height cannot be negative");
    }
    if (width > GBufferedImage::WIDTH_HEIGHT_MAX
            || height > GBufferedImage::WIDTH_HEIGHT_MAX) {
        error("GBufferedImage::" + member + ": width/height too large (cannot exceed "
              + integerToString(WIDTH_HEIGHT_MAX));
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
        stanfordcpplib::getPlatform()->gbufferedimage_constructor(this, x, y, width, height, rgb);
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

bool operator ==(const GBufferedImage& img1, const GBufferedImage& img2) {
    return img1.m_width == img2.m_width
            && img1.m_height == img2.m_height
            && img1.m_backgroundColor == img2.m_backgroundColor
            && img1.m_pixels == img2.m_pixels;
}

bool operator !=(const GBufferedImage& img1, const GBufferedImage& img2) {
    return !(img1 == img2);
}
