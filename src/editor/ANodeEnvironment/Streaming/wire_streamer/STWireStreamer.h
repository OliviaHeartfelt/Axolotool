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
        ANodeEnvDB::ANodeEnvDB* m_nodeEnvDB = nullptr;
        ARegistry::Registry* m_registry = nullptr;

        std::generator<Config::WirePayload&&> streamChunkWires(QSqlQuery& query) {
            if (!m_nodeEnvDB || !m_nodeEnvDB || !m_nodeEnvDB->isOpen()) co_return;

            auto wiresOpt = m_nodeEnvDB->wire.getWiresInView(query, true);
            if (!wiresOpt || wiresOpt->isEmpty()) co_return;

            for (const auto& wireRecord : *wiresOpt) {
                Config::WirePayload payload;

                if (!STWireStreamerDetails::WireProcessing::processWire(m_nodeEnvDB, *m_registry, query, payload, wireRecord.id)) continue;

                co_yield std::move(payload);
            }
        }

    public:
        explicit STWireStreamer(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry* registry) : m_nodeEnvDB(nodeEnvDB), m_registry(registry) {}

        bool streamChunkToQueue(const muuid::uuid& chunkId, STStreamerDetails::BoundedQueue::BoundedQueue<Config::WirePayload>& queue) {
            if (!m_nodeEnvDB) return false;

            return ANodeEnvDB::Helpers::useQuery(m_nodeEnvDB->getPool(), [&](QSqlQuery& query) {
                for (const Config::WirePayload& payload : streamChunkWires(query)) {
                    if (!queue.pushBlocking(std::move(payload))) break;
                }
                return true;
            });
        }
    };
}