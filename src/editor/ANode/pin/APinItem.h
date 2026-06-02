#pragma once


#include "APinData.h"
#include "APinDrag.h"

#include <QApplication>
#include <QGraphicsSvgItem>
#include <QGraphicsSceneDragDropEvent>
#include <QSvgRenderer>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

class PinItem : public QGraphicsSvgItem {

    QPointF dragStartPosition;
    APinData::PinData pinData;

    bool hasDraggingStarted(QGraphicsSceneMouseEvent* event) {
        return (event->screenPos() - dragStartPosition).manhattanLength() >= QApplication::startDragDistance();
    }

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
    }
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        event->accept();

        if (hasDraggingStarted(event)) {
            APinDrag::Drag::useDrag(event, pinData, mapToScene(boundingRect().center()));
        }
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        event->accept();
    }

    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override {
        event->acceptProposedAction();
    }
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override {
        event->acceptProposedAction();
    }
    void dropEvent(QGraphicsSceneDragDropEvent* event) override {
        APinDrag::Drag::finishDrag(event);
        event->acceptProposedAction();
    }
};