#pragma once

namespace NDCellDetails::Config {

    struct PinItemRecord { };
    struct WidgetRecord { };

    using CellContent = std::variant<std::monostate, PinItemRecord, WidgetRecord>;

    struct alignas(8) Record {
        muuid::uuid id;
        muuid::uuid nodeId;
        CellContent content;
        short row = -1;
        short col = -1;
        short rowSpan = 0;
        short colSpan = 0;
    };
    struct alignas(8) RecordInfo {
        short row = -1;
        short col = -1;
        short rowSpan = 0;
        short colSpan = 0;
    };
}