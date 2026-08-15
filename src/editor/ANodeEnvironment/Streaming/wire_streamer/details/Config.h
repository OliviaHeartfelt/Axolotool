#pragma once

#include "../../../Storage/ANodeEnvDB.h"

namespace STWireStreamerDetails::Config {    

    using namespace ANodeEnvDB::Config;

    struct WirePayload {
        Wire::FullWireRecord wire;
    };
}