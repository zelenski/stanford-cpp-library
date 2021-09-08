/*
 * File: glayout.cpp
 * -----------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to glayout.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#include "glayout.h"
#include "require.h"
#include "strlib.h"

GLayout::GLayout() {
    // empty
}

void GLayout::clearLayout(QLayout* layout) {
    if (!layout) {
        return;
    }

    // http://doc.qt.io/qt-5/qlayout.html#takeAt
    QLayoutItem* child;
    while ((child = layout->takeAt(0))) {
        // TODO: delete child;
    }
    layout->update();
}

bool GLayout::contains(QLayout* layout, QWidget* widget) {
    if (!layout || !widget) {
        return false;
    }

    for (int i = 0; i < layout->count(); i++) {
        QLayoutItem* child = layout->itemAt(i);
        QWidget* childWidget = child->widget();
        if (childWidget == widget) {
            return true;
        }
    }

    return false;
}

void GLayout::forceUpdate(GInteractor* interactor) {
    if (interactor) {
        forceUpdate(interactor->getWidget());
    }
}

/**
 * Forces the given widget to update, even if it's hidden.
 * from https://stackoverflow.com/questions/2427103/qt-how-to-force-a-hidden-widget-to-calculate-its-layout
 */
void GLayout::forceUpdate(QWidget* widget) {
    if (!widget) {
        return;
    }

    // Update all child widgets.
    for (int i = 0; i < widget->children().size(); i++) {
        QObject* child = widget->children()[i];
        if (child->isWidgetType()) {
            forceUpdate((QWidget*) child);
        }
    }

    widget->updateGeometry();
    widget->update();

    // Invalidate the layout of the widget.
    if (widget->layout()) {
        invalidateLayout(widget->layout());
    }

    widget->setAttribute(Qt::WA_DontShowOnScreen, true);   // TODO: remove?
    widget->setAttribute(Qt::WA_DontShowOnScreen, false);
    widget->show();
}

QSize GLayout::getPreferredSize(QWidget* widget) {
    require::nonNull(widget, "GLayout::getPreferredSize", "widget");
    QRect geom = widget->geometry();
    QSize hint = widget->sizeHint();
    int height = hint.height() > 0 ? hint.height() : geom.height();
    int width = hint.width() > 0 ? hint.width() : geom.width();
    return QSize(width, height);
}

QSize GLayout::getProperSize(QLayout* layout) {
    require::nonNull(layout, "GLayout::getProperSize", "layout");
    QRect geom = layout->geometry();
    QSize hint = layout->sizeHint();
    int height = geom.height() > 0 ? geom.height() : hint.height();
    int width = geom.width() > 0 ? geom.width() : hint.width();
    return QSize(width, height);
}

QSize GLayout::getProperSize(QWidget* widget) {
    require::nonNull(widget, "GLayout::getProperSize", "widget");
    QRect geom = widget->geometry();
    QSize hint = widget->sizeHint();
    int height = geom.height() > 0 ? geom.height() : hint.height();
    int width = geom.width() > 0 ? geom.width() : hint.width();
    return QSize(width, height);
}

/**
 * Helper function for forceUpdate(). Not self-sufficient!
 * from https://stackoverflow.com/questions/2427103/qt-how-to-force-a-hidden-widget-to-calculate-its-layout
 */
void GLayout::invalidateLayout(QLayout* layout) {
    if (!layout) {
        return;
    }

    // Recompute the given layout and all its child layouts.
    for (int i = 0; i < layout->count(); i++) {
        QLayoutItem* item = layout->itemAt(i);
        if (item->layout()) {
            invalidateLayout(item->layout());
        } else {
            item->invalidate();
        }
    }
    layout->invalidate();
    layout->activate();
}

GLayout::Position GLayout::toPosition(const std::string& positionName) {
    std::string regionU = toUpperCase(positionName);
    if (regionU == "NORTH" || regionU == "TOP") {
        return GLayout::North;
    } else if (regionU == "SOUTH" || regionU == "BOTTOM") {
        return GLayout::South;
    } else if (regionU == "EAST" || regionU == "RIGHT") {
        return GLayout::East;
    } else if (regionU == "WEST" || regionU == "LEFT") {
        return GLayout::West;
    } else {
        // "CENTER" or "MIDDLE" or "NONE"
        return GLayout::Center;
    }
}

GBorderLayout::GBorderLayout(QWidget* parent, int margin, int spacing)
        : QLayout(parent) {
    setMargin(margin);
    setSpacing(spacing);
}

GBorderLayout::GBorderLayout(int spacing) {
    setSpacing(spacing);
}

GBorderLayout::~GBorderLayout() {
    QLayoutItem* l;
    while ((l = takeAt(0))) {
        delete l;
    }
}

void GBorderLayout::addItem(QLayoutItem* item) {
    require::nonNull(item, "GBorderLayout::addItem");
    add(item, GLayout::North);
}

void GBorderLayout::addWidget(QWidget* widget) {
    addWidget(widget, GLayout::Center);
}

void GBorderLayout::addWidget(QWidget* widget, GLayout::Position position) {
    require::nonNull(widget, "GBorderLayout::addWidget", "widget");
    add(new QWidgetItem(widget), position);
}

Qt::Orientations GBorderLayout::expandingDirections() const {
    return Qt::Horizontal | Qt::Vertical;
}

bool GBorderLayout::hasHeightForWidth() const {
    return false;
}

int GBorderLayout::count() const {
    return list.size();
}

QLayoutItem* GBorderLayout::itemAt(int index) const {
    ItemWrapper* wrapper = list.value(index);
    if (wrapper) {
        return wrapper->item;
    } else {
        return nullptr;
    }
}

QSize GBorderLayout::minimumSize() const {
    return calculateSize(MinimumSize);
}

void GBorderLayout::setGeometry(const QRect& rect) {
    ItemWrapper* center = nullptr;
    int eastWidth = 0;
    int westWidth = 0;
    int northHeight = 0;
    int southHeight = 0;
    int centerHeight = 0;

    QLayout::setGeometry(rect);

    // size/place north/south widgets first
    for (int i = 0; i < list.size(); ++i) {
        ItemWrapper* wrapper = list.at(i);
        QLayoutItem* item = wrapper->item;
        GLayout::Position position = wrapper->position;

        if (position == GLayout::North) {
            QRect geom(rect.x(),
                       northHeight,
                       rect.width(),
                       item->sizeHint().height());
            item->setGeometry(geom);
            northHeight += item->geometry().height() + spacing();
        } else if (position == GLayout::South) {
            QRect geom(item->geometry().x(),
                       item->geometry().y(),
                       rect.width(),
                       item->sizeHint().height());
            item->setGeometry(geom);

            southHeight += item->geometry().height() + spacing();
            geom = QRect(rect.x(),
                         rect.y() + rect.height() - southHeight + spacing(),
                         item->geometry().width(),
                         item->geometry().height());
            item->setGeometry(geom);
        } else if (position == GLayout::Center) {
            center = wrapper;
        }
    }

    centerHeight = rect.height() - northHeight - southHeight;

    // size/place west/east widgets second
    for (int i = 0; i < list.size(); ++i) {
        ItemWrapper* wrapper = list.at(i);
        QLayoutItem* item = wrapper->item;
        GLayout::Position position = wrapper->position;

        if (position == GLayout::West) {
            QRect geom(rect.x() + westWidth,
                       northHeight,
                       item->sizeHint().width(),
                       centerHeight);
            item->setGeometry(geom);
            westWidth += item->geometry().width() + spacing();
        } else if (position == GLayout::East) {
            QRect geom(item->geometry().x(),
                       item->geometry().y(),
                       item->sizeHint().width(),
                       centerHeight);
            item->setGeometry(geom);

            eastWidth += item->geometry().width() + spacing();
            geom = QRect(rect.x() + rect.width() - eastWidth + spacing(),
                       northHeight,
                       item->geometry().width(),
                       item->geometry().height());
            item->setGeometry(geom);
        }
    }

    // size/place central widget last
    if (center) {
        QRect geom(westWidth,
                   northHeight,
                   rect.width() - eastWidth - westWidth,
                   centerHeight);
        center->item->setGeometry(geom);
    }
}

QSize GBorderLayout::sizeHint() const {
    return calculateSize(SizeHint);
}

QLayoutItem* GBorderLayout::takeAt(int index) {
    if (index >= 0 && index < list.size()) {
        ItemWrapper* layoutStruct = list.takeAt(index);
        return layoutStruct->item;
    } else {
        return nullptr;
    }
}

void GBorderLayout::add(QLayoutItem* item, GLayout::Position position) {
    require::nonNull(item, "GBorderLayout::add");
    list.append(new ItemWrapper(item, position));
}

QSize GBorderLayout::calculateSize(SizeType sizeType) const {
    QSize totalSize;
    for (int i = 0; i < list.size(); ++i) {
        ItemWrapper* wrapper = list.at(i);
        GLayout::Position position = wrapper->position;
        QSize itemSize;

        if (sizeType == MinimumSize) {
            itemSize = wrapper->item->minimumSize();
        } else { // (sizeType == SizeHint)
            itemSize = wrapper->item->sizeHint();
        }

        if (position == GLayout::North || position == GLayout::South || position == GLayout::Center) {
            totalSize.rheight() += itemSize.height();
        }

        if (position == GLayout::West || position == GLayout::East || position == GLayout::Center) {
            totalSize.rwidth() += itemSize.width();
        }
    }
    return totalSize;
}
