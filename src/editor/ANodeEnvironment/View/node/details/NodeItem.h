#pragma once

#include "NodeGrid.h"
#include "../../pin/VWPin.h"
#include "../../cell/details/CellItem.h"
#include "../../../Storage/ANodeEnvDB.h"

namespace VWNodeDetails::NodeItem {

    struct NodeGridConfig {
        int totalRows = 1;
        int totalCols = 1; 
        qreal margin = 6.0;
        qreal spacing = 4.0;
    };

    enum StyleKey {
        SelectedPen = 1000,
        CornerRadius = 1001,
        HeaderBrush = 1002
    };

    class Node : public QGraphicsRectItem {
        QPen   p_borderPen = QPen(QColor("#455a64"), 1);
        QPen   p_selectedPen = QPen(QColor("#f1c40f"), 2, Qt::DashLine);
        QBrush p_bgBrush = QBrush(QColor("#34495e"));
        qreal  p_cornerRadius = 5.0;

        muuid::uuid m_id;
        muuid::uuid m_core_id;

        bool m_is_new;
        bool m_is_drty = false;
        bool m_is_moving = false;

    public:
        static std::optional<ANodeEnvDB::Config::Node::FullNodeRecord> toRecord(const Node& nodeItem) {
            auto* body = nodeItem.body.get();
            if (!body) return std::nullopt;

            ANodeEnvDB::Config::Node::FullNodeRecord nodeRecord{
                .id = nodeItem.m_id,
                .coreId = nodeItem.m_core_id,
                .name = "",
                .rowNum = static_cast<short>(body->rowNum()),
                .colNum = static_cast<short>(body->colNum()),
                .pos = nodeItem.pos(),
                .width = nodeItem.rect().width(),
                .height = nodeItem.rect().height(),
                .state = std::nullopt
            };
            return std::move(nodeRecord);
        }

        Node(QGraphicsItem* parent, const muuid::uuid& coreId, const std::optional<muuid::uuid>& id = std::nullopt, const std::optional<NodeGridConfig>& nodeConfig = std::nullopt)
            : QGraphicsRectItem(parent), m_core_id(coreId) {
            
            if (id) {
                m_id = *id;
                m_is_new = false;
            }
            else {
                m_id = muuid::uuid::generate_unix_time_based();
                m_is_new = true;
            }

            setFlags(
                QGraphicsItem::ItemIsMovable |
                QGraphicsItem::ItemIsSelectable |
                QGraphicsItem::ItemSendsGeometryChanges
            );
            
            if (nodeConfig) {
                body = std::make_unique<NodeGrid::Grid>(this, nodeConfig->totalRows, nodeConfig->totalCols, nodeConfig->margin, nodeConfig->spacing);
            }
            else {
                body = std::make_unique<NodeGrid::Grid>(this);
            }
        }
        ~Node() = default;

        std::unique_ptr<NodeGrid::Grid> body;

        const muuid::uuid& id() const { return m_id; }
        const muuid::uuid& coreId() const { return m_core_id; }

        bool isUpdateNeeded() const { return m_is_drty; }
        void setUpdate(bool value) { m_is_drty = value; }

        bool isNew() const { return m_is_new; }
        void setIsNew(bool value) { m_is_new = value; }

        QList<WVCellDetails::CellItem::CellItem*> cells() const {
            QList<WVCellDetails::CellItem::CellItem*> cellList;

            for (auto* child : childItems()) {
                if (auto* cell = dynamic_cast<WVCellDetails::CellItem::CellItem*>(child)) {
                    cellList.append(cell);
                }
                for (QGraphicsItem* grandChild : child->childItems()) {
                    if (auto* cell = dynamic_cast<WVCellDetails::CellItem::CellItem*>(grandChild)) {
                        cellList.append(cell);
                    }
                }
            }
            return cellList;
        }
        QList<VWPin::PinItem*> pins() const {
            QList<VWPin::PinItem*> pinList;

            for (auto* child : childItems()) {
                if (auto* pin = dynamic_cast<VWPin::PinItem*>(child)) {
                    pinList.append(pin);
                }
                for (QGraphicsItem* grandChild : child->childItems()) {
                    if (auto* pin = dynamic_cast<VWPin::PinItem*>(grandChild)) {
                        pinList.append(pin);
                    }
                }
            }
            return pinList;
        }

        QRectF boundingRect() const override {
            constexpr qreal margin = 4.0;
            return rect().adjusted(-margin, -margin, margin, margin);
        }
        QPainterPath shape() const override {
            QPainterPath path;
            path.addRect(rect());
            return path;
        }

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            m_is_moving = false;
            QGraphicsRectItem::mousePressEvent(event);
        }
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
            QGraphicsRectItem::mouseReleaseEvent(event);
            m_is_moving = false;
        }

        QVariant itemChange(GraphicsItemChange change, const QVariant& value) override {
            if (change == ItemPositionChange && scene()) {
                if (flags() & QGraphicsItem::ItemIsMovable) {
                    if (!m_is_moving) {
                        m_is_moving = true;
                        m_is_drty = true;
                    }
                }
            }
            return QGraphicsRectItem::itemChange(change, value);
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
            Q_UNUSED(widget);
            painter->setRenderHint(QPainter::Antialiasing);

            if (option->state & QStyle::State_Selected) {
                painter->setPen(p_selectedPen);
            }
            else {
                painter->setPen(p_borderPen);
            }

            painter->setBrush(p_bgBrush);
            painter->drawRoundedRect(rect(), p_cornerRadius, p_cornerRadius);
        }
    };
}