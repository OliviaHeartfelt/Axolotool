#pragma once

#include "../pin/APinRegistry.h"
#include "../node/ANode.h"
#include "../scene/ANodeScene.h"

#include "../../Storage/ANodeEnvDB.h"

#include "mockPluginData.h"

namespace ACanvas {

    class Canvas {
        std::unique_ptr<QGraphicsScene> scene;
        std::unique_ptr<QGraphicsView> view;

        std::unique_ptr<ANodeEnvDB::ANodeEnvDB> nodeEnvDB;

    public:
        Canvas() {

            scene = std::make_unique<ANodeScene::NodeScene>();
            view = std::make_unique<QGraphicsView>(scene.get());

            nodeEnvDB = std::make_unique<ANodeEnvDB::ANodeEnvDB>("test_node_env.db", "NodeEnvPool");

            scene->setSceneRect(-10000, -10000, 20000, 20000);

            view->setRenderHint(QPainter::Antialiasing);
            view->setAcceptDrops(true);
            view->setDragMode(QGraphicsView::RubberBandDrag);

            if (nodeEnvDB) {

                if (nodeEnvDB->open()) {
                    MockData::injectMockPluginData(nodeEnvDB.get());
                    MockData::createNodeInstance(nodeEnvDB.get(), QPointF{ 0.0, 0.0 });
                    MockData::createNodeInstance(nodeEnvDB.get(), QPointF{ 0.0, 10.0 });
                }
                else {
                    qCritical() << "Could not open Node Environment Database!";
                }
            }

            view->show();
        }
        QGraphicsView* getView() {
            return view.get();
        }
    };
}