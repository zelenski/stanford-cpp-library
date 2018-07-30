/*
 * File: qborderlayout.h
 * ---------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#ifdef SPL_QT_GUI
#ifndef _qgborderlayout_h
#define _qgborderlayout_h

#include <QLayout>
#include <QRect>
#include <QWidget>

// based on: http://doc.qt.io/qt-5.6/qtwidgets-layouts-borderlayout-example.html

class QGBorderLayout : public QLayout {
public:
    enum Position { West, North, South, East, Center };

    QGBorderLayout(QWidget* parent, int margin = 0, int spacing = -1);
    QGBorderLayout(int spacing = -1);
    ~QGBorderLayout();

    void addItem(QLayoutItem* item) Q_DECL_OVERRIDE;
    void addWidget(QWidget* widget);
    void addWidget(QWidget* widget, Position position);
    Qt::Orientations expandingDirections() const Q_DECL_OVERRIDE;
    bool hasHeightForWidth() const Q_DECL_OVERRIDE;
    int count() const Q_DECL_OVERRIDE;
    QLayoutItem* itemAt(int index) const Q_DECL_OVERRIDE;
    QSize minimumSize() const Q_DECL_OVERRIDE;
    void setGeometry(const QRect& rect) Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QLayoutItem* takeAt(int index) Q_DECL_OVERRIDE;

    void add(QLayoutItem* item, Position position);

    static void clearLayout(QLayout* layout);
    static void forceUpdate(QWidget* widget);
    static void invalidateLayout(QLayout* layout);
    static Position toPosition(const std::string& positionName);

private:
    struct ItemWrapper {
        ItemWrapper(QLayoutItem* i, Position p) {
            item = i;
            position = p;
        }

        QLayoutItem* item;
        Position position;
    };

    enum SizeType { MinimumSize, SizeHint };
    QSize calculateSize(SizeType sizeType) const;

    QList<ItemWrapper*> list;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgborderlayout_h
#endif // SPL_QT_GUI
