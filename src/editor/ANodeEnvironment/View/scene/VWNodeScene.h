#pragma once

#include "../wire/VWWire.h"

namespace VWNodeScene {

    class NodeScene : public QGraphicsScene {
        QGraphicsPathItem* temporaryWire = nullptr;
        VWWire::TemporaryWire::WireTempData wireData;
        bool runtimeHasTarget = false;
    
        void setWireStartPos(QGraphicsSceneDragDropEvent* event) {
            QByteArray posBlock = event->mimeData()->data(VWWire::TemporaryWire::WireTemp::mimeType());
            QDataStream posIn(&posBlock, QIODevice::ReadOnly);
            posIn.setVersion(QDataStream::Qt_6_11);
    
            VWWire::TemporaryWire::WireTempData data;
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

    public:
        NodeScene(QWidget* parent = nullptr) : QGraphicsScene(parent) {}
    
    protected:
        void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override {
            if (event->mimeData()->hasFormat(VWWire::TemporaryWire::WireTemp::mimeType())) {
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
                VWWire::TemporaryWire::WireTemp::WireTemp::draw(event, wireData, temporaryWire);
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