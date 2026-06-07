#pragma once

#include <QGraphicsPathItem>
#include <QPen>
#include <QPainterPath>
#include <QtMath>


class AWireItem : public QGraphicsPathItem {
private:
    QGraphicsItem* originPin = nullptr;
    QGraphicsItem* targetPin = nullptr;
    qreal dgrOrigin = 0;
    qreal dgrTarget = 180;

public:
    AWireItem(QGraphicsItem* sourcePin, QGraphicsItem* targetPin, qreal dgrOrigin, qreal dgrTarget)
        : originPin(sourcePin), targetPin(targetPin), dgrOrigin(dgrOrigin), dgrTarget(dgrTarget)
    {
        setPen(QPen(Qt::lightGray, 2, Qt::SolidLine));
        setZValue(-1);

        if (sourcePin && targetPin)
            updatePath();
    }

    void updatePath() {
        if (!originPin || !targetPin) return;

        QPointF startPos = originPin->mapToScene(originPin->boundingRect().center());
        QPointF endPos = targetPin->mapToScene(targetPin->boundingRect().center());

        qreal distance = qHypot(
            endPos.x() - startPos.x(), 
            endPos.y() - startPos.y());
        qreal tension = qMax(distance * 0.35, 40.0);

        qreal radOrigin = qDegreesToRadians(dgrOrigin);
        qreal radTarget = qDegreesToRadians(dgrTarget);

        QPointF controlPointOrigin(
            startPos.x() + qCos(radOrigin) * tension, 
            startPos.y() + qSin(radOrigin) * tension);
        QPointF controlPointTarget(
            endPos.x() + qCos(radTarget) * tension, 
            endPos.y() + qSin(radTarget) * tension);

        QPainterPath path;
        path.moveTo(startPos);
        path.cubicTo(controlPointOrigin, controlPointTarget, endPos);
        setPath(path);
    }
};