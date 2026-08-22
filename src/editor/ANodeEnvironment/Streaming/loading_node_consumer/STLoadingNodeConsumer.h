#pragma once

#include "../../View/AView.h"
#include "../../Registry/ARegistry.h"
#include "../../Storage/ANodeEnvDB.h"
#include "../details/BoundedQueue.h"
#include "../loading_node_streamer/STLoadingNodeStreamer.h"

namespace STLoadingNodeConsumer {

    class STLoadingNodeConsumer : public QObject {
        Q_OBJECT

            ANodeEnvDB::ANodeEnvDB* m_node_env_db = nullptr;
        QGraphicsScene* m_scene = nullptr;
        ARegistry::Registry* m_registry = nullptr;
        STStreamerDetails::BoundedQueue::BoundedQueue<STLoadingNodeStreamer::Config::NodePayload> m_queue{ 50 };
        QTimer frameTimer;

        static constexpr int64_t MAX_FRAME_BUDGET_MS = 3;

    public:
        explicit STLoadingNodeConsumer(QGraphicsScene* scene, ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry* registry, QObject* parent = nullptr) 
            : QObject(parent), m_scene(scene), m_node_env_db(nodeEnvDB), m_registry(registry) 
        {
            connect(&frameTimer, &QTimer::timeout, this, &STLoadingNodeConsumer::processQueue);
        }

        ~STLoadingNodeConsumer() override {
            cancel();
        }

        STStreamerDetails::BoundedQueue::BoundedQueue<STLoadingNodeStreamer::Config::NodePayload>& getQueue() {
            return m_queue;
        }

        void startLoading() {
            cancel();
            m_queue.reset();
            frameTimer.start(16);
        }

        void cancel() {
            m_queue.cancel();
            frameTimer.stop();
        }

    signals:
        void nodesLoadingFinished();

    private slots:
        void processQueue() {
            if (!m_scene || !m_node_env_db || !m_registry) return;
            QElapsedTimer timer;
            timer.start();

            STLoadingNodeStreamer::Config::NodePayload payload;

            while (m_queue.tryPop(payload)) {

                auto* node = AView::Node::CreateNode::createNode(m_node_env_db, *m_registry, nullptr, AView::Context::Node::FactoryData{
                    .node = AView::Context::Node::NodeFactoryData::fromNodeRecord(payload.node),
                    .nodeCells = payload.nodeCells,
                    .widgets = payload.widgets
                    });

                if (node) {
                    m_registry->nodeView.nodeViewRegistry.addVisible(node->id(), node);
                    m_scene->addItem(node);

                    qDebug() << "> Node created! #Nodes:" << m_registry->nodeView.nodeViewRegistry.sizeVisible() - 1 << "->" << m_registry->nodeView.nodeViewRegistry.sizeVisible();
                }

                if (timer.elapsed() >= MAX_FRAME_BUDGET_MS) return;
            }

            if (m_queue.isCompleted()) {
                frameTimer.stop();
                emit nodesLoadingFinished();
            }
        }
    };
}