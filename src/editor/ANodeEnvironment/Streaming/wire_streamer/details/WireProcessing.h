#pragma once

#include "Config.h"
#include "../../details/Helper.h"

#include "../../../Storage/ANodeEnvDB.h"
#include "../../../Registry/ARegistry.h"

namespace STWireStreamerDetails::WireProcessing {

	inline std::optional<ANodeEnvDB::Config::Wire::FullWireCoreRecord> processWireCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const muuid::uuid& wireCoreId);
	inline bool processWireData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Wire::FullWireCoreRecord& wireCore);
	inline bool processWireStyle(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Wire::FullWireCoreRecord& wireCore);

	inline bool processWire(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, Config::WirePayload& payload, const muuid::uuid& nodeId) {
		if (!nodeEnvDB) return false;

		const auto wireOpt = nodeEnvDB->wire.getWire(query, nodeId);
		if (!wireOpt) return false;

		payload.wire = *wireOpt;

		const auto wireCoreOpt = processWireCore(nodeEnvDB, registry, query, wireOpt->coreId);

		if (!wireCoreOpt) return false;

		if (!processWireData(nodeEnvDB, registry, query, *wireCoreOpt) || !processWireStyle(nodeEnvDB, registry, query, *wireCoreOpt)) return false;
		return true;
	}
	inline std::optional<ANodeEnvDB::Config::Wire::FullWireCoreRecord> processWireCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const muuid::uuid& wireCoreId) {
		const auto wireCore = STStreamerDetails::Helper::getOrFetch(registry.wire.wireCoreRegistry, wireCoreId, [&]() {
			return nodeEnvDB->wire.getWireCore(query, wireCoreId);
			});
		if (!wireCore) return std::nullopt;

		return *wireCore;
	}

	inline bool processWireData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Wire::FullWireCoreRecord& wireCore) {
		if (wireCore.dataId) {
			STStreamerDetails::Helper::ensureCached(registry.wire.wireDataRegistry, *wireCore.dataId, [&]() {
				return nodeEnvDB->wireSource.getWireData(query, *wireCore.dataId);
				});
		}
		return true;
	}
	inline bool processWireStyle(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Wire::FullWireCoreRecord& wireCore) {
		if (wireCore.styleId) {
			STStreamerDetails::Helper::ensureCached(registry.wire.wireStyleRegistry, *wireCore.styleId, [&]() {
				return nodeEnvDB->wireSource.getWireStyle(query, *wireCore.styleId);
			});
		}
		return true;
	}
}