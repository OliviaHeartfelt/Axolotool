#pragma once

#include "./pin/APinItem.h"

#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QString>
#include <QGraphicsItem>
#include <QPainter>

class CellItem : public QGraphicsRectItem {
private:
    QGraphicsSimpleTextItem* textItem;
    PinItem* pinItem;

public:
    CellItem(const QString& name, QGraphicsItem* parent) : QGraphicsRectItem(parent) {
        // Transparent container bounds for the cell
        setRect(0, 0, 90, 20);
        setPen(Qt::NoPen);

        // Add text relative to this cell's (0,0)
        textItem = new QGraphicsSimpleTextItem(name, this);
        textItem->setPos(5, 2);
        textItem->setBrush(Qt::white);

        // Add pin relative to this cell's (0,0)
        pinItem = new PinItem(this);
        pinItem->setPos(70, 2); // Placed on the right side of the cell
    }
};