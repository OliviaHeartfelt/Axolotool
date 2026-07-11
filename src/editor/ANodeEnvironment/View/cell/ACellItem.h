#pragma once

#include "../pin/APin.h"
#include "../registry/FRegistryKey.h"

namespace ACellItem {

    class Item : public QGraphicsRectItem {
    private:
        QGraphicsSimpleTextItem* textItem;
        APin::Item* pinItem;

        //std::shared_ptr<APin::PinData> cellPinData;
        //std::shared_ptr<APin::AllowLists> allowLists;

    public:
        Item(const QString& name, QGraphicsItem* parent) : QGraphicsRectItem(parent) {
            // Transparent container bounds for the cell
            setRect(0, 0, 90, 20);
            setPen(Qt::NoPen);

            //cellPinData = std::make_shared<APin::PinData>();
            //cellPinData->flow() = { "Standard", "In" };
            //cellPinData->style() = { "Standard", "Int" };

            //allowLists = std::make_shared<APin::AllowLists>();
            //allowLists->flow().add({ "Standard", "In" });

            // Add text relative to this cell's (0,0)
            textItem = new QGraphicsSimpleTextItem(name, this);
            textItem->setPos(5, 2);
            textItem->setBrush(Qt::white);

            // Add pin relative to this cell's (0,0)
            pinItem = new APin::Item(this);
            pinItem->pinData()->flow(FRegistryKey::FRegistryKey{ "Standard", "In" });
            pinItem->pinData()->style(FRegistryKey::FRegistryKey{ "Standard", "Int" });
            pinItem->allowLists()->flow.add(FRegistryKey::FRegistryKey{ "Standard", "In" });
            pinItem->safeUpdate();

            pinItem->setPos(70, 2); // Placed on the right side of the cell
        }
    };
}