#pragma once

namespace NDCellDetails::Config {

    struct CellInfo {
        short row = -1;
        short col = -1;
        short rowSpan = 0;
        short colSpan = 0;
    };
    struct FullCellRecord {
        muuid::uuid id;
        muuid::uuid nodeId;
        QString name;
        bool isOut;
        short row;
        short col;
        short rowSpan;
        short colSpan;
        std::optional<muuid::uuid> pinId;
        std::optional<muuid::uuid> widgetId;
    };
    struct CreateCellRecord {
        muuid::uuid id;
        muuid::uuid nodeId;
        std::optional<QString> name =         std::nullopt;
        std::optional<muuid::uuid> pinId =    std::nullopt;
        std::optional<muuid::uuid> widgetId = std::nullopt;
        bool isOut = false;
        short row = -1;
        short col = -1;
        short rowSpan = 0;
        short colSpan = 0;
    };
    struct UpdateCellRecord {
        std::optional<QString> name =         std::nullopt;
        std::optional<muuid::uuid> pinId =    std::nullopt;
        std::optional<muuid::uuid> widgetId = std::nullopt;
        std::optional<bool> isOut =    false;
        std::optional<short> row =     std::nullopt;
        std::optional<short> col =     std::nullopt;
        std::optional<short> rowSpan = std::nullopt;
        std::optional<short> colSpan = std::nullopt;
    };
}