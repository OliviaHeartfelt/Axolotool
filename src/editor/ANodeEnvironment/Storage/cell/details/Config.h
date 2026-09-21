#pragma once

#include "../../NDParser.h"

namespace NDCellDetails::Config {

    struct CellInfo {
        short row = -1;
        short col = -1;
        short rowSpan = 0;
        short colSpan = 0;
    };
    struct CreateCellRecord {
        muuid::uuid id;
        muuid::uuid nodeId;
        std::optional<QString> name = std::nullopt;
        std::optional<muuid::uuid> pinTemplateId = std::nullopt;
        std::optional<muuid::uuid> pinInstanceId = std::nullopt;
        std::optional<muuid::uuid> widgetId =      std::nullopt;
        bool isOut = false;
        short row = -1;
        short col = -1;
        short rowSpan = 0;
        short colSpan = 0;

        static std::optional<CreateCellRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto nodeId = NDParser::parse<muuid::uuid>(obj, "nodeId");
            if (!nodeId) return std::nullopt;

            auto optName = NDParser::parse<QString>(obj, "name", true);
            auto pinTemplateId = NDParser::parse<muuid::uuid>(obj, "pinTemplateId", true);
            auto pinInstanceId = NDParser::parse<muuid::uuid>(obj, "pinInstanceId", true);
            auto widgetId = NDParser::parse<muuid::uuid>(obj, "widgetId", true);

            auto isOut = NDParser::parse<bool>(obj, "isOut");
            if (!isOut) return std::nullopt;

            auto row = NDParser::parse<short>(obj, "row");
            if (!row) return std::nullopt;

            auto col = NDParser::parse<short>(obj, "col");
            if (!col) return std::nullopt;

            auto rowSpan = NDParser::parse<short>(obj, "rowSpan");
            if (!rowSpan) return std::nullopt;

            auto colSpan = NDParser::parse<short>(obj, "colSpan");
            if (!colSpan) return std::nullopt;

            return CreateCellRecord{
                .id = *optID,
                .nodeId = *nodeId,
                .name = optName,
                .pinTemplateId = pinTemplateId,
                .pinInstanceId = pinInstanceId,
                .widgetId = widgetId,
                .isOut = *isOut,
                .row = *row,
                .col = *col,
                .rowSpan = *rowSpan,
                .colSpan = *colSpan
            };
        }
    };
    struct FactoryCellRecord {
        muuid::uuid nodeId;
        std::optional<QString> name = std::nullopt;
        std::optional<muuid::uuid> pinTemplateId = std::nullopt;
        std::optional<muuid::uuid> pinInstanceId = std::nullopt;
        std::optional<muuid::uuid> widgetId = std::nullopt;
        bool isOut = false;
        short row = -1;
        short col = -1;
        short rowSpan = 0;
        short colSpan = 0;
    };
    struct UpdateCellRecord {
        std::optional<muuid::uuid> id     = std::nullopt;
        std::optional<muuid::uuid> nodeId = std::nullopt;
        std::variant<std::monostate, std::optional<QString>> name = std::monostate{};
        std::variant<std::monostate, std::optional<muuid::uuid>> pinTemplateId = std::monostate{};
        std::variant<std::monostate, std::optional<muuid::uuid>> pinInstanceId = std::monostate{};
        std::variant<std::monostate, std::optional<muuid::uuid>> widgetId =      std::monostate{};
        std::optional<bool> isOut =    false;
        std::optional<short> row =     std::nullopt;
        std::optional<short> col =     std::nullopt;
        std::optional<short> rowSpan = std::nullopt;
        std::optional<short> colSpan = std::nullopt;
    };

    struct FullCellRecord {
        muuid::uuid id;
        muuid::uuid nodeId;
        std::optional<QString> name;
        bool isOut;
        short row;
        short col;
        short rowSpan;
        short colSpan;
        std::optional<muuid::uuid> pinTemplateId;
        std::optional<muuid::uuid> pinInstanceId;
        std::optional<muuid::uuid> widgetId;

        static CreateCellRecord toCreate(const FullCellRecord& fullrecord) {
            return CreateCellRecord{
                .id = fullrecord.id,
                .nodeId = fullrecord.nodeId,
                .name = fullrecord.name,
                .pinTemplateId = fullrecord.pinTemplateId,
                .pinInstanceId = fullrecord.pinInstanceId,
                .widgetId = fullrecord.widgetId,
                .isOut = fullrecord.isOut,
                .row = fullrecord.row,
                .col = fullrecord.col,
                .rowSpan = fullrecord.rowSpan,
                .colSpan = fullrecord.colSpan
            };
        }
        static UpdateCellRecord toUpdate(const FullCellRecord& fullrecord) {
            return UpdateCellRecord{
                .id = std::nullopt,
                .nodeId = std::nullopt,
                .name = fullrecord.name,
                .pinTemplateId = fullrecord.pinTemplateId,
                .pinInstanceId = fullrecord.pinInstanceId,
                .widgetId = fullrecord.widgetId,
                .isOut = fullrecord.isOut,
                .row = fullrecord.row,
                .col = fullrecord.col,
                .rowSpan = fullrecord.rowSpan,
                .colSpan = fullrecord.colSpan
            };
        }
    };
}