#pragma once

#include "../../View/AView.h"
#include "../../Registry/ARegistry.h"
#include "../../Storage/ANodeEnvDB.h"
#include "../details/BoundedQueue.h"
#include "../wire_streamer/STWireStreamer.h"

namespace STWireConsumer {

    class STWireConsumer : public QObject {
        Q_OBJECT

            QGraphicsScene* m_scene = nullptr;
        ARegistry::Registry* m_registry = nullptr;
        ANodeEnvDB::ANodeEnvDB* m_nodeEnvDB = nullptr;
        STStreamerDetails::BoundedQueue::BoundedQueue<STWireStreamer::Config::WirePayload> m_queue{ 50 };
        QTimer frameTimer;

        static constexpr int64_t MAX_FRAME_BUDGET_MS = 3;

    public:
        explicit STWireConsumer(QGraphicsScene* scene, ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry* registry, QObject* parent = nullptr)
            : QObject(parent), m_scene(scene), m_nodeEnvDB(nodeEnvDB), m_registry(registry)
        {
            connect(&frameTimer, &QTimer::timeout, this, &STWireConsumer::processQueue);
        }

        ~STWireConsumer() override {
            cancel();
        }

        STStreamerDetails::BoundedQueue::BoundedQueue<STWireStreamer::Config::WirePayload>& getQueue() {
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
        void wiresLoadingFinished();

    private slots:

        void processQueue() {
            if (!m_scene || !m_registry || !m_nodeEnvDB) return;

            QElapsedTimer timer;
            timer.start();

            STWireStreamer::Config::WirePayload payload;

            while (m_queue.tryPop(payload)) {

                AView::Wire::WireItem* wire = AView::Wire::createPermanentWire<AView::Pin::PinItem>(
                    payload.wire.originId,
                    payload.wire.targetId,
                    m_registry,
                    m_nodeEnvDB
                );

                if (wire) {
                    m_scene->addItem(wire);
                }

                if (timer.elapsed() >= MAX_FRAME_BUDGET_MS) {
                    return;
                }
            }

            if (m_queue.isCompleted()) {
                frameTimer.stop();
                emit wiresLoadingFinished();
            }
        }
    };
}