#pragma once

#include "details/Config.h"
#include "details/NodeProcessing.h"
#include "../details/BoundedQueue.h"

#include "../../Storage/ANodeEnvDB.h"

namespace STNodeStreamer {

    namespace Config {
        using namespace STNodeStreamerDetails::Config;
    }

    class STNodeStreamer {
        ANodeEnvDB::ANodeEnvDB* m_nodeEnvDB = nullptr;
        ARegistry::Registry* m_registry = nullptr;

        std::generator<Config::NodePayload&&> streamChunkNodes(QSqlQuery& query) {
            if (!m_registry || !m_nodeEnvDB || !m_nodeEnvDB->isOpen()) co_return;

            auto nodesOpt = m_nodeEnvDB->node.getNodesInView(query, true);
            if (!nodesOpt || nodesOpt->isEmpty()) co_return;

            for (const auto& nodeRecord : *nodesOpt) {
                Config::NodePayload payload;

                if (!STNodeStreamerDetails::NodeProcessing::processNode(m_nodeEnvDB, *m_registry, query, payload, nodeRecord.id)) continue;

                co_yield std::move(payload);
            }
        }

    public:
        explicit STNodeStreamer(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry* registry) : m_nodeEnvDB(nodeEnvDB), m_registry(registry) {}

        bool streamChunkToQueue(const muuid::uuid& chunkId, STStreamerDetails::BoundedQueue::BoundedQueue<Config::NodePayload>& queue) {
            if (!m_nodeEnvDB) return false;

            return ANodeEnvDB::Helpers::useQuery(m_nodeEnvDB->getPool(), [&](QSqlQuery& query) {
                for (const Config::NodePayload& payload : streamChunkNodes(query)) {
                    if (!queue.pushBlocking(std::move(payload))) break;
                }
                return true;
            });
        }
    };
}