#pragma once

#include "details/Config.h"
#include "details/WireProcessing.h"
#include "../details/BoundedQueue.h"

#include "../../Storage/ANodeEnvDB.h"

namespace STWireStreamer {

    namespace Config {
        using namespace STWireStreamerDetails::Config;
    }

    class STWireStreamer {
        ANodeEnvDB::ANodeEnvDB* nodeEnvDB;

        std::generator<Config::WirePayload&&> streamChunkWires(QSqlQuery& query, Config::StreamCache& cache) {
            if (!nodeEnvDB || !nodeEnvDB->isOpen()) co_return;

            auto wiresOpt = nodeEnvDB->wire.getWiresInView(query, true);
            if (!wiresOpt || wiresOpt->isEmpty()) co_return;

            for (const auto& wireRecord : *wiresOpt) {
                Config::WirePayload payload;

                if (!STWireStreamerDetails::WireProcessing::processWire(nodeEnvDB, query, cache, payload, wireRecord.id)) continue;

                co_yield std::move(payload);
            }
        }

    public:
        explicit STWireStreamer(ANodeEnvDB::ANodeEnvDB* _nodeEnvDB) : nodeEnvDB(_nodeEnvDB) {}

        bool streamChunkToQueue(STStreamerDetails::BoundedQueue::BoundedQueue<Config::WirePayload>& queue) {
            if (!nodeEnvDB) return false;

            return ANodeEnvDB::Helpers::useQuery(nodeEnvDB->getPool(), [&](QSqlQuery& query) {
                Config::StreamCache cache;

                for (const Config::WirePayload& payload : streamChunkWires(query, cache)) {
                    if (!queue.pushBlocking(std::move(payload))) break;
                }
                return true;
            });
        }
    };
}