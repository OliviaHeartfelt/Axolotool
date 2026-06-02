#pragma once

#include "AW"

#include <QGraphicsPathItem>
#include <QPen>
#include <QPainterPath>
#include <QtMath>

class AWireItem : public QGraphicsPathItem {
private:
    QGraphicsItem* m_sourcePin = nullptr;
    QGraphicsItem* m_targetPin = nullptr;
    qreal m_dgrOrigin = 0;
    qreal m_dgrTarget = 180;

public:
    AWireItem(QGraphicsItem* sourcePin, QGraphicsItem* targetPin, qreal dgrOrigin, qreal dgrTarget)
        : m_sourcePin(sourcePin), m_targetPin(targetPin), m_dgrOrigin(dgrOrigin), m_dgrTarget(dgrTarget)
    {
        setPen(QPen(Qt::lightGray, 2, Qt::SolidLine));
        setZValue(-1);

        updatePath();
    }

    void updatePath() {
        if (!m_sourcePin || !m_targetPin) return;

        // Automatically sample the live, absolute center coordinates of both pins
        QPointF startPos = m_sourcePin->mapToScene(m_sourcePin->boundingRect().center());
        QPointF endPos = m_targetPin->mapToScene(m_targetPin->boundingRect().center());

        // Run your exact omnidirectional Bezier math
        qreal distance = qHypot(endPos.x() - startPos.x(), endPos.y() - startPos.y());
        qreal tension = qMax(distance * 0.35, 40.0);

        qreal radOrigin = qDegreesToRadians(m_dgrOrigin);
        qreal radTarget = qDegreesToRadians(m_dgrTarget);

        QPointF ctrlPoint1(startPos.x() + qCos(radOrigin) * tension, startPos.y() + qSin(radOrigin) * tension);
        QPointF ctrlPoint2(endPos.x() + qCos(radTarget) * tension, endPos.y() + qSin(radTarget) * tension);

        QPainterPath path;
        path.moveTo(startPos);
        path.cubicTo(ctrlPoint1, ctrlPoint2, endPos);
        setPath(path);
    }
};