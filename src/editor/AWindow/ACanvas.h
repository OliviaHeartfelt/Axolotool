#pragma once

#include "../ANode/ANode.h"
#include "../ANode/scene/ANodeScene.h"
#include "../ANode/pin/APinDataRegistry.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

#include <QColor>

class ANodeItem : public QGraphicsRectItem {
public:
    ANodeItem() {
        setRect(0, 0, 80, 40);
        setBrush(QBrush(Qt::blue));
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    }
};

class Canvas {
    QGraphicsScene* scene = nullptr;
    QGraphicsView* view = nullptr;

public:
    Canvas() {
        //test:
        APinRegistry::Flow::load("Standard", {
            { "Standard", "In",  APinRegistry::FlowValue{ 0.0 } },
            { "Standard", "Out", APinRegistry::FlowValue{ 180.0 } }
        });
        APinRegistry::Type::load("Standard", {
            { "Standard", "Exe",  {} },
            { "Standard", "Bool", { 1 } },
            { "Standard", "Int",  { 32 } }
        });
        APinRegistry::Style::load("Standard", {
            { "Standard", "Exe",  {} },
            { "Standard", "Bool", { Qt::red } },
            { "Standard", "Int",  { Qt::cyan } }
        });

        scene = new ANodeScene();
        view = new QGraphicsView(scene);

        scene->setSceneRect(-10000, -10000, 20000, 20000);

        view->setRenderHint(QPainter::Antialiasing);
        view->setAcceptDrops(true);
        view->setDragMode(QGraphicsView::RubberBandDrag);

        scene->addItem(new ANode());
        scene->addItem(new ANode());   

        view->show();
    }
    QGraphicsView* getView() {
        return view;
    }
};