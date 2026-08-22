#pragma once

#include "../../View/AView.h"
#include "../../Registry/ARegistry.h"
#include "../../Storage/ANodeEnvDB.h"
#include "../details/BoundedQueue.h"
#include "../loading_wire_streamer/STLoadingWireStreamer.h"

namespace STLoadingWireConsumer {

    class STLoadingWireConsumer : public QObject {
        Q_OBJECT

            QGraphicsScene* m_scene = nullptr;
        ARegistry::Registry* m_registry = nullptr;
        ANodeEnvDB::ANodeEnvDB* m_nodeEnvDB = nullptr;
        STStreamerDetails::BoundedQueue::BoundedQueue<STLoadingWireStreamer::Config::WirePayload> m_queue{ 50 };
        QTimer frameTimer;

        static constexpr int64_t MAX_FRAME_BUDGET_MS = 3;

    public:
        explicit STLoadingWireConsumer(QGraphicsScene* scene, ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry* registry, QObject* parent = nullptr)
            : QObject(parent), m_scene(scene), m_nodeEnvDB(nodeEnvDB), m_registry(registry)
        {
            connect(&frameTimer, &QTimer::timeout, this, &STLoadingWireConsumer::processQueue);
        }

        ~STLoadingWireConsumer() override {
            cancel();
        }

        STStreamerDetails::BoundedQueue::BoundedQueue<STLoadingWireStreamer::Config::WirePayload>& getQueue() {
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

            STLoadingWireStreamer::Config::WirePayload payload;

            while (m_queue.tryPop(payload)) {

                AView::Wire::WireItem* wire = AView::Wire::createPermanentWire<AView::Pin::PinItem, AView::Cell::CellItem::CellItem>(
                    payload.wire.originId,
                    payload.wire.targetId,
                    m_registry,
                    m_nodeEnvDB,
                    payload.wire.id
                );

                if (wire) {
                    m_registry->wireView.wireViewRegistry.addVisible(wire->id(), wire);
                    wire->setIsNew(false);

                    qDebug() << "> Wire created! #Wires:" << m_registry->wireView.wireViewRegistry.sizeVisible() - 1 << "->" << m_registry->wireView.wireViewRegistry.sizeVisible();
                }

                if (timer.elapsed() >= MAX_FRAME_BUDGET_MS) return;
            }

            if (m_queue.isCompleted()) {
                frameTimer.stop();
                emit wiresLoadingFinished();
            }
        }
    };
}