#pragma once

#include <QApplication>
#include <QGraphicsSvgItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QSvgRenderer>
#include <QGraphicsSceneMouseEvent>
#include <QDrag>
#include <QWidget>

#include <QString>

class PinItem : public QGraphicsSvgItem {
    unsigned int i = 0;
    QPointF dragStartPosition;
public:
    PinItem(QGraphicsItem* parent) : QGraphicsSvgItem(parent) {
        setSharedRenderer(new QSvgRenderer(QStringLiteral(":/icons/outline/point.svg"), this));
        setAcceptDrops(true);
        setAcceptedMouseButtons(Qt::LeftButton);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        event->accept();
        dragStartPosition = event->screenPos();
        qDebug() << "Pin grabbed! Parent node is locked.";
    }
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        event->accept();

        // Calculate how far the mouse moved
        qreal distance = (event->screenPos() - dragStartPosition).manhattanLength();

        // Only start a real drag if the user moved the mouse enough (prevents accidental jitters)
        if (distance < QApplication::startDragDistance()) {
            return;
        }

        qDebug() << "Starting official QDrag operation!";

        // 1. Create the Drag object (Pass the viewport widget as parent)
        QDrag* drag = new QDrag(event->widget());

        // 2. Create the data payload (The MIME handshake)
        QMimeData* mimeData = new QMimeData();
        mimeData->setData("application/x-node-connection", "wire_data");
        drag->setMimeData(mimeData);

        // 3. Freeze regular mouse events and enter the official Drag & Drop loop!
        drag->exec(Qt::CopyAction | Qt::MoveAction);
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        event->accept();
        qDebug() << "Pin released.";
    }

    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override {
        qDebug() << "dragEnterEvent";
        event->acceptProposedAction();
    }
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override {
        qDebug() << "[" << ++i << "] dragMoveEvent";
        event->acceptProposedAction();
    }
    void dropEvent(QGraphicsSceneDragDropEvent* event) override {
        qDebug() << "A wire was successfully dropped onto this specific pin!" << " Mime data: " << event->mimeData()->data("application/x-node-connection");
        event->acceptProposedAction();
    }
};