#pragma once

#include "./pin/APinItem.h"
#include "./pin/APinData.h"
#include "./pin/APinAllowLists.h"
#include <memory>

#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QString>
#include <QGraphicsItem>
#include <QPainter>

class CellItem : public QGraphicsRectItem {
private:
    QGraphicsSimpleTextItem* textItem;
    PinItem* pinItem;

    std::shared_ptr<APinData::PinData> cellPinData;
    std::shared_ptr<APinAllowLists::AllowLists> allowLists;

public:
    CellItem(const QString& name, QGraphicsItem* parent) : QGraphicsRectItem(parent) {
        // Transparent container bounds for the cell
        setRect(0, 0, 90, 20);
        setPen(Qt::NoPen);

        cellPinData = std::make_shared<APinData::PinData>();
        cellPinData->flow() = { "Standard", "In" };
        cellPinData->style() = { "Standard", "Int" };

        allowLists = std::make_shared<APinAllowLists::AllowLists>();
        allowLists->flow().add({ "Standard", "In" });

        // Add text relative to this cell's (0,0)
        textItem = new QGraphicsSimpleTextItem(name, this);
        textItem->setPos(5, 2);
        textItem->setBrush(Qt::white);

        // Add pin relative to this cell's (0,0)
        pinItem = new PinItem(this);
        pinItem->pinData(cellPinData);
        pinItem->allowLists(allowLists);

        pinItem->setPos(70, 2); // Placed on the right side of the cell
    }
};