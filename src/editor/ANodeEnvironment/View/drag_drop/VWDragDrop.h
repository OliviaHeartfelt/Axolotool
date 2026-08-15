#pragma once

#include "../../Registry/ARegistry.h"
#include "../wire/VWWire.h"
#include "../pin/details/Concepts.h"
#include "details/Context.h"

namespace VWDragDrop {

    namespace Context {
        using namespace VWDragDropDetails::Context;
    }

    template<Context::ItemData T>
    inline void setDragData(QMimeData* mimeData, const QString& mimeType, const T& itemData) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);

        out.setVersion(QDataStream::Qt_6_11);
        out << itemData;
        mimeData->setData(mimeType, block);
    }

    template<Context::ItemData T, VWPinDetails::Concepts::PinItemConcept PinItem>
    inline void useDrag(QGraphicsSceneMouseEvent* event, ARegistry::Registry& registry, PinItem* sourcePin, const QString& mimeType, const T& itemData, const char* mimePosPropertyStr, QPointF scenePos) {
        if (!event || !event->widget()) return;

        QDrag* drag = new QDrag(event->widget());
        if (!drag) return;

        QMimeData* mimeData = new QMimeData();
        if (!mimeData) return;

        setDragData(mimeData, mimeType, itemData);

        auto flowOpt = registry.node.pinFlowRegistry.at(sourcePin->pinData().flow());

        VWWire::TemporaryWire::WireTemp::setMimeData(mimeData, VWWire::TemporaryWire::WireTempData{
            .pos = scenePos,
            .dgrOrigin = flowOpt ? flowOpt->degree : 0,
            .dgrTargetDefault = flowOpt ? flowOpt->degree + 180 : 180
            });

        if (sourcePin && mimePosPropertyStr) {
            mimeData->setProperty(mimePosPropertyStr, reinterpret_cast<quintptr>(sourcePin));
        }

        drag->setMimeData(mimeData);
        drag->exec(Qt::CopyAction | Qt::MoveAction);
    }

    template<Context::ItemData T>
    inline bool finishDrag(QGraphicsSceneDragDropEvent* event, const QString& mimeType, T& itemData) {
        if (!event || !event->mimeData() || !event->mimeData()->hasFormat(mimeType)) {
            return false;
        }

        QByteArray block = event->mimeData()->data(mimeType);
        QDataStream in(&block, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_6_11);

        in >> itemData;

        qDebug() << "Successfully unpacked MIME drop data for type:" << mimeType;
        return true;
    }
}