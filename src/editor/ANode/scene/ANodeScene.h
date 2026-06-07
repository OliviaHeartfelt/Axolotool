#pragma once

#include "../wire/AWireTemp.h"
#include "../wire/AWireData.h"

#include <QDataStream>
#include <QGraphicsScene>
#include <QIODevice>
#include <QGraphicsPathItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

class ANodeScene : public QGraphicsScene {
    QGraphicsPathItem* m_temporaryWire = nullptr;
    AWireData::WireData wireData;
    bool m_runtimeHasTarget = false;

    void setWireStartPos(QGraphicsSceneDragDropEvent* event) {
        QByteArray posBlock = event->mimeData()->data(AWireTemp::mimeType());
        QDataStream posIn(&posBlock, QIODevice::ReadOnly);
        posIn.setVersion(QDataStream::Qt_6_11);

        AWireData::WireData data;
        posIn >> data;
        wireData = data;
    }
    void addTempWire() {
        if (!m_temporaryWire) {
            m_temporaryWire = new QGraphicsPathItem();
            m_temporaryWire->setPen(QPen(Qt::lightGray, 2, Qt::DashLine));
            addItem(m_temporaryWire);
        }
    }
    void cleanUpTemporaryWire() {
        if (m_temporaryWire) {
            removeItem(m_temporaryWire);
            delete m_temporaryWire;
            m_temporaryWire = nullptr;
        }
    }

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override {
        if (event->mimeData()->hasFormat(AWireTemp::mimeType()) ) {
            event->acceptProposedAction();
            setWireStartPos(event);
            addTempWire();
        }
        else {
            QGraphicsScene::dragEnterEvent(event);
        }
    }

    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override {
        if (m_temporaryWire) {
            event->acceptProposedAction();
            AWireTemp::draw(event, wireData, m_temporaryWire, m_runtimeHasTarget);
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