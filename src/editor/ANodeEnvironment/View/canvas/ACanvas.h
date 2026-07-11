#pragma once

#include "../pin/APinRegistry.h"
#include "../node/ANode.h"
#include "../scene/ANodeScene.h"

namespace ACanvas {

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
                { "In",  APinRegistry::FlowValue{ 0.0 } },
                { "Out", APinRegistry::FlowValue{ 180.0 } }
                });
            APinRegistry::Type::load("Standard", {
                { "Exe",  {} },
                { "Bool", { 1 } },
                { "Int",  { 32 } }
                });
            APinRegistry::Style::load("Standard", {
                { "Exe",  {} },
                { "Bool", { Qt::red } },
                { "Int",  { Qt::cyan } }
                });

            scene = new ANodeScene::NodeScene();
            view = new QGraphicsView(scene);

            scene->setSceneRect(-10000, -10000, 20000, 20000);

            view->setRenderHint(QPainter::Antialiasing);
            view->setAcceptDrops(true);
            view->setDragMode(QGraphicsView::RubberBandDrag);

            scene->addItem(new ANode::ANode());
            scene->addItem(new ANode::ANode());

            view->show();
        }
        QGraphicsView* getView() {
            return view;
        }
    };
}