module;

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
#include <QPainter>
#include <QtTypes>
#include <QGraphicsItem>
#include <QString>
#include <QMimeData>
#include <QMetaObject>

export module APinItem;

import Utility;
import AWire;
import ARegistry;

import APinAllowLists;
import APinData;
import APinDrag;
import APinRegistry;
import APinFlags;

export namespace APinItem {

    class PinItem : public QGraphicsSvgItem {
    private:
        std::atomic<APinFlags::PinFlags> pFlags;
        QPointF dragStartPosition;

        std::atomic<std::shared_ptr<APinData::PinData>> pPinData;
        std::atomic<std::shared_ptr<APinAllowLists::AllowLists>> pAllowLists;
        QList<AWire::WireItem*> connectedWires;


        bool hasDraggingStarted(QGraphicsSceneMouseEvent* event) {
            return (event->screenPos() - dragStartPosition).manhattanLength() >= QApplication::startDragDistance();
        }

        bool createPermanentWire(QGraphicsSceneDragDropEvent* event) {
            if (pFlags.load().allowMultipleWires != 1 && connectedWires.size() > 0) return false;

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
            this->registerWire(permanentWire);
            return true;
        }
        
        void setSVG(const QString& iconPath) {
            QMetaObject::invokeMethod(this, [this, iconPath]() {
                if (auto* oldRenderer = renderer())
                    oldRenderer->deleteLater();
                setSharedRenderer(new QSvgRenderer(iconPath, this));
            }, Qt::QueuedConnection);
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

        void safeUpdate() { QMetaObject::invokeMethod(this, [this]() { this->update(); }, Qt::QueuedConnection); }

        // Get
        std::shared_ptr<APinData::PinData> pinData() { return pPinData.load(); }
        std::shared_ptr<APinAllowLists::AllowLists> allowLists() { return pAllowLists.load(); }
        APinFlags::PinFlags flags() const { return pFlags.load(); }

        // Set
        void pinData(std::shared_ptr<APinData::PinData> newData) { 
            pPinData.store(std::move(newData));
            safeUpdate();
        }
        void allowLists(std::shared_ptr<APinAllowLists::AllowLists> newLists) { pAllowLists.store(std::move(newLists)); }
        void flags(const APinFlags::PinFlags& newFlags) { pFlags.store(newFlags); }
        void svg(const QString& iconPath) { 
            QMetaObject::invokeMethod(this, [this, iconPath]() {
                if (auto* oldRenderer = renderer())
                    oldRenderer->deleteLater();
                setSharedRenderer(new QSvgRenderer(iconPath, this));
            }, Qt::QueuedConnection);
        }

        // Is Allowed
        bool isFlowAllowed(const ARegistry::FRegistryKey& key) const {
            if (const auto& list = pAllowLists.load(); !list || list->flow.size() == 0)
                return pFlags.load().defaultAllowFlowValue == 1;
            else
                return list->flow.contains(key);
        }
        bool isTypeAllowed(const ARegistry::FRegistryKey& key) const {
            if (const auto& list = pAllowLists.load(); !list || list->type.size() == 0)
                return pFlags.load().defaultAllowTypeValue == 1;
            else
                return list->type.contains(key);
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

            if (auto data = pPinData.load(); data && hasDraggingStarted(event))
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

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
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
            if (auto data = pPinData.load()) {
                if (auto tempOpt = APinRegistry::Style::at(data->style()))
                    pinColor = tempOpt.value().color;
            }
            imgColorPainter.fillRect(svgImage.rect(), pinColor);
            imgColorPainter.end();

            painter->drawImage(rect.topLeft(), svgImage);
        }
    };
}