#pragma once

#include "../wire/AWire.h"

namespace ANodeScene {

    class NodeScene : public QGraphicsScene {
        QGraphicsPathItem* temporaryWire = nullptr;
        AWire::WireData wireData;
        bool runtimeHasTarget = false;
    
        void setWireStartPos(QGraphicsSceneDragDropEvent* event) {
            QByteArray posBlock = event->mimeData()->data(AWire::WireTemp::mimeType());
            QDataStream posIn(&posBlock, QIODevice::ReadOnly);
            posIn.setVersion(QDataStream::Qt_6_11);
    
            AWire::WireData data;
            posIn >> data;
            wireData = data;
        }
        void addTempWire() {
            if (!temporaryWire) {
                temporaryWire = new QGraphicsPathItem();
                temporaryWire->setPen(QPen(Qt::lightGray, 2, Qt::DashLine));
                addItem(temporaryWire);
            }
        }
        void cleanUpTemporaryWire() {
            if (temporaryWire) {
                removeItem(temporaryWire);
                delete temporaryWire;
                temporaryWire = nullptr;
            }
        }
    
    protected:
        void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override {
            if (event->mimeData()->hasFormat(AWire::WireTemp::mimeType())) {
                event->acceptProposedAction();
                setWireStartPos(event);
                addTempWire();
            }
            else {
                QGraphicsScene::dragEnterEvent(event);
            }
        }
    
        void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override {
            if (temporaryWire) {
                event->acceptProposedAction();
                AWire::WireTemp::draw(event, wireData, temporaryWire, runtimeHasTarget);
            }
    
            QGraphicsScene::dragMoveEvent(event);
        }
    
        void dropEvent(QGraphicsSceneDragDropEvent* event) override {
            cleanUpTemporaryWire();
            QGraphicsScene::dropEvent(event);
        }
    
        void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override {
            cleanUpTemporaryWire();
            QGraphicsScene::dragLeaveEvent(event);
        }
    };
}