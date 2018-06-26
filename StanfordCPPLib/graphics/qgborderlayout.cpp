#include "qgborderlayout.h"
#include "strlib.h"

QGBorderLayout::QGBorderLayout(QWidget *parent, int margin, int spacing)
    : QLayout(parent)
{
    setMargin(margin);
    setSpacing(spacing);
}

QGBorderLayout::QGBorderLayout(int spacing)
{
    setSpacing(spacing);
}

QGBorderLayout::~QGBorderLayout()
{
    QLayoutItem *l;
    while ((l = takeAt(0)))
        delete l;
}

void QGBorderLayout::addItem(QLayoutItem *item)
{
    add(item, West);
}

void QGBorderLayout::addWidget(QWidget *widget)
{
    addWidget(widget, QGBorderLayout::Center);
}

void QGBorderLayout::addWidget(QWidget *widget, Position position)
{
    add(new QWidgetItem(widget), position);
}

Qt::Orientations QGBorderLayout::expandingDirections() const
{
    return Qt::Horizontal | Qt::Vertical;
}

bool QGBorderLayout::hasHeightForWidth() const
{
    return false;
}

int QGBorderLayout::count() const
{
    return list.size();
}

QLayoutItem *QGBorderLayout::itemAt(int index) const
{
    ItemWrapper *wrapper = list.value(index);
    if (wrapper)
        return wrapper->item;
    else
        return nullptr;
}

QSize QGBorderLayout::minimumSize() const
{
    return calculateSize(MinimumSize);
}

void QGBorderLayout::setGeometry(const QRect &rect)
{
    ItemWrapper *center = nullptr;
    int eastWidth = 0;
    int westWidth = 0;
    int northHeight = 0;
    int southHeight = 0;
    int centerHeight = 0;
    int i;

    QLayout::setGeometry(rect);

    for (i = 0; i < list.size(); ++i) {
        ItemWrapper *wrapper = list.at(i);
        QLayoutItem *item = wrapper->item;
        Position position = wrapper->position;

        if (position == North) {
            item->setGeometry(QRect(rect.x(), northHeight, rect.width(),
                                    item->sizeHint().height()));

            northHeight += item->geometry().height() + spacing();
        } else if (position == South) {
            item->setGeometry(QRect(item->geometry().x(),
                                    item->geometry().y(), rect.width(),
                                    item->sizeHint().height()));

            southHeight += item->geometry().height() + spacing();

            item->setGeometry(QRect(rect.x(),
                              rect.y() + rect.height() - southHeight + spacing(),
                              item->geometry().width(),
                              item->geometry().height()));
        } else if (position == Center) {
            center = wrapper;
        }
    }

    centerHeight = rect.height() - northHeight - southHeight;

    for (i = 0; i < list.size(); ++i) {
        ItemWrapper *wrapper = list.at(i);
        QLayoutItem *item = wrapper->item;
        Position position = wrapper->position;

        if (position == West) {
            item->setGeometry(QRect(rect.x() + westWidth, northHeight,
                                    item->sizeHint().width(), centerHeight));

            westWidth += item->geometry().width() + spacing();
        } else if (position == East) {
            item->setGeometry(QRect(item->geometry().x(), item->geometry().y(),
                                    item->sizeHint().width(), centerHeight));

            eastWidth += item->geometry().width() + spacing();

            item->setGeometry(QRect(
                              rect.x() + rect.width() - eastWidth + spacing(),
                              northHeight, item->geometry().width(),
                              item->geometry().height()));
        }
    }

    if (center)
        center->item->setGeometry(QRect(westWidth, northHeight,
                                        rect.width() - eastWidth - westWidth,
                                        centerHeight));
}

QSize QGBorderLayout::sizeHint() const
{
    return calculateSize(SizeHint);
}

QLayoutItem *QGBorderLayout::takeAt(int index)
{
    if (index >= 0 && index < list.size()) {
        ItemWrapper *layoutStruct = list.takeAt(index);
        return layoutStruct->item;
    }
    return nullptr;
}

void QGBorderLayout::add(QLayoutItem *item, Position position)
{
    list.append(new ItemWrapper(item, position));
}

QSize QGBorderLayout::calculateSize(SizeType sizeType) const
{
    QSize totalSize;

    for (int i = 0; i < list.size(); ++i) {
        ItemWrapper *wrapper = list.at(i);
        Position position = wrapper->position;
        QSize itemSize;

        if (sizeType == MinimumSize)
            itemSize = wrapper->item->minimumSize();
        else // (sizeType == SizeHint)
            itemSize = wrapper->item->sizeHint();

        if (position == North || position == South || position == Center)
            totalSize.rheight() += itemSize.height();

        if (position == West || position == East || position == Center)
            totalSize.rwidth() += itemSize.width();
    }
    return totalSize;
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
