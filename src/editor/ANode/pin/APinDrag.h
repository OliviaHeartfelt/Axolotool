#pragma once

#include "APinData.h"
#include "../wire/AWire.h"

#include <QDataStream>
#include <QDebug>
#include <QString>
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QDrag>
#include <QIODevice>
#include <QWidget>

namespace APinDrag {
    class Drag {
        static void setDragData(QMimeData* mimeData, APinData::PinData& pinData) {
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);

            out.setVersion(QDataStream::Qt_6_11);
            out << pinData;
            mimeData->setData(mimeType(), block);
        }

    public:
        static const QString mimeType() {
            return "application/x-anode-data";
        }

        static void useDrag(QGraphicsSceneMouseEvent* event, APinData::PinData& pinData, QPointF&& scenePos) {
            qDebug() << "drag started!";

            QDrag* drag = new QDrag(event->widget());
            QMimeData* mimeData = new QMimeData();

            setDragData(mimeData, pinData);
            AWire::setMimeData(mimeData, scenePos);
            drag->setMimeData(mimeData);

            drag->exec(Qt::CopyAction | Qt::MoveAction);
        }
        static void finishDrag(QGraphicsSceneDragDropEvent* event) {
            if (!event->mimeData()->hasFormat(mimeType())) return;

            QByteArray block = event->mimeData()->data(mimeType());
            QDataStream in(&block, QIODevice::ReadOnly);
            in.setVersion(QDataStream::Qt_6_11);

            APinData::PinData data;
            in >> data;

            qDebug() << "A wire was successfully dropped onto this specific pin!" << " Mime data:";
            data.debug();
        }
    };
}