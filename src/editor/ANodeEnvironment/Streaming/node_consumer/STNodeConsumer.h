#pragma once

#include "../../View/AView.h"
#include "../../Registry/ARegistry.h"
#include "../details/BoundedQueue.h"
#include "../node_streamer/STNodeStreamer.h"

namespace STNodeConsumer {

    class STNodeConsumer : public QObject {
        Q_OBJECT

        QGraphicsScene* m_scene = nullptr;
        ARegistry::Registry* m_registry = nullptr;
        STStreamerDetails::BoundedQueue::BoundedQueue<STNodeStreamer::Config::NodePayload> m_queue{ 50 };
        QTimer frameTimer;

        static constexpr int64_t MAX_FRAME_BUDGET_MS = 3;

    public:
        explicit STNodeConsumer(QGraphicsScene* scene, ARegistry::Registry* registry, QObject* parent = nullptr) : QObject(parent), m_scene(scene), m_registry(registry) {
            connect(&frameTimer, &QTimer::timeout, this, &STNodeConsumer::processQueue);
        }

        ~STNodeConsumer() override {
            cancel();
        }

        STStreamerDetails::BoundedQueue::BoundedQueue<STNodeStreamer::Config::NodePayload>& getQueue() {
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
            if (!m_scene || !m_registry) return;

            QElapsedTimer timer;
            timer.start();

            STNodeStreamer::Config::NodePayload payload;

            while (m_queue.tryPop(payload)) {

                auto* node = AView::Node::createNode(*m_registry, nullptr, AView::Context::Node::FactoryData{
                    .node = AView::Context::Node::NodeFactoryData::fromNodeRecord(payload.node),
                    .nodeCells = payload.nodeCells,
                    .widgets = payload.widgets
                    });

                if (node) {
                    m_scene->addItem(node);
                }

                if (timer.elapsed() >= MAX_FRAME_BUDGET_MS) {
                    return;
                }
            }

            if (m_queue.isCompleted()) {
                frameTimer.stop();
                emit nodesLoadingFinished();
            }
        }
    };
}