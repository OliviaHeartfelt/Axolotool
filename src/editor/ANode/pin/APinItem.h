#pragma once

#include "APinAllowLists.h"
#include "APinData.h"
#include "APinDrag.h"
#include "APinDataRegistry.h"
#include "../wire/AWireItem.h"
#include "../registry/IRegistry.h"
#include <functional>
#include <cstdint>
#include <memory>

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QGraphicsSceneDragDropEvent>
#include <QSvgRenderer>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QList>
#include <QVariant>
#include <QReadWriteLock>
#include <QPainter>


class PinItem : public QGraphicsSvgItem {
private:
    QList<AWireItem*> connectedWires;
    APinData::PinData pPinData;

    std::shared_ptr<APinAllowLists::AllowLists> allowLists = nullptr;

    QPointF dragStartPosition;
    std::move_only_function<bool(const QByteArray&)> isBodyAllowed = nullptr;

    const bool hasDraggingStarted(QGraphicsSceneMouseEvent* event) {
        return (event->screenPos() - dragStartPosition).manhattanLength() >= QApplication::startDragDistance();
    }

    const bool createPermanentWire(QGraphicsSceneDragDropEvent* event) {
        quintptr sourceAddress = event->mimeData()->property(APinDrag::Drag::mimeProperty::sourcePinItemPtr()).value<quintptr>();
        QGraphicsItem* sourceItem = reinterpret_cast<QGraphicsItem*>(sourceAddress);
        PinItem* sourcePin = dynamic_cast<PinItem*>(sourceItem);

        if (!sourcePin || sourcePin == this) return false;

        APinData::PinData originPinData = APinDrag::Drag::finishDrag(event);
        if (!isConnectable(originPinData)) return false;

        auto sourceFlow = APinRegistry::Flow::at(sourcePin->pinData().flow());
        if (!sourceFlow) return false;
        auto targetFlow = APinRegistry::Flow::at(pPinData.flow());
        if (!targetFlow) return false;

        AWireItem* permanentWire = new AWireItem(
            sourcePin,
            this, 
            (sourceFlow.value()).value().degree,
            (targetFlow.value()).value().degree
        );
        scene()->addItem(permanentWire);

        sourcePin->registerWire(permanentWire);
        this->registerWire(permanentWire);
        return true;
    }

public:
    PinItem(QGraphicsItem* parent, const QString& iconPath = QStringLiteral(":/icons/outline/point.svg")) : QGraphicsSvgItem(parent) {
        setSharedRenderer(new QSvgRenderer(iconPath, this));
        setAcceptDrops(true);
        setAcceptedMouseButtons(Qt::LeftButton);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
        this->update();
    }

    // Pin Data Body
    void setIsBodyAllowedFn(std::move_only_function<bool(const QByteArray&)> newCheck) { isBodyAllowed = std::move(newCheck); }
    void setIsBodyAllowedFn() { isBodyAllowed = nullptr; }
    bool IsBodyAllowed(const QByteArray& body) {
        if (!isBodyAllowed) return true;
        return isBodyAllowed(body);
    }

    // Get
    APinData::PinData& pinData() { return pPinData; }
    std::shared_ptr<APinAllowLists::AllowLists> getAllowLists() {
        if (!allowLists) return nullptr;
        return allowLists;
    }

    // Set
    void pinData(const APinData::PinData& newData) {
        if (pPinData == newData) return;

        pPinData = newData;
        this->update();
    }
    void setAllowLists(std::shared_ptr<APinAllowLists::AllowLists> newLists) {
        allowLists = std::move(newLists);
    }

    // Is Allowed
    bool isFlowAllowed(const IRegistry::FRegistryKey& key) { 
        if (!allowLists) return true;
        return allowLists->flow().contains(key);
    }
    bool isTypeAllowed(const IRegistry::FRegistryKey& key) {
        if (!allowLists) return true;
        return allowLists->type().contains(key);
    }

    // Wiring Check
    bool isConnectable(APinData::PinData& otherData) {
        if (!isFlowAllowed(otherData.flow()) || !isTypeAllowed(otherData.type())) return false;
        if (!IsBodyAllowed(otherData.body())) return false;
        return true;
    }

    // Register
    void registerWire(AWireItem* wire) { connectedWires.append(wire); }
    void unregisterWire(AWireItem* wire) { connectedWires.removeOne(wire); }

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
            APinDrag::Drag::useDrag(event, this, pPinData, mapToScene(boundingRect().center()));
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override { event->accept(); }

    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override { event->acceptProposedAction(); }
    void dragMoveEvent( QGraphicsSceneDragDropEvent* event) override { event->acceptProposedAction(); }

    void dropEvent(QGraphicsSceneDragDropEvent* event) override {
        if (!event->mimeData()->hasFormat(APinDrag::Drag::mimeType())) return;
        event->acceptProposedAction();

        createPermanentWire(event);
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        QRectF rect = boundingRect().toAlignedRect();;
        QSize size = rect.toRect().size();

        if (size.isEmpty()) {
            QGraphicsSvgItem::paint(painter, option, widget);
            return;
        }

        QImage svgImage(size, QImage::Format_ARGB32_Premultiplied);
        svgImage.fill(Qt::transparent);

        QPainter imgPainter(&svgImage);
        QGraphicsSvgItem::paint(&imgPainter, option, widget);
        imgPainter.end();

        QPainter imgColorPainter(&svgImage);
        imgColorPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);

        QColor pinColor = Qt::gray;
        if (auto tempOpt = APinRegistry::Style::at(pPinData.style()))
            pinColor = tempOpt.value().value().color;

        imgColorPainter.fillRect(svgImage.rect(), pinColor);
        imgColorPainter.end();

        painter->drawImage(rect.topLeft(), svgImage);
    }
};