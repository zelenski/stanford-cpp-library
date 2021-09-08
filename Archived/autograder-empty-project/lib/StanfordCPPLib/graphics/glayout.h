/*
 * File: glayout.h
 * ---------------
 *
 * @author Marty Stepp
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to glayout.h to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _glayout_h
#define _glayout_h

#include <QLayout>
#include <QRect>
#include <QWidget>

#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#undef INTERNAL_INCLUDE

/**
 * A helper class with functionality related to container layout management.
 * Clients generally do not need to use this class directly.
 * @private
 */
class GLayout {
public:

    enum Position { West, North, South, East, Center };

    static void clearLayout(QLayout* layout);
    static bool contains(QLayout* layout, QWidget* widget);
    static void forceUpdate(GInteractor* interactor);
    static void forceUpdate(QWidget* widget);
    static QSize getPreferredSize(QWidget* widget);
    static QSize getProperSize(QLayout* layout);
    static QSize getProperSize(QWidget* widget);
    static void invalidateLayout(QLayout* layout);
    static Position toPosition(const std::string& positionName);

private:
    GLayout();   // forbid construction
};

/**
 * A Qt layout manager that performs a "border" layout a la Java AWT's.
 * This class currently does not work properly.
 *
 * based on: http://doc.qt.io/qt-5.6/qtwidgets-layouts-borderlayout-example.html
 *
 * TODO: finish/debug or remove
 *
 * @private
 */
class GBorderLayout : public QLayout {
public:
    GBorderLayout(QWidget* parent, int margin = 0, int spacing = -1);
    GBorderLayout(int spacing = -1);
    virtual ~GBorderLayout() Q_DECL_OVERRIDE;

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
    Q_DISABLE_COPY(GBorderLayout)

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

#endif // _glayout_h
