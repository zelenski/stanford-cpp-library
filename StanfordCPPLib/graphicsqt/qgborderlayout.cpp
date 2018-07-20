#include "qgborderlayout.h"
#include "strlib.h"

QGBorderLayout::QGBorderLayout(QWidget* parent, int margin, int spacing)
        : QLayout(parent) {
    setMargin(margin);
    setSpacing(spacing);
}

QGBorderLayout::QGBorderLayout(int spacing) {
    setSpacing(spacing);
}

QGBorderLayout::~QGBorderLayout() {
    QLayoutItem* l;
    while ((l = takeAt(0))) {
        delete l;
    }
}

void QGBorderLayout::addItem(QLayoutItem* item) {
    add(item, North);
}

void QGBorderLayout::addWidget(QWidget* widget) {
    addWidget(widget, QGBorderLayout::Center);
}

void QGBorderLayout::addWidget(QWidget* widget, Position position) {
    add(new QWidgetItem(widget), position);
}

Qt::Orientations QGBorderLayout::expandingDirections() const {
    return Qt::Horizontal | Qt::Vertical;
}

bool QGBorderLayout::hasHeightForWidth() const {
    return false;
}

int QGBorderLayout::count() const {
    return list.size();
}

/**
 * Forces the given widget to update, even if it's hidden.
 * from https://stackoverflow.com/questions/2427103/qt-how-to-force-a-hidden-widget-to-calculate-its-layout
 */
void QGBorderLayout::forceUpdate(QWidget* widget) {
    if (!widget) return;

    // Update all child widgets.
    widget->setAttribute(Qt::WA_DontShowOnScreen);   // TODO: remove?
    widget->show();

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
}

/**
 * Helper function for forceUpdate(). Not self-sufficient!
 * from https://stackoverflow.com/questions/2427103/qt-how-to-force-a-hidden-widget-to-calculate-its-layout
 */
void QGBorderLayout::invalidateLayout(QLayout* layout) {
    if (!layout) return;

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


QLayoutItem *QGBorderLayout::itemAt(int index) const {
    ItemWrapper* wrapper = list.value(index);
    if (wrapper) {
        return wrapper->item;
    } else {
        return nullptr;
    }
}

QSize QGBorderLayout::minimumSize() const {
    return calculateSize(MinimumSize);
}

void QGBorderLayout::setGeometry(const QRect& rect) {
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
        Position position = wrapper->position;

        if (position == North) {
            QRect geom(rect.x(),
                       northHeight,
                       rect.width(),
                       item->sizeHint().height());
            item->setGeometry(geom);
            northHeight += item->geometry().height() + spacing();
        } else if (position == South) {
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
        } else if (position == Center) {
            center = wrapper;
        }
    }

    centerHeight = rect.height() - northHeight - southHeight;

    // size/place west/east widgets second
    for (int i = 0; i < list.size(); ++i) {
        ItemWrapper* wrapper = list.at(i);
        QLayoutItem* item = wrapper->item;
        Position position = wrapper->position;

        if (position == West) {
            QRect geom(rect.x() + westWidth,
                       northHeight,
                       item->sizeHint().width(),
                       centerHeight);
            item->setGeometry(geom);
            westWidth += item->geometry().width() + spacing();
        } else if (position == East) {
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

QSize QGBorderLayout::sizeHint() const {
    return calculateSize(SizeHint);
}

QLayoutItem* QGBorderLayout::takeAt(int index) {
    if (index >= 0 && index < list.size()) {
        ItemWrapper* layoutStruct = list.takeAt(index);
        return layoutStruct->item;
    }
    return nullptr;
}

void QGBorderLayout::add(QLayoutItem* item, Position position) {
    list.append(new ItemWrapper(item, position));
}

QSize QGBorderLayout::calculateSize(SizeType sizeType) const {
    QSize totalSize;
    for (int i = 0; i < list.size(); ++i) {
        ItemWrapper* wrapper = list.at(i);
        Position position = wrapper->position;
        QSize itemSize;

        if (sizeType == MinimumSize) {
            itemSize = wrapper->item->minimumSize();
        } else { // (sizeType == SizeHint)
            itemSize = wrapper->item->sizeHint();
        }

        if (position == North || position == South || position == Center) {
            totalSize.rheight() += itemSize.height();
        }

        if (position == West || position == East || position == Center) {
            totalSize.rwidth() += itemSize.width();
        }
    }
    return totalSize;
}

void QGBorderLayout::clearLayout(QLayout* layout) {
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

QGBorderLayout::Position QGBorderLayout::toPosition(const std::string& positionName) {
    std::string regionU = toUpperCase(positionName);
    if (regionU == "NORTH" || regionU == "TOP") {
        return QGBorderLayout::North;
    } else if (regionU == "SOUTH" || regionU == "BOTTOM") {
        return QGBorderLayout::South;
    } else if (regionU == "EAST" || regionU == "RIGHT") {
        return QGBorderLayout::East;
    } else if (regionU == "WEST" || regionU == "LEFT") {
        return QGBorderLayout::West;
    } else {
        // "CENTER" or "MIDDLE" or "NONE"
        return QGBorderLayout::Center;
    }
}
