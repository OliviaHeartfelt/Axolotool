#pragma once

#include "../../NDParser.h"
#include "../../NDConcepts.h"

namespace NDWidgetDetails::Config {

    // Widget Core
    struct FullWidgetCoreRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        muuid::uuid visualFactoryId;
        std::optional<muuid::uuid> typeId;
        std::optional<muuid::uuid> dataId;
    };
    struct CreateWidgetCoreRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        muuid::uuid visualFactoryId;
        std::optional<muuid::uuid> typeId = std::nullopt;
        std::optional<muuid::uuid> dataId = std::nullopt;

        static std::optional<CreateWidgetCoreRecord> Parse(const QJsonObject& obj) {
            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto contributorId = NDParser::parse<muuid::uuid>(obj, "contributorId");
            if (!contributorId) return std::nullopt;

            auto visualFactoryId = NDParser::parse<muuid::uuid>(obj, "visualFactoryId");
            if (!visualFactoryId) return std::nullopt;

            auto typeId = NDParser::parse<muuid::uuid>(obj, "typeId", true);
            auto dataId = NDParser::parse<muuid::uuid>(obj, "dataId", true);

            return CreateWidgetCoreRecord{
                .id = *optID,
                .contributorId = *contributorId,
                .visualFactoryId = *visualFactoryId,
                .typeId = typeId,
                .dataId = dataId
            };
        }
    };
    struct UpdateWidgetCoreRecord {
        std::optional<muuid::uuid> id =              std::nullopt;
        std::optional<muuid::uuid> contributorId =   std::nullopt;
        std::optional<muuid::uuid> visualFactoryId = std::nullopt;
        std::variant<std::monostate, std::optional<muuid::uuid>> typeId = std::monostate{};
        std::variant<std::monostate, std::optional<muuid::uuid>> dataId = std::monostate{};
    };

    // Widget
    struct FullWidgetRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        std::optional<std::vector<uint8_t>> state;
        qreal w;
        qreal h;
    };
    struct CreateWidgetRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        std::optional<std::vector<uint8_t>> state = std::nullopt;
        std::optional<qreal> w = std::nullopt;
        std::optional<qreal> h = std::nullopt;

        static std::optional<CreateWidgetRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto coreId = NDParser::parse<muuid::uuid>(obj, "coreId");
            if (!coreId) return std::nullopt;

            auto w = NDParser::parse<double>(obj, "w", true);
            auto h = NDParser::parse<double>(obj, "h", true);

            return CreateWidgetRecord{
                .id = *optID,
                .coreId = *coreId,
                .w = w,
                .h = h
            };
        }
    };
    struct FactoryWidgetRecord {
        std::optional<qreal> w = std::nullopt;
        std::optional<qreal> h = std::nullopt;
    };
    struct UpdateWidgetRecord {
        std::optional<muuid::uuid> id =     std::nullopt;
        std::optional<muuid::uuid> coreId = std::nullopt;
        std::variant<std::monostate, std::optional<std::vector<uint8_t>>> state = std::monostate{};
        std::optional<qreal> width  = std::nullopt;
        std::optional<qreal> height = std::nullopt;
    };
}