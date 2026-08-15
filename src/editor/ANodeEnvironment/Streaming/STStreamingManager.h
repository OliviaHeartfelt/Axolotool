#pragma once

#include "../Storage/ANodeEnvDB.h"
#include "node_consumer/STNodeConsumer.h"
#include "node_streamer/STNodeStreamer.h"
#include "wire_consumer/STWireConsumer.h"
#include "wire_streamer/STWireStreamer.h"

namespace STStreamingManager {

    namespace NodeConsumer { using namespace STNodeConsumer; }
    namespace NodeStreamer { using namespace STNodeStreamer; }

    namespace WireConsumer { using namespace STWireConsumer; }
    namespace WireStreamer { using namespace STWireStreamer; }

    class StreamingManager : public QObject {
        Q_OBJECT

            ANodeEnvDB::ANodeEnvDB* m_db = nullptr;

        STNodeConsumer::STNodeConsumer* m_nodeConsumer = nullptr;
        STNodeStreamer::STNodeStreamer* m_nodeStreamer = nullptr;

        STWireConsumer::STWireConsumer* m_wireConsumer = nullptr;
        STWireStreamer::STWireStreamer* m_wireStreamer = nullptr;

        QFuture<void> m_currentStreamerFuture;
        muuid::uuid m_activeChunkId;

    public:
        explicit StreamingManager(
            ANodeEnvDB::ANodeEnvDB* db,
            STNodeConsumer::STNodeConsumer* nodeConsumer,
            STNodeStreamer::STNodeStreamer* nodeStreamer,
            STWireConsumer::STWireConsumer* wireConsumer,
            STWireStreamer::STWireStreamer* wireStreamer,
            QObject* parent = nullptr
        ) :
            QObject(parent),
            m_db(db),
            m_nodeConsumer(nodeConsumer),
            m_nodeStreamer(nodeStreamer),
            m_wireConsumer(wireConsumer),
            m_wireStreamer(wireStreamer)
        {
            connect(m_nodeConsumer, &STNodeConsumer::STNodeConsumer::nodesLoadingFinished,
                this, &StreamingManager::onNodesPhaseFinished);
        }

        ~StreamingManager() override {
            cancelCurrentLoad();
        }

        void loadChunk(const muuid::uuid& chunkId) {
            cancelCurrentLoad();

            if (!m_nodeConsumer || !m_nodeStreamer) return;
            m_activeChunkId = chunkId;

            m_nodeConsumer->startLoading();

            m_currentStreamerFuture = QtConcurrent::run([this, chunkId]() {
                m_nodeStreamer->streamChunkToQueue(chunkId, m_nodeConsumer->getQueue());

                m_nodeConsumer->getQueue().finish();
                });
        }

        void cancelCurrentLoad() {
            if (m_nodeConsumer) m_nodeConsumer->cancel();
            if (m_wireConsumer) m_wireConsumer->cancel();

            if (m_currentStreamerFuture.isRunning()) {
                m_currentStreamerFuture.waitForFinished();
            }
        }

    private slots:
        void onNodesPhaseFinished() {
            if (!m_wireConsumer || !m_wireStreamer) return;

            m_wireConsumer->startLoading();

            m_currentStreamerFuture = QtConcurrent::run([this]() {
                m_wireStreamer->streamChunkToQueue(m_activeChunkId, m_wireConsumer->getQueue());

                m_wireConsumer->getQueue().finish();
                });
        }
    };
}