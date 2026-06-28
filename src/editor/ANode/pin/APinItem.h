#pragma once

#include "../wire/AWire.h"
#include "../registry/ARegistry.h"

#include "APinAllowLists.h"
#include "APinData.h"
#include "APinDrag.h"
#include "APinRegistry.h"
#include "APinFlags.h"

namespace APinItem {

    class PinItem : public QGraphicsSvgItem {
        mutable QReadWriteLock lock;

        APinFlags::PinFlags pFlags;
        QColor pinColor = Qt::gray;
        QPointF dragStartPosition{};

        std::atomic<std::shared_ptr<APinData::PinData>> pPinData;
        std::atomic<std::shared_ptr<APinAllowLists::AllowLists>> pAllowLists;

        QImage rasterCache;
        QList<AWire::WireItem*> connectedWires;


        bool hasDraggingStarted(QGraphicsSceneMouseEvent* event) {
            return (event->screenPos() - dragStartPosition).manhattanLength() >= QApplication::startDragDistance();
        }

        bool createPermanentWire(QGraphicsSceneDragDropEvent* event) {
            QWriteLocker locker(&lock);
            if (pFlags.allowMultipleWires != 1 && connectedWires.size() > 0) return false;

            auto data = pPinData.load();
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
            connectedWires.append(permanentWire);
            return true;
        }

        void updateColorAndCache() {
            if (auto data = pPinData.load()) {
                if (auto tempOpt = APinRegistry::Style::at(data->style())) {
                    QColor newColor = tempOpt.value().color;
                    if (pinColor != newColor) {
                        pinColor = newColor;
                        pFlags.isColorDirty = 1;
                    }
                }
            }
        }

    public:
        PinItem(QGraphicsItem* parent, const QString& iconPath = QStringLiteral(":/icons/outline/point.svg")) : 
            QGraphicsSvgItem(parent), 
            pPinData(std::make_shared<APinData::PinData>()),
            pAllowLists(std::make_shared<APinAllowLists::AllowLists>())
        {
            setSharedRenderer(new QSvgRenderer(iconPath, this));
            setAcceptDrops(true);
            setAcceptedMouseButtons(Qt::LeftButton);
            setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
            this->update();
        }

        void safeUpdate() { QMetaObject::invokeMethod(this, [this]() { 
            updateColorAndCache();
            this->update(); 
        }, Qt::QueuedConnection); }

        // Get
        std::shared_ptr<APinData::PinData> pinData() const { return pPinData.load(); }
        std::shared_ptr<APinAllowLists::AllowLists> allowLists() { return pAllowLists.load(); }
        APinFlags::PinFlags flags() const { return pFlags; }

        // Set
        void pinData(std::shared_ptr<APinData::PinData> newData) { 
            pPinData.store(std::move(newData));
            safeUpdate();
        }
        void allowLists(std::shared_ptr<APinAllowLists::AllowLists> newLists) { pAllowLists.store(std::move(newLists)); }

        void svg(const QString& iconPath) { 
            QMetaObject::invokeMethod(this, [this, iconPath]() {
                if (auto* oldRenderer = renderer())
                    oldRenderer->deleteLater();
                setSharedRenderer(new QSvgRenderer(iconPath, this));
                pFlags.isColorDirty = 1;
                updateColorAndCache();
                this->update();
            }, Qt::QueuedConnection);
        }

        // Is Allowed
        bool isFlowAllowed(const ARegistry::FRegistryKey& key) const {
            if (const auto& list = pAllowLists.load(); !list || list->flow.size() == 0)
                return pFlags.defaultAllowFlowValue == 1;
            else
                return list->flow.contains(key);
        }
        bool isTypeAllowed(const ARegistry::FRegistryKey& key) const {
            if (const auto& list = pAllowLists.load(); !list || list->type.size() == 0)
                return pFlags.defaultAllowTypeValue == 1;
            else
                return list->type.contains(key);
        }

        // Wiring Check
        bool isConnectable(APinData::PinData& otherData) const {
            return isFlowAllowed(otherData.flow()) && isTypeAllowed(otherData.type());
        }

        // Wire
        void registerWire(AWire::WireItem* wire) { 
            QWriteLocker locker(&lock);
            connectedWires.append(wire);
        }
        void unregisterWire(AWire::WireItem* wire) { 
            QWriteLocker locker(&lock);
            connectedWires.removeOne(wire);
        }

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

            if (auto data = pPinData.load(); data && hasDraggingStarted(event))
                    APinDrag::Drag::useDrag(event, this, *data, mapToScene(boundingRect().center()));
        }
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override { event->accept(); }

        void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override { event->acceptProposedAction(); }
        void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override { event->acceptProposedAction(); }

        void dropEvent(QGraphicsSceneDragDropEvent* event) override {
            if (!event->mimeData()->hasFormat(APinDrag::Drag::mimeType())) return;
            event->acceptProposedAction();

            dragStartPosition = { 0.0, 0.0 };
            createPermanentWire(event);
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
            QRectF rect = boundingRect().toAlignedRect();
            QSize size = rect.toRect().size();

            if (size.isEmpty()) {
                QGraphicsSvgItem::paint(painter, option, widget);
                return;
            }

            if (pFlags.isColorDirty == 1 || rasterCache.size() != size) {
                rasterCache = QImage(size, QImage::Format_ARGB32_Premultiplied);
                rasterCache.fill(Qt::transparent);

                QPainter imgPainter(&rasterCache);
                if (auto* svgRender = renderer())
                    svgRender->render(&imgPainter, QRectF(QPointF(0, 0), size));
                imgPainter.end();

                QPainter imgColorPainter(&rasterCache);
                imgColorPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);

                imgColorPainter.fillRect(rasterCache.rect(), pinColor);
                imgColorPainter.end();

                pFlags.isColorDirty = 0;
            }
            painter->drawImage(rect.topLeft(), rasterCache);
        }
    };
}