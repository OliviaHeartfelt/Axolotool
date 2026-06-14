module;

#include <QByteArray>
#include <QDataStream>
#include <QMimeData>
#include <QIODevice>
#include <QWidget>
#include <QString>
#include <QGraphicsPathItem>
#include <QGraphicsSceneDragDropEvent>
#include <QtMath>

export module AWireTemp;

import AWireData;

export namespace AWireTemp {

    class WireTemp {

    public:
        static const QString mimeType() {
            return "application/x-anode-wire-data";
        }

        static void setMimeData(QMimeData* mimeData, AWireData::Data& wireData) {
            QByteArray posBlock;
            QDataStream posOut(&posBlock, QIODevice::WriteOnly);
            posOut.setVersion(QDataStream::Qt_6_11);

            posOut << wireData;
            mimeData->setData(mimeType(), posBlock);
        }
        static void setMimeData(QMimeData* mimeData, QPointF& pos) {
            QByteArray posBlock;
            QDataStream posOut(&posBlock, QIODevice::WriteOnly);
            posOut.setVersion(QDataStream::Qt_6_11);

            AWireData::Data wireData{ pos };
            posOut << wireData;
            mimeData->setData(mimeType(), posBlock);
        }

        static void draw(QGraphicsSceneDragDropEvent* event, AWireData::Data& wireData, QGraphicsPathItem* wire, const bool hasTarget) {
            QPointF currentMousePos = event->scenePos();
            QPainterPath path;

            path.moveTo(wireData.pos);

            qreal distance = qHypot(currentMousePos.x() - wireData.pos.x(), currentMousePos.y() - wireData.pos.y());
            qreal tension = qMax(distance * 0.35, 40.0);

            qreal radOrigin = qDegreesToRadians(wireData.dgrOrigin);
            qreal radTarget = qDegreesToRadians(wireData.dgrTargetDefault);

            // 3. Project control points along their exact directional vectors
            QPointF ctrlPoint1(
                wireData.pos.x() + qCos(radOrigin) * tension,
                wireData.pos.y() + qSin(radOrigin) * tension);
            QPointF ctrlPoint2(
                currentMousePos.x() + qCos(radTarget) * tension,
                currentMousePos.y() + qSin(radTarget) * tension);

            // 4. Construct the cubic spline instead of a line
            path.cubicTo(ctrlPoint1, ctrlPoint2, currentMousePos);

            wire->setPath(path);
        }
    };
}