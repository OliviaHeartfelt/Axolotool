#pragma once

#include "Config.h"
#include "../../details/Helper.h"

#include "../../../Storage/ANodeEnvDB.h"

namespace STWireStreamerDetails::WireProcessing {

	using namespace STStreamerDetails;

	inline std::optional<ANodeEnvDB::Config::Wire::FullWireCoreRecord> processWireCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::WirePayload& payload, const muuid::uuid& wireCoreId);
	inline bool processWireData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::WirePayload& payload, const ANodeEnvDB::Config::Wire::FullWireCoreRecord& wireCore);
	inline bool processWireStyle(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::WirePayload& payload, const ANodeEnvDB::Config::Wire::FullWireCoreRecord& wireCore);

	inline bool processWire(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::WirePayload& payload, const muuid::uuid& nodeId) {
		if (!nodeEnvDB) return false;

		const auto wireOpt = nodeEnvDB->wire.getWire(query, nodeId);
		if (!wireOpt) return false;

		payload.wire = *wireOpt;

		const auto wireCoreOpt = processWireCore(nodeEnvDB, query, cache, payload, wireOpt->coreId);

		if (!wireCoreOpt) return false;

		if (!processWireData(nodeEnvDB, query, cache, payload, *wireCoreOpt) || !processWireStyle(nodeEnvDB, query, cache, payload, *wireCoreOpt)) return false;

		return true;
	}
	inline std::optional<ANodeEnvDB::Config::Wire::FullWireCoreRecord> processWireCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::WirePayload& payload, const muuid::uuid& wireCoreId) {
		const ANodeEnvDB::Config::Wire::FullWireCoreRecord* wireCore = Helper::getOrFetch(cache.wireCore, wireCoreId, [&]() {
			return nodeEnvDB->node.getNodeCore(query, wireCoreId);
			});

		if (wireCore) {
			payload.wireCore.emplace(wireCoreId, *wireCore);
			return *wireCore;
		}
		return std::nullopt;
	}

	inline bool processWireData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::WirePayload& payload, const ANodeEnvDB::Config::Wire::FullWireCoreRecord& wireCore) {
		if (wireCore.dataId) {
			Helper::hydratePayloadOpt(cache.wireData, payload.wireData, *wireCore.dataId, [&]() {
				return nodeEnvDB->wireSource.getWireData(query, *wireCore.dataId);
				});
		}
		return true;
	}
	inline bool processWireStyle(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::WirePayload& payload, const ANodeEnvDB::Config::Wire::FullWireCoreRecord& wireCore) {
		if (wireCore.styleId) {
			Helper::hydratePayloadOpt(cache.wireStyle, payload.wireStyle, *wireCore.styleId, [&]() {
				return nodeEnvDB->wireSource.getWireStyle(query, *wireCore.styleId);
			});
		}
		return true;
	}
}