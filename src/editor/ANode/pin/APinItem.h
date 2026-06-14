#pragma once

#include "APinAllowLists.h"
#include "APinData.h"
#include "APinDrag.h"
#include "APinDataRegistry.h"
#include "APinFlags.h"
#include <functional>
#include <cstdint>
#include <memory>
#include <atomic>

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

import Utility;
import AWire;

namespace APinItem {

    class PinItem : public QGraphicsSvgItem {
    private:
        QList<AWire::WireItem*> connectedWires;
        std::weak_ptr<APinData::PinData> pPinData;
        std::weak_ptr<APinAllowLists::AllowLists> pAllowLists;

        QPointF dragStartPosition;

        APinFlags::PinFlags pFlags;

        const bool hasDraggingStarted(QGraphicsSceneMouseEvent* event) {
            return (event->screenPos() - dragStartPosition).manhattanLength() >= QApplication::startDragDistance();
        }

        const bool createPermanentWire(QGraphicsSceneDragDropEvent* event) {
            if (pFlags.allowMultipleWires != 1 && connectedWires.size() > 0) return false;

            auto data = pPinData.lock();
            if (!data) return false;

            quintptr sourceAddress = event->mimeData()->property(APinDrag::Drag::mimeProperty::sourcePinItemPtr()).value<quintptr>();
            QGraphicsItem* sourceItem = reinterpret_cast<QGraphicsItem*>(sourceAddress);
            PinItem* sourcePin = dynamic_cast<PinItem*>(sourceItem);

            if (!sourcePin || sourcePin == this) return false;

            auto sourceData = sourcePin->pinData();
            if (!sourceData) return false;

            APinData::PinData originPinData = APinDrag::Drag::finishDrag(event);
            if (!isConnectable(originPinData)) return false;

            auto sourceFlow = APinRegistry::Flow::at(sourceData->flow());
            if (!sourceFlow) return false;
            auto targetFlow = APinRegistry::Flow::at(data->flow());
            if (!targetFlow) return false;

            AWire::WireItem* permanentWire = new AWire::WireItem(
                sourcePin,
                this,
                sourceFlow.value().degree,
                targetFlow.value().degree
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

        // Get
        std::shared_ptr<APinData::PinData> pinData() { return pPinData.lock(); }
        std::shared_ptr<APinAllowLists::AllowLists> allowLists() { return pAllowLists.lock(); }
        APinFlags::PinFlags& flags() { return pFlags; }

        // Set
        void pinData(std::shared_ptr<APinData::PinData> newData) {
            pPinData = std::move(newData);
            this->update();
        }
        void allowLists(std::shared_ptr<APinAllowLists::AllowLists> newLists) { pAllowLists = std::move(newLists); }
        void svg(const QString& iconPath) { setSharedRenderer(new QSvgRenderer(iconPath, this)); }

        // Is Allowed
        bool isFlowAllowed(const FRegistryKey::FRegistryKey& key) const {
            auto lists = pAllowLists.lock();
            if (!lists || lists->flow().size() == 0) return pFlags.defaultAllowFlowValue == 1;
            return lists->flow().contains(key);
        }
        bool isTypeAllowed(const FRegistryKey::FRegistryKey& key) const {
            auto lists = pAllowLists.lock();
            if (!lists || lists->type().size() == 0) return pFlags.defaultAllowTypeValue == 1;
            return lists->type().contains(key);
        }

        // Wiring Check
        bool isConnectable(APinData::PinData& otherData) const { return isFlowAllowed(otherData.flow()) && isTypeAllowed(otherData.type()); }

        // Register
        void registerWire(AWire::WireItem* wire) { connectedWires.append(wire); }
        void unregisterWire(AWire::WireItem* wire) { connectedWires.removeOne(wire); }

    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant& value) override {
            if (change == ItemScenePositionHasChanged)
                for (AWire::WireItem* wire : connectedWires) { wire->updatePath(); }

            return QGraphicsItem::itemChange(change, value);
        }

        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            event->accept();
            dragStartPosition = event->screenPos();
        }
        void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
            event->accept();

            if (auto data = pPinData.lock(); data && hasDraggingStarted(event))
                APinDrag::Drag::useDrag(event, this, *data, mapToScene(boundingRect().center()));
        }
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override { event->accept(); }

        void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override { event->acceptProposedAction(); }
        void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override { event->acceptProposedAction(); }

        void dropEvent(QGraphicsSceneDragDropEvent* event) override {
            if (!event->mimeData()->hasFormat(APinDrag::Drag::mimeType())) return;
            event->acceptProposedAction();

            createPermanentWire(event);
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
            auto data = pPinData.lock();
            if (!data) return;

            QRectF rect = boundingRect().toAlignedRect();;
            QSize size = rect.toRect().size();

            if (size.isEmpty()) {
                QGraphicsSvgItem::paint(painter, option, widget);
                return;
            }

            QImage svgImage(size, QImage::Format_ARGB32_Premultiplied);
            svgImage.fill(Qt::transparent);

            QPainter imgPainter(&svgImage);
            if (auto* svgRender = renderer())
                svgRender->render(&imgPainter, QRectF(QPointF(0, 0), size));
            imgPainter.end();

            QPainter imgColorPainter(&svgImage);
            imgColorPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);

            QColor pinColor = Qt::gray;
            if (auto tempOpt = APinRegistry::Style::at(data->style()))
                pinColor = tempOpt.value().color;

            imgColorPainter.fillRect(svgImage.rect(), pinColor);
            imgColorPainter.end();

            painter->drawImage(rect.topLeft(), svgImage);
        }
    };
}