#pragma once

#include "../../../Storage/ANodeEnvDB.h"

namespace STWireStreamerDetails::Config {    

    using namespace ANodeEnvDB::Config;

    struct WirePayload {
        Wire::FullWireRecord wire;
        std::optional<Wire::FullWireCoreRecord> wireCore;
        std::optional<WireSource::FullWireDataRecord> wireData;
        std::optional<WireSource::FullWireStyleRecord> wireStyle;
    };

    struct StreamCache {
        std::unordered_map<muuid::uuid, Wire::FullWireCoreRecord> wireCore;
        std::unordered_map<muuid::uuid, WireSource::FullWireDataRecord> wireData;
        std::unordered_map<muuid::uuid, WireSource::FullWireStyleRecord> wireStyle;
    };
}