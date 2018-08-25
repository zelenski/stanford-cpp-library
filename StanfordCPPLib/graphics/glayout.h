/*
 * File: glayout.h
 * ---------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to glayout.h to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#ifndef _gborderlayout_h
#define _gborderlayout_h

#include <QLayout>
#include <QRect>
#include <QWidget>

// based on: http://doc.qt.io/qt-5.6/qtwidgets-layouts-borderlayout-example.html

class GLayout {
public:
    enum Position { West, North, South, East, Center };

    static void clearLayout(QLayout* layout);
    static void forceUpdate(QWidget* widget);
    static void invalidateLayout(QLayout* layout);
    static Position toPosition(const std::string& positionName);

private:
    GLayout();   // forbid construction
};

class GBorderLayout : public QLayout {
public:
    GBorderLayout(QWidget* parent, int margin = 0, int spacing = -1);
    GBorderLayout(int spacing = -1);
    ~GBorderLayout();

    void addItem(QLayoutItem* item) Q_DECL_OVERRIDE;
    void addWidget(QWidget* widget);
    void addWidget(QWidget* widget, GLayout::Position position);
    Qt::Orientations expandingDirections() const Q_DECL_OVERRIDE;
    bool hasHeightForWidth() const Q_DECL_OVERRIDE;
    int count() const Q_DECL_OVERRIDE;
    QLayoutItem* itemAt(int index) const Q_DECL_OVERRIDE;
    QSize minimumSize() const Q_DECL_OVERRIDE;
    void setGeometry(const QRect& rect) Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QLayoutItem* takeAt(int index) Q_DECL_OVERRIDE;

    void add(QLayoutItem* item, GLayout::Position position);

private:
    struct ItemWrapper {
        ItemWrapper(QLayoutItem* i, GLayout::Position p) {
            item = i;
            position = p;
        }

        QLayoutItem* item;
        GLayout::Position position;
    };

    enum SizeType { MinimumSize, SizeHint };
    QSize calculateSize(SizeType sizeType) const;

    QList<ItemWrapper*> list;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gborderlayout_h
