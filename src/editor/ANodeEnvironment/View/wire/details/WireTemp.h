#pragma once

namespace VWWireDetails::WireTemp {

    struct WireTempData {
        QPointF pos{};
        qreal dgrOrigin = 0;
        qreal dgrTargetDefault = 180;

        friend inline QDataStream& operator<<(QDataStream& out, const WireTempData& data) { return out << data.pos << data.dgrOrigin << data.dgrTargetDefault; }
        friend inline QDataStream& operator>>(QDataStream& in, WireTempData& data) {        return in >>  data.pos >> data.dgrOrigin >> data.dgrTargetDefault; }
    };

    class WireTemp {
        inline static std::optional<QPointF> stuckPos = std::nullopt;

    public:
        static const QString mimeType() {
            return "application/x-anode-wire-data";
        }

        static void setMimeData(QMimeData* mimeData, WireTempData&& wireData) {
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

            WireTempData wireData{ pos };
            posOut << wireData;
            mimeData->setData(mimeType(), posBlock);
        }

        static void stuck(const QPointF pos) {
            stuckPos = pos;
        }
        static void unstuck() {
            stuckPos = std::nullopt;
        }

        static void draw(QGraphicsSceneDragDropEvent* event, WireTempData& wireData, QGraphicsPathItem* wire) {
            QPointF targetPos = stuckPos ? *stuckPos : event->scenePos();

            QPainterPath path;

            path.moveTo(wireData.pos);

            qreal distance = qHypot(targetPos.x() - wireData.pos.x(), targetPos.y() - wireData.pos.y());
            qreal tension = qMax(distance * 0.35, 40.0);

            qreal radOrigin = qDegreesToRadians(wireData.dgrOrigin);
            qreal radTarget = qDegreesToRadians(wireData.dgrTargetDefault);

            QPointF ctrlPoint1(
                wireData.pos.x() + qCos(radOrigin) * tension,
                wireData.pos.y() + qSin(radOrigin) * tension);
            QPointF ctrlPoint2(
                targetPos.x() + qCos(radTarget) * tension,
                targetPos.y() + qSin(radTarget) * tension);

            path.cubicTo(ctrlPoint1, ctrlPoint2, targetPos);

            wire->setPath(path);
        }
    };
}