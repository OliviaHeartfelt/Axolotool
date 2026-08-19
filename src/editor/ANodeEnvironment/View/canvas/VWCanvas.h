#pragma once

#include "../../Registry/ARegistry.h"
#include "../scene/VWNodeScene.h"
#include "details/CanvasView.h"

namespace VWCanvas {

    class VWCanvas : public QWidget {
        using CanvasView = VWCanvasDetails::CanvasView::CanvasView;
        using NodeScene = VWNodeScene::NodeScene;

        ARegistry::Registry* m_registry = nullptr;
        NodeScene* scene = nullptr;
        CanvasView* view = nullptr;

        QLabel* zoomLabel = nullptr;
        QLabel* posLabel = nullptr;

        //const QPointF pos = view->mapToScene(viewport->rect().center());

    public:
        explicit VWCanvas(ARegistry::Registry* registry, QWidget* parent = nullptr) : QWidget(parent), m_registry(registry) {
            if (!m_registry) return;

            scene = new NodeScene(this);
            if (!scene) return;

            view =  new CanvasView(scene, m_registry, this);
            if (!view) return;

            scene->setSceneRect(-10000, -10000, 20000, 20000);

            view->setRenderHint(QPainter::Antialiasing);
            view->setAcceptDrops(true);
            view->setDragMode(QGraphicsView::RubberBandDrag);

            zoomLabel = new QLabel("100%", this);
            if (zoomLabel) {
                zoomLabel->setStyleSheet("color: white; background: rgba(0,0,0,128); padding: 4px;");

                zoomLabel->adjustSize();
                zoomLabel->move(width() - zoomLabel->width() - 10, 10);

                connect(view, &CanvasView::zoomChanged, this, [this](int percentage) {
                    zoomLabel->setText(QString::number(percentage) + "%");
                    zoomLabel->adjustSize();
                    zoomLabel->move(width() - zoomLabel->width() - 10, 10);
                });
            }

            posLabel = new QLabel("x: 0, y: 0", this);
            if (posLabel) {
                posLabel->setStyleSheet("color: white; background: rgba(0,0,0,128); padding: 4px;");

                posLabel->adjustSize();
                posLabel->move(width() - posLabel->width() - 10, 40);

                connect(view, &CanvasView::posChanged, this, [this]() {
                    if (!view->viewport()) return;

                    QPointF pos = view->mapToScene(view->viewport()->rect().center());
                    int cartesianX = qRound(pos.x());
                    int cartesianY = qRound(-pos.y());

                    posLabel->setText(QString("x: %1, y: %2").arg(cartesianX).arg(cartesianY));
                    posLabel->adjustSize();
                    posLabel->move(width() - posLabel->width() - 10, 40);
                    });
            }


            auto* layout = new QVBoxLayout(this);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->addWidget(view);
            setLayout(layout);
        }

        void resizeEvent(QResizeEvent* event) override {
            QWidget::resizeEvent(event);

            if (zoomLabel) {
                zoomLabel->move(width() - zoomLabel->width() - 10, 10);
            }
            if (posLabel) {
                posLabel->move(width() - posLabel->width() - 10, 40);
            }
        }

        QGraphicsScene* graphicsScene() const { return scene; }
        CanvasView* graphicsView()  const { return view; }
    };
}