#pragma once

#include "../scene/VWNodeScene.h"
#include "details/CanvasView.h"

namespace VWCanvas {

    class VWCanvas : public QWidget {
        using CanvasView = VWCanvasDetails::CanvasView::CanvasView;
        using NodeScene = VWNodeScene::NodeScene;

        NodeScene* scene;
        CanvasView* view;
        QLabel* zoomLabel;

    public:
        explicit VWCanvas(QWidget* parent = nullptr) : QWidget(parent) {
            scene = new NodeScene(this);
            view =  new CanvasView(scene, this);

            scene->setSceneRect(-10000, -10000, 20000, 20000);

            view->setRenderHint(QPainter::Antialiasing);
            view->setAcceptDrops(true);
            view->setDragMode(QGraphicsView::RubberBandDrag);

            zoomLabel = new QLabel("100%", this);
            zoomLabel->setStyleSheet("color: white; background: rgba(0,0,0,128);");
            zoomLabel->move(10, 10);

            auto* layout = new QVBoxLayout(this);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->addWidget(view);
            setLayout(layout);
        }

        void resizeEvent(QResizeEvent* event) override {
            QWidget::resizeEvent(event);
            zoomLabel->move(width() - zoomLabel->width() - 10, 10);
        }

        QGraphicsScene* graphicsScene() const { return scene; }
        CanvasView* graphicsView()  const { return view; }
    };
}