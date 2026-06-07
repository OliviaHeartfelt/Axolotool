#pragma once

#include "APinData.h"
#include "APinDrag.h"
#include "../wire/AWireItem.h"
#include <functional>
#include <cstdint>

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QGraphicsSceneDragDropEvent>
#include <QSvgRenderer>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QList>
#include <QVariant>

class PinItem : public QGraphicsSvgItem {
private:
    QList<AWireItem*> connectedWires;
    QList<QString> flowAllowList;
    QList<QString> typeAllowList;
    APinData::PinData pinData;

    QPointF dragStartPosition;
    std::move_only_function<bool(QByteArray&)> isBodyAllowedFn = [](QByteArray& body) {
        return true;
    };

    const bool hasDraggingStarted(QGraphicsSceneMouseEvent* event) {
        return (event->screenPos() - dragStartPosition).manhattanLength() >= QApplication::startDragDistance();
    }

    const bool createWire(QGraphicsSceneDragDropEvent* event) {
        quintptr sourceAddress = event->mimeData()->property(APinDrag::Drag::mimeProperty::sourcePinItemPtr()).value<quintptr>();
        QGraphicsItem* sourceItem = reinterpret_cast<QGraphicsItem*>(sourceAddress);
        PinItem* sourcePin = dynamic_cast<PinItem*>(sourceItem);

        if (!sourcePin || sourcePin == this) return false;

        APinData::PinData originPinData = APinDrag::Drag::finishDrag(event);
        if (!isConnectable(originPinData)) return false;

        

        AWireItem* permanentWire = new AWireItem(
            sourcePin,
            this,
            //APinStandardFlow::flowToDgr(sourcePin->pinData.flow), 
            //APinStandardFlow::flowToDgr(this->pinData.flow)
            0,0
        );
        scene()->addItem(permanentWire);

        sourcePin->registerWire(permanentWire);
        this->registerWire(permanentWire);
        return true;
    }

public:
    PinItem(QGraphicsItem* parent) : QGraphicsSvgItem(parent) {
        setSharedRenderer(new QSvgRenderer(QStringLiteral(":/icons/outline/point.svg"), this));
        setAcceptDrops(true);
        setAcceptedMouseButtons(Qt::LeftButton);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    }
    void registerWire(AWireItem* wire) { connectedWires.append(wire); }
    void unregisterWire(AWireItem* wire) { connectedWires.removeOne(wire); }

    //---------------------------------------------------------------------------------------
    void setPinData(APinData::PinData& newPinData) { pinData = newPinData; }
    //void setFlow(APinStandardFlow::Flow newFlow) { pinData.flow = newFlow; }
    //void setType(APinStandardType::Type newType) { pinData.type = newType; }
    void setBody(QByteArray newBody) { pinData.body = newBody; }

    void getPinData(APinData::PinData& pinData) { pinData = this->pinData; }
    //APinStandardFlow::Flow getFlow() { return pinData.flow; }
    //APinStandardType::Type getType() { return pinData.type; }
    QByteArray getBody() { return pinData.body; }

    bool isConnectable(APinData::PinData& data) {
        //if (data.flow == APinStandardFlow::Flow::Undefined || !flowAllowList.contains(data.flow)) return false;
        //if (data.type == APinStandardType::Type::Undefined || !typeAllowList.contains(data.type)) return false;
        //if (isBodyAllowedFn || !isBodyAllowedFn(data.body)) return false;
        return true;
    }
    bool setIsBodyAllowed(std::move_only_function<bool(QByteArray&)>& newCheck) {
        if (!newCheck) return false;
        isBodyAllowedFn = std::move(newCheck);
        return true;
    }
    bool IsBodyAllowed(QByteArray& body) {
        if (!isBodyAllowedFn) return false;
        return isBodyAllowedFn(body);
    }
    //---------------------------------------------------------------------------------------

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override {
        if (change == ItemScenePositionHasChanged) { for (AWireItem* wire : connectedWires) { wire->updatePath(); } }
        return QGraphicsItem::itemChange(change, value);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        event->accept();
        dragStartPosition = event->screenPos();
    }
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        event->accept();

        if (hasDraggingStarted(event))
            APinDrag::Drag::useDrag(event, this, pinData, mapToScene(boundingRect().center()));
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override { event->accept(); }

    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override { event->acceptProposedAction(); }
    void dragMoveEvent( QGraphicsSceneDragDropEvent* event) override { event->acceptProposedAction(); }

    void dropEvent(QGraphicsSceneDragDropEvent* event) override {
        if (!event->mimeData()->hasFormat(APinDrag::Drag::mimeType())) return;
        event->acceptProposedAction();

        if (!createWire(event)) return;
    }
};