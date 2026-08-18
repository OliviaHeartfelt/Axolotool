#pragma once

namespace ARegistryDetails::Context {

    struct CreateCellRecord {
        std::optional<muuid::uuid> id = std::nullopt;
        muuid::uuid nodeId;
        std::optional<QString> name = std::nullopt;
        std::optional<muuid::uuid> pinCoreId = std::nullopt;
        std::optional<muuid::uuid> widgetCoreId = std::nullopt;
        short row = -1;
        short col = -1;
        short rowSpan = 0;
        short colSpan = 0;
    };
}