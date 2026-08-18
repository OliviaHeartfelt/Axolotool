#pragma once

#include "NodeGrid.h"

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
        QPen   p_borderPen = QPen(QColor("#2980b9"), 1);
        QPen   p_selectedPen = QPen(QColor("#f1c40f"), 2, Qt::DashLine);
        QBrush p_bgBrush = QBrush(QColor("#3498db"));
        qreal  p_cornerRadius = 5.0;

        muuid::uuid m_id;
        muuid::uuid m_core_id;

        bool m_is_new = true;
        bool m_is_drty = false;
        bool m_is_moving = false;

    public:
        Node(QGraphicsItem* parent, const muuid::uuid& coreId, const std::optional<muuid::uuid>& id = std::nullopt, const std::optional<NodeGridConfig>& nodeConfig = std::nullopt)
            : QGraphicsRectItem(parent), m_core_id(coreId) {
            
            if (id) {
                m_id = *id;
                m_is_new = false;
            }
            else {
                m_id = muuid::uuid::generate_unix_time_based();
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
        ~Node() {
            if (!body) return;
            body->deleteGrid(false);
        }

        std::unique_ptr<NodeGrid::Grid> body;

        const muuid::uuid& id() const { return m_id; }
        const muuid::uuid& coreId() const { return m_core_id; }

        bool isUpdateNeeded() const { return m_is_drty; }
        void setUpdate(bool value) { m_is_drty = value; }

        bool isNew() const { return m_is_new; }
        void setIsNew(bool value) { m_is_new = value; }



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
    };
}