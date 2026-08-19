#pragma once

namespace VWWireDetails::WireItem {

    class WireItem : public QGraphicsPathItem {
        QGraphicsItem* m_originPin = nullptr;
        QGraphicsItem* m_targetPin = nullptr;
        qreal m_dgrOrigin = 0;
        qreal m_dgrTarget = 0;

        muuid::uuid m_id;
        std::optional<muuid::uuid> m_core_id;

        bool m_isNew = true;
        bool m_update = false;

    public:
        WireItem(QGraphicsItem* originPin, QGraphicsItem* targetPin, const qreal dgrOrigin, const qreal dgrTarget, const std::optional<muuid::uuid>& coreId)
            : m_originPin(originPin), m_targetPin(targetPin), m_dgrOrigin(dgrOrigin), m_dgrTarget(dgrTarget), m_core_id(coreId)
        {
            m_id = muuid::uuid::generate_unix_time_based();

            setBrush(Qt::NoBrush);

            if (originPin && targetPin) {
                updatePath();
            }
        }

        const muuid::uuid& id() const { return m_id; }

        const std::optional<muuid::uuid>& coreId() const { return m_core_id; }
        void coreId(const muuid::uuid& newCore) { m_core_id = newCore; }

        QGraphicsItem* origin() { return m_originPin; }
        QGraphicsItem* target() { return m_targetPin; }

        bool isUpdateNeeded() const { return m_update; }
        void setUpdate(bool value) { m_update = value; }

        bool isNew() const { return m_isNew; }
        void setIsNew(bool value) { m_isNew = value; }

        void updatePath() {
            if (!m_originPin || !m_targetPin) return;

            QPointF startPos = m_originPin->mapToScene(m_originPin->boundingRect().center());
            QPointF endPos =   m_targetPin->mapToScene(m_targetPin->boundingRect().center());

            qreal distance = qHypot(
                endPos.x() - startPos.x(),
                endPos.y() - startPos.y());
            qreal tension = qMax(distance * 0.35, 40.0);

            qreal radOrigin = qDegreesToRadians(m_dgrOrigin);
            qreal radTarget = qDegreesToRadians(m_dgrTarget);

            QPointF controlPointOrigin(
                startPos.x() + qCos(radOrigin) * tension,
                startPos.y() + qSin(radOrigin) * tension);
            QPointF controlPointTarget(
                endPos.x() + qCos(radTarget) * tension,
                endPos.y() + qSin(radTarget) * tension);

            QPainterPath path;
            path.moveTo(startPos);
            path.cubicTo(controlPointOrigin, controlPointTarget, endPos);

            prepareGeometryChange();
            setPath(path);
        }
    };
}