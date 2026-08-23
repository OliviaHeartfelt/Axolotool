#pragma once

#include "../../../Registry/ARegistry.h"
#include "../../../Storage/ANodeEnvDB.h"
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
        ANodeEnvDB::ANodeEnvDB* m_nodeEnvDB = nullptr;

        const char* mimePosPropertyStr = "sourcePinItemPtr";
        const QString mimeType = QStringLiteral("application/x-anode-data");
        QString iconPath = QStringLiteral(":/icons/outline/point.svg");

        QMarginsF m_padding{ -8.0, -8.0, 8.0, 8.0 };

        QSvgRenderer m_svgRenderer;
        QByteArray m_rawSvgTemplate;
        QColor m_currentRenderColor;

        QPointF dragStartPosition {0.0, 0.0};
        QList<VWWire::PermanentWire::WireItem*> connectedWires;

        VWPinDetails::PinAllowSet::VWPinAllowSet m_allowFlowSet;
        VWPinDetails::PinAllowSet::VWPinAllowSet m_allowTypeSet;
        PinData::PinData m_pinData;

        muuid::uuid m_id;
        muuid::uuid m_core_id;

        bool m_is_new;
        bool m_is_dirty = false;

        bool isSvgDirty = true;
        bool isHighlighted = false;

        bool hasDraggingStarted(QGraphicsSceneMouseEvent* event) {
            return (event->screenPos() - dragStartPosition).manhattanLength() >= QApplication::startDragDistance();
        }

    public:
        PinItem(ARegistry::Registry* registry, ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QGraphicsItem* parent, const std::optional<muuid::uuid>& id, const muuid::uuid& coreId, const Context::FactoryData& factoryData)
            : m_registry(registry), m_nodeEnvDB(nodeEnvDB), QGraphicsSvgItem(parent), m_core_id(coreId)
        {
            if (id) {
                m_id = *id;
                bool m_is_new = false;
            }
            else {
                m_id = muuid::uuid::generate_unix_time_based();
                bool m_is_new = true;
            }

            if (factoryData.flow)  m_pinData.flow(*factoryData.flow);
            if (factoryData.type)  m_pinData.type(*factoryData.type);
            if (factoryData.style) m_pinData.style(*factoryData.style);
            if (factoryData.allowFlowVec) m_allowFlowSet.insert(*factoryData.allowFlowVec);
            if (factoryData.allowTypeVec) m_allowTypeSet.insert(*factoryData.allowTypeVec);

            setSharedRenderer(new QSvgRenderer(iconPath, this));
            setAcceptDrops(true);
            setAcceptedMouseButtons(Qt::LeftButton);
            setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

            setSvgSource();

            update();
        }

        const muuid::uuid& id() const { return m_id; }
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

        void setSvgSource(const QString& filePath = QStringLiteral(":/icons/outline/point.svg")) {
            QWriteLocker locker(&lock);
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly)) {
                m_rawSvgTemplate = file.readAll();
                isSvgDirty = true;
            }
        }
        
        void registerWire(VWWire::PermanentWire::WireItem* wire) {
            QWriteLocker locker(&lock);
            connectedWires.append(wire);
            isSvgDirty = true;
            update();
        }
        void unregisterWire(VWWire::PermanentWire::WireItem* wire) {
            QWriteLocker locker(&lock);
            connectedWires.removeOne(wire);
            isSvgDirty = true;
            update();
        }
        void removeWire(VWWire::PermanentWire::WireItem* wire) {
            if (!wire || !wire->origin() || !wire->target()) return;

            auto* originPin = dynamic_cast<PinItem*>(wire->origin());
            auto* targetPin = dynamic_cast<PinItem*>(wire->target());

            if (originPin) originPin->unregisterWire(wire);
            if (targetPin) targetPin->unregisterWire(wire);

            if (QGraphicsScene* s = wire->scene()) {
                s->removeItem(wire);
            }

            m_registry->wireView.wireViewRegistry.hide(wire->id());
            qDebug() << "> Wire Removed! #Wires:" << m_registry->wireView.wireViewRegistry.sizeVisible() + 1 << "->" << m_registry->wireView.wireViewRegistry.sizeVisible();
        }
        void removeAllWires() {
            for (auto* wire : connectedWires) {
                if (!wire) {
                    unregisterWire(wire);
                    continue;
                }

                removeWire(wire);
            }
        }

        bool isCompatibleSourcePin(const PinItem* originPin) const {
            if (!originPin || originPin == this) return false;

            if (originPin->parentItem() == this->parentItem()) return false;

            if (!m_allowFlowSet.contains(originPin->pinData().flow())) return false;
            if (!m_allowTypeSet.contains(originPin->pinData().type())) return false;

            if (!originPin->allowFlowSet().contains(m_pinData.flow())) return false;
            if (!originPin->allowTypeSet().contains(m_pinData.type())) return false;

            for (auto* wire : connectedWires) {
                if (!wire) continue;
                if (wire->origin() == originPin || wire->target() == originPin) {
                    VWWire::TemporaryWire::WireTemp::unstuck();
                    return false;
                }
            }

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
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override { event->accept(); }
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override {
            if (event->button() == Qt::LeftButton && !connectedWires.empty()) {
                removeAllWires();
                event->accept();
                return;
            }
            QGraphicsItem::mouseDoubleClickEvent(event);
        }

        void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
            if (m_registry && hasDraggingStarted(event)) {
                event->accept();

                if (scene()) scene()->clearSelection();

                VWDragDrop::useDrag(event, *m_registry, m_nodeEnvDB, this,
                    mimeType, m_pinData, mimePosPropertyStr,
                    mapToScene(boundingRect().center()));
                return;
            }

            QGraphicsItem::mouseMoveEvent(event);
        }
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

            if (!m_registry || !originPin || originPin == this || !isCompatibleSourcePin(originPin)) {
                event->ignore();
                return;
            }

            QGraphicsScene* scene = originPin->scene();
            if (!m_registry || !scene) {
                VWWire::TemporaryWire::WireTemp::unstuck();
                event->ignore();
                return;
            }

            auto* wire = VWWire::PermanentWire::createPermanentWire(originPin, this, m_registry, m_nodeEnvDB, std::nullopt);
            if (!wire) {
                VWWire::TemporaryWire::WireTemp::unstuck();
                event->ignore();
                return;
            }

            event->acceptProposedAction();
            //scene->addItem(wire);

            //originPin->registerWire(wire);
            //this->registerWire(wire);

            VWWire::TemporaryWire::WireTemp::unstuck();
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
            QRectF rect = boundingRect();
            if (rect.isEmpty() || m_rawSvgTemplate.isEmpty()) return;

            QColor pinColor = Qt::gray;
            if (m_registry) {
                if (const auto style = m_registry->node.pinStyleRegistry.at(m_pinData.style()))
                    pinColor = style->color;
            }
            if (isHighlighted) {
                pinColor = pinColor.lighter(150);
            }

            if (isSvgDirty || m_currentRenderColor != pinColor) {
                m_currentRenderColor = pinColor;

                QByteArray hexColor = pinColor.name(QColor::HexRgb).toUtf8();
                QByteArray coloredSvg = m_rawSvgTemplate;

                coloredSvg.replace("currentColor", hexColor);

                m_svgRenderer.load(coloredSvg);
                isSvgDirty = false;
            }

            QSize svgSize = m_svgRenderer.defaultSize();
            QRectF targetRect(
                rect.x() + (rect.width() - svgSize.width()) / 2.0,
                rect.y() + (rect.height() - svgSize.height()) / 2.0,
                svgSize.width(),
                svgSize.height()
            );

            m_svgRenderer.render(painter, targetRect);
        }
    };
    static_assert(Concepts::PinItemConcept<PinItem, VWWire::PermanentWire::WireItem>);
}