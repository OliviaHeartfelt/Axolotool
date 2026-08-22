#pragma once

#include "../../../Storage/ANodeEnvDB.h"

namespace STLoadingWireStreamerDetails::Config {

    using namespace ANodeEnvDB::Config;

    struct WirePayload {
        Wire::FullWireRecord wire;
    };
}