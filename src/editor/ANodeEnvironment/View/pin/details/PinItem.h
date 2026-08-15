#pragma once

#include "../../../Registry/ARegistry.h"
#include "../../drag_drop/VWDragDrop.h"
#include "../../wire/VWWire.h"
#include "PinData.h"
#include "PinAllowSet.h"
#include "Concepts.h"
#include "Context.h"

namespace VWPinDetails::PinItem {

	class PinItem : public QGraphicsSvgItem {
        mutable QReadWriteLock lock;
        ARegistry::Registry* m_registry = nullptr;

        const char* mimePosPropertyStr = "sourcePinItemPtr";
        const QString mimeType = QStringLiteral("application/x-anode-data");
        QString iconPath = QStringLiteral(":/icons/outline/point.svg");

        QImage rasterCache;
        QMarginsF m_padding{ -8.0, -8.0, 8.0, 8.0 };

        QPointF dragStartPosition {0.0, 0.0};
        QList<VWWire::PermanentWire::WireItem*> connectedWires;

        VWPinDetails::PinAllowSet::VWPinAllowSet m_allowFlowSet;
        VWPinDetails::PinAllowSet::VWPinAllowSet m_allowTypeSet;
        PinData::PinData m_pinData;

        muuid::uuid m_core_id;

        bool m_is_new = true;
        bool m_is_dirty = false;

        bool isSvgDirty = true;
        bool isHighlighted = false;

        void initPinItem() {
            setSharedRenderer(new QSvgRenderer(iconPath, this));
            setAcceptDrops(true);
            setAcceptedMouseButtons(Qt::LeftButton);
            setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
            update();
        }

        bool hasDraggingStarted(QGraphicsSceneMouseEvent* event) {
            return (event->screenPos() - dragStartPosition).manhattanLength() >= QApplication::startDragDistance();
        }

    public:
        PinItem(ARegistry::Registry* registry, QGraphicsItem* parent, const muuid::uuid& coreId, const Context::FactoryData& factoryData)
            : m_registry(registry), QGraphicsSvgItem(parent), m_core_id(coreId)
        {
            if (factoryData.flow)  m_pinData.flow(*factoryData.flow);
            if (factoryData.type)  m_pinData.type(*factoryData.type);
            if (factoryData.style) m_pinData.style(*factoryData.style);
            if (factoryData.allowFlowVec) m_allowFlowSet.insert(*factoryData.allowFlowVec);
            if (factoryData.allowTypeVec) m_allowTypeSet.insert(*factoryData.allowTypeVec);

            initPinItem();
        }

        const muuid::uuid& coreId() const { return m_core_id; }

        bool isUpdateNeeded() const { return m_is_dirty; }
        void setUpdate(bool value) { m_is_dirty = value; }

        bool isNew() const { return m_is_new; }
        void setIsNew(bool value) { m_is_new = value; }

        void setPadding(const QMarginsF& margins) {
            if (m_padding == margins) return;

            prepareGeometryChange();
            m_padding = margins;
            update();
        }
        QRectF boundingRect() const override {
            return QGraphicsSvgItem::boundingRect().adjusted(
                m_padding.left(),
                m_padding.top(),
                m_padding.right(),
                m_padding.bottom()
            );
        }
        QPainterPath shape() const override {
            QPainterPath path;
            path.addRect(boundingRect());
            return path;
        }
        void safeUpdate() {
            QMetaObject::invokeMethod(this, [this]() {
                isSvgDirty = true;
                this->update();
            }, Qt::QueuedConnection);
        }

        PinData::PinData& pinData() { return m_pinData; }
        const PinData::PinData& pinData() const { return m_pinData; }

        VWPinDetails::PinAllowSet::VWPinAllowSet& allowFlowSet() { return m_allowFlowSet; }
        const VWPinDetails::PinAllowSet::VWPinAllowSet& allowFlowSet() const { return m_allowFlowSet; }

        VWPinDetails::PinAllowSet::VWPinAllowSet& allowTypeSet() { return m_allowTypeSet; }
        const VWPinDetails::PinAllowSet::VWPinAllowSet& allowTypeSet() const { return m_allowTypeSet; }

        void setSvg(const QString& iconPath = QStringLiteral(":/icons/outline/point.svg")) {
            QMetaObject::invokeMethod(this, [this, iconPath]() {
                if (auto* oldRenderer = renderer())
                    oldRenderer->deleteLater();
                setSharedRenderer(new QSvgRenderer(iconPath, this));
                isSvgDirty = true;
                this->update();
            }, Qt::QueuedConnection);
        }
        
        void registerWire(VWWire::PermanentWire::WireItem* wire) {
            QWriteLocker locker(&lock);
            connectedWires.append(wire);
        }
        void unregisterWire(VWWire::PermanentWire::WireItem* wire) {
            QWriteLocker locker(&lock);
            connectedWires.removeOne(wire);
        }
        void destroyWire(VWWire::PermanentWire::WireItem* wire) {
            if (!wire) return;

            auto* originPin = dynamic_cast<PinItem*>(wire->origin());
            auto* targetPin = dynamic_cast<PinItem*>(wire->target());

            if (originPin) originPin->unregisterWire(wire);
            if (targetPin) targetPin->unregisterWire(wire);

            if (QGraphicsScene* s = wire->scene()) {
                s->removeItem(wire);
            }

            delete wire;
        }

        bool isCompatibleSourcePin(const PinItem* originPin) const {
            if (!originPin || originPin == this) return false;

            if (originPin->parentItem() == this->parentItem()) return false;

            if (!m_allowFlowSet.contains(originPin->pinData().flow())) return false;
            if (!m_allowTypeSet.contains(originPin->pinData().type())) return false;

            if (!originPin->allowFlowSet().contains(m_pinData.flow())) return false;
            if (!originPin->allowTypeSet().contains(m_pinData.type())) return false;

            return true;
        }

    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant& value) override {
            if (change == ItemScenePositionHasChanged) {
                QList<VWWire::PermanentWire::WireItem*> wiresCopy;
                {
                    QReadLocker locker(&lock);
                    wiresCopy = connectedWires;
                }
                for (VWWire::PermanentWire::WireItem* wire : wiresCopy) {
                    if (wire) wire->updatePath();
                }
            }

            return QGraphicsItem::itemChange(change, value);
        }

        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            event->accept();
            dragStartPosition = event->screenPos();
        }
        void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
            event->accept();

            if (m_registry && hasDraggingStarted(event))
                VWDragDrop::useDrag(event, *m_registry, this, mimeType, m_pinData, mimePosPropertyStr, mapToScene(boundingRect().center()));
        }
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override { event->accept(); }

        void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override {
            if (!event->mimeData()->hasFormat(mimeType)) {
                event->ignore();
                return;
            }

            const auto sourcePtr = event->mimeData()->property(mimePosPropertyStr).value<quintptr>();
            auto* sourcePin = reinterpret_cast<PinItem*>(sourcePtr);

            if (sourcePin && isCompatibleSourcePin(sourcePin)) {
                event->setDropAction(Qt::LinkAction);
                event->accept();

                isHighlighted = true;
                isSvgDirty = true;
                update();

                VWWire::TemporaryWire::WireTemp::stuck(mapToScene(boundingRect().center()));
            }
            else {
                event->ignore();
            }
        }
        void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override {
            isHighlighted = false;
            isSvgDirty = true;
            update();

            VWWire::TemporaryWire::WireTemp::unstuck();

            QGraphicsSvgItem::dragLeaveEvent(event);
        }

        void dropEvent(QGraphicsSceneDragDropEvent* event) override {
            isHighlighted = false;
            isSvgDirty = true;
            update();

            const auto originPtr = event->mimeData()->property(mimePosPropertyStr).value<quintptr>();
            auto* originPin = reinterpret_cast<PinItem*>(originPtr);

            if (originPin && originPin != this && isCompatibleSourcePin(originPin)) {
                event->acceptProposedAction();

                if (!m_registry) return;

                QGraphicsScene* scene = originPin->scene();
                if (!scene) return;

                const auto originPinFlowOpt = m_registry->node.pinFlowRegistry.at(originPin->pinData().flow());
                if (!originPinFlowOpt) return;

                const auto targetPinFlowOpt = m_registry->node.pinFlowRegistry.at(m_pinData.flow());
                if (!targetPinFlowOpt) return;

                auto* wire = VWWire::PermanentWire::createPermanentWire(originPin, this, *m_registry);
                if (!wire) return;
                scene->addItem(wire);

                originPin->registerWire(wire);
                this->registerWire(wire);

                qDebug() << "Successfully created wire link between pins!";
            }
            else {
                event->ignore();
            }
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
            QRectF rect = boundingRect();
            QSize size = rect.size().toSize();

            if (size.isEmpty()) return;

            if (isSvgDirty || rasterCache.size() != size) {
                rasterCache = QImage(size, QImage::Format_ARGB32_Premultiplied);
                rasterCache.fill(Qt::transparent);

                if (auto* svgRender = renderer()) {

                    QPainter imgPainter(&rasterCache);
                    QSize svgSize = svgRender->defaultSize();
                    QRectF svgTargetRect(
                        (size.width() - svgSize.width()) / 2.0,
                        (size.height() - svgSize.height()) / 2.0,
                        svgSize.width(),
                        svgSize.height()
                    );
                    svgRender->render(&imgPainter, svgTargetRect);
                    imgPainter.end();

                    QPainter imgColorPainter(&rasterCache);
                    imgColorPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);

                    QColor pinColor = Qt::gray;
                    if (m_registry) {
                        if (const auto style = m_registry->node.pinStyleRegistry.at(m_pinData.style()))
                            pinColor = style->color;
                    }
                    if (isHighlighted) {
                        pinColor = pinColor.lighter(150);
                    }

                    imgColorPainter.fillRect(rasterCache.rect(), pinColor);
                    imgColorPainter.end();
                }

                isSvgDirty = false;
            }
            painter->drawImage(rect.topLeft(), rasterCache);
        }
    };
    static_assert(Concepts::PinItemConcept<PinItem>);
}