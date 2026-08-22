#pragma once

#include "details/Config.h"
#include "details/NodeProcessing.h"
#include "../details/BoundedQueue.h"

#include "../../Storage/ANodeEnvDB.h"
#include "../../Registry/ARegistry.h"

namespace STLoadingNodeStreamer {

    namespace Config {
        using namespace STLoadingNodeStreamerDetails::Config;
    }

    class STLoadingNodeStreamer {
        ANodeEnvDB::ANodeEnvDB* m_nodeEnvDB = nullptr;
        ARegistry::Registry* m_registry = nullptr;

        std::generator<Config::NodePayload> streamChunkNodes(QSqlQuery& query) {
            if (!m_registry || !m_nodeEnvDB || !m_nodeEnvDB->isOpen()) co_return;

            auto nodesOpt = m_nodeEnvDB->node.getNodesInView(query, true);
            if (!nodesOpt || nodesOpt->isEmpty()) co_return;

            for (const auto& nodeRecord : *nodesOpt) {
                Config::NodePayload payload;

                if (!STLoadingNodeStreamerDetails::NodeProcessing::processNode(m_nodeEnvDB, *m_registry, query, payload, nodeRecord.id)) continue;
                co_yield std::move(payload);
            }
        }

    public:
        explicit STLoadingNodeStreamer(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry* registry) : m_nodeEnvDB(nodeEnvDB), m_registry(registry) {}

        bool streamChunkToQueue(STStreamerDetails::BoundedQueue::BoundedQueue<Config::NodePayload>& queue) {
            if (!m_nodeEnvDB) return false;

            return ANodeEnvDB::Helpers::useQuery(m_nodeEnvDB->getPool(), [&](QSqlQuery& query) {
                for (const Config::NodePayload& payload : streamChunkNodes(query)) {
                    if (!queue.pushBlocking(payload)) break;
                }
                return true;
            });
        }
    };
}