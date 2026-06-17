module;

#include <QDataStream>
#include <QDebug>
#include <QString>
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QDrag>
#include <QIODevice>
#include <QWidget>
#include <QVariant>
#include <QGraphicsItem>
#include <QReadWriteLock>

export module APinDrag;

import AWire;
import APinData;

export namespace APinDrag {

    class Drag {
        static void setDragData(QMimeData* mimeData, APinData::PinData& pinData) {
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);

            out.setVersion(QDataStream::Qt_6_11);
            out << pinData;
            mimeData->setData(mimeType(), block);
        }


    public:
        static const QString mimeType() { return QStringLiteral("application/x-anode-data"); }
        struct mimeProperty {
            static const char* sourcePinItemPtr() { return "sourcePinItemPtr"; }
        };

        static void useDrag(QGraphicsSceneMouseEvent* event, QGraphicsItem* sourcePin, APinData::PinData& pinData, QPointF&& scenePos) {
            QDrag* drag = new QDrag(event->widget());
            QMimeData* mimeData = new QMimeData();

            setDragData(mimeData, pinData);
            AWire::WireTemp::setMimeData(mimeData, scenePos);
            mimeData->setProperty(mimeProperty::sourcePinItemPtr(), reinterpret_cast<quintptr>(sourcePin));
            drag->setMimeData(mimeData);

            drag->exec(Qt::CopyAction | Qt::MoveAction);
        }

        static APinData::PinData finishDrag(QGraphicsSceneDragDropEvent* event) {
            APinData::PinData data;
            if (!event->mimeData()->hasFormat(mimeType())) return data;

            QByteArray block = event->mimeData()->data(mimeType());
            QDataStream in(&block, QIODevice::ReadOnly);
            in.setVersion(QDataStream::Qt_6_11);
            
            in >> data;

            qDebug() << "Drop Mime data:";
            data.debug();
            return data;
        }
    };
}