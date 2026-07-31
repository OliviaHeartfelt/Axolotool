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
        ANodeEnvDB::ANodeEnvDB* nodeEnvDB;

        std::generator<Config::NodePayload&&> streamChunkNodes(QSqlQuery& query, Config::StreamCache& cache) {
            if (!nodeEnvDB || !nodeEnvDB->isOpen()) co_return;

            auto nodesOpt = nodeEnvDB->node.getNodesInView(query, true);
            if (!nodesOpt || nodesOpt->isEmpty()) co_return;

            for (const auto& nodeRecord : *nodesOpt) {
                Config::NodePayload payload;

                if (!STNodeStreamerDetails::NodeProcessing::processNode(nodeEnvDB, query, cache, payload, nodeRecord.id)) continue;

                co_yield std::move(payload);
            }
        }

    public:
        explicit STNodeStreamer(ANodeEnvDB::ANodeEnvDB* _nodeEnvDB) : nodeEnvDB(_nodeEnvDB) {}

        bool streamChunkToQueue(STStreamerDetails::BoundedQueue::BoundedQueue<Config::NodePayload>& queue) {
            if (!nodeEnvDB) return false;

            return ANodeEnvDB::Helpers::useQuery(nodeEnvDB->getPool(), [&](QSqlQuery& query) {
                Config::StreamCache cache;

                for (const Config::NodePayload& payload : streamChunkNodes(query, cache)) {
                    if (!queue.pushBlocking(std::move(payload))) break;
                }
                return true;
            });
        }
    };
}