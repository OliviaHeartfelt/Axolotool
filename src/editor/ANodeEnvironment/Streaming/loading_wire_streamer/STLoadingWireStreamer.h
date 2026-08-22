#pragma once

#include "details/Config.h"
#include "details/WireProcessing.h"
#include "../details/BoundedQueue.h"

#include "../../Storage/ANodeEnvDB.h"

namespace STLoadingWireStreamer {

    namespace Config {
        using namespace STLoadingWireStreamerDetails::Config;
    }

    class STLoadingWireStreamer {
        ANodeEnvDB::ANodeEnvDB* m_nodeEnvDB = nullptr;
        ARegistry::Registry* m_registry = nullptr;

        std::generator<Config::WirePayload> streamChunkWires(QSqlQuery& query) {
            if (!m_registry || !m_nodeEnvDB || !m_nodeEnvDB->isOpen()) co_return;

            auto wiresOpt = m_nodeEnvDB->wire.getWiresInView(query, true);
            if (!wiresOpt || wiresOpt->isEmpty()) co_return;

            for (const auto& wireRecord : *wiresOpt) {
                Config::WirePayload payload;
                if (!STLoadingWireStreamerDetails::WireProcessing::processWire(m_nodeEnvDB, *m_registry, query, payload, wireRecord.id)) continue;

                co_yield std::move(payload);
            }
        }

    public:
        explicit STLoadingWireStreamer(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry* registry) : m_nodeEnvDB(nodeEnvDB), m_registry(registry) {}

        bool streamChunkToQueue(STStreamerDetails::BoundedQueue::BoundedQueue<Config::WirePayload>& queue) {
            if (!m_nodeEnvDB) return false;

            return ANodeEnvDB::Helpers::useQuery(m_nodeEnvDB->getPool(), [&](QSqlQuery& query) {
                for (const Config::WirePayload& payload : streamChunkWires(query)) {
                    if (!queue.pushBlocking(payload)) break;
                }
                return true;
            });
        }
    };
}