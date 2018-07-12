/*
 * File: qgdrawingsurface.h
 * ------------------------
 *
 * @version 2018/07/11
 * - initial version
 */

#ifndef _qgdrawingsurface_h
#define _qgdrawingsurface_h

#include <string>
#include <QWidget>
#include "grid.h"
#include "qgobjects.h"

/*
 * ...
 */
class QGDrawingSurface {
public:
    QGDrawingSurface();
    virtual ~QGDrawingSurface();

    // QGCompound methods
    virtual void clear() = 0;
    virtual void conditionalRepaint();
    virtual void conditionalRepaintRegion(int x, int y, int width, int height);
    virtual void conditionalRepaintRegion(const GRectangle& bounds);
    virtual void draw(QGObject* gobj) = 0;
    virtual void draw(QGObject* gobj, double x, double y);
    virtual void draw(QGObject& gobj);
    virtual void draw(QGObject& gobj, double x, double y);
    virtual void draw(QPainter* painter) = 0;
    virtual void drawArc(double x, double y, double width, double height, double start, double sweep);
    virtual void drawImage(const std::string& filename, double x = 0, double y = 0);
    virtual void drawLine(const GPoint& p0, const GPoint& p1);
    virtual void drawLine(double x0, double y0, double x1, double y1);
    virtual void drawOval(const GRectangle& bounds);
    virtual void drawOval(double x, double y, double width, double height);
    virtual GPoint drawPolarLine(const GPoint& p0, double r, double theta);
    virtual GPoint drawPolarLine(double x0, double y0, double r, double theta);
    virtual void drawPixel(double x, double y);
    virtual void drawPixel(double x, double y, int color);
    virtual void drawPixel(double x, double y, const std::string& color);
    virtual void drawPolygon(std::initializer_list<double> coords);
    virtual void drawPolygon(std::initializer_list<GPoint> points);
    virtual void drawRect(const GRectangle& bounds);
    virtual void drawRect(double x, double y, double width, double height);
    virtual void drawString(const std::string& text, double x, double y);
    virtual void fillArc(double x, double y, double width, double height, double start, double sweep);
    virtual void fillOval(const GRectangle& bounds);
    virtual void fillOval(double x, double y, double width, double height);
    virtual void fillPolygon(std::initializer_list<double> coords);
    virtual void fillRect(const GRectangle& bounds);
    virtual void fillRect(double x, double y, double width, double height);
    virtual std::string getBackground() const;
    virtual int getBackgroundInt() const;
    virtual std::string getColor() const;
    virtual int getColorInt() const;
    virtual std::string getFillColor() const;
    virtual int getFillColorInt() const;
    virtual std::string getFont() const;
    virtual std::string getForeground() const;
    virtual int getForegroundInt() const;
    virtual double getLineWidth() const;
    virtual int getPixel(double x, double y) const = 0;
    virtual int getPixelARGB(double x, double y) const = 0;
    virtual Grid<int> getPixels() const = 0;
    virtual Grid<int> getPixelsARGB() const = 0;
    virtual bool isAutoRepaint() const;
    virtual bool isRepaintImmediately() const;
    virtual void repaint() = 0;
    virtual void repaintRegion(int x, int y, int width, int height) = 0;
    virtual void repaintRegion(const GRectangle& bounds);
    virtual void setAutoRepaint(bool autoRepaint);
    virtual void setBackground(int color);
    virtual void setBackground(const std::string& color);
    virtual void setColor(int color);
    virtual void setColor(const std::string& color);
    virtual void setFillColor(int color);
    virtual void setFillColor(const std::string& color);
    virtual void setFont(const std::string& font);
    virtual void setForeground(int color);
    virtual void setForeground(const std::string& color);
    virtual void setLineWidth(double lineWidth);
    virtual void setPixel(double x, double y, int r, int g, int b);
    virtual void setPixel(double x, double y, int rgb) = 0;
    virtual void setPixel(double x, double y, const std::string& color);
    virtual void setPixelARGB(double x, double y, int a, int r, int g, int b);
    virtual void setPixelARGB(double x, double y, int argb) = 0;
    virtual void setPixels(const Grid<int>& pixels) = 0;
    virtual void setPixelsARGB(const Grid<int>& pixelsARGB) = 0;
    virtual void setRepaintImmediately(bool autoRepaint);
    virtual void setRGB(double x, double y, int rgb);
    virtual void setRGB(double x, double y, int r, int g, int b);
    virtual void setRGB(double x, double y, const std::string& color);

protected:
    QGDrawingSurface* _forwardTarget;
    std::string _backgroundColor;
    std::string _color;
    std::string _fillColor;
    std::string _font;
    int _backgroundColorInt;
    int _colorInt;
    int _fillColorInt;
    double _lineWidth;
    bool _autoRepaint;

    virtual void initializeQGObject(QGObject* obj, bool filled = false);
    virtual void setDrawingForwardTarget(QGDrawingSurface* forwardTarget);
};

class QGForwardDrawingSurface : public virtual QGDrawingSurface {
public:
    virtual void clear() Q_DECL_OVERRIDE;
    virtual void draw(QGObject* gobj) Q_DECL_OVERRIDE;
    virtual void draw(QPainter* painter) Q_DECL_OVERRIDE;
    virtual int getPixel(double x, double y) const Q_DECL_OVERRIDE;
    virtual int getPixelARGB(double x, double y) const Q_DECL_OVERRIDE;
    virtual Grid<int> getPixels() const Q_DECL_OVERRIDE;
    virtual Grid<int> getPixelsARGB() const Q_DECL_OVERRIDE;
    virtual bool isAutoRepaint() const Q_DECL_OVERRIDE;
    virtual void repaint() Q_DECL_OVERRIDE;
    virtual void repaintRegion(int x, int y, int width, int height) Q_DECL_OVERRIDE;
    virtual void setAutoRepaint(bool autoRepaint) Q_DECL_OVERRIDE;
    virtual void setBackground(int color) Q_DECL_OVERRIDE;
    virtual void setBackground(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setColor(int color) Q_DECL_OVERRIDE;
    virtual void setColor(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setFillColor(int color) Q_DECL_OVERRIDE;
    virtual void setFillColor(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setFont(const std::string& font) Q_DECL_OVERRIDE;
    virtual void setLineWidth(double lineWidth) Q_DECL_OVERRIDE;
    virtual void setPixel(double x, double y, int rgb) Q_DECL_OVERRIDE;
    virtual void setPixel(double x, double y, int r, int g, int b) Q_DECL_OVERRIDE;
    virtual void setPixelARGB(double x, double y, int argb) Q_DECL_OVERRIDE;
    virtual void setPixelARGB(double x, double y, int a, int r, int g, int b) Q_DECL_OVERRIDE;
    virtual void setPixels(const Grid<int>& pixels) Q_DECL_OVERRIDE;
    virtual void setPixelsARGB(const Grid<int>& pixelsARGB) Q_DECL_OVERRIDE;
    virtual void setRepaintImmediately(bool repaintImmediately) Q_DECL_OVERRIDE;

protected:
    virtual void ensureForwardTarget() = 0;
    virtual void ensureForwardTargetConstHack() const;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgcanvas_h
