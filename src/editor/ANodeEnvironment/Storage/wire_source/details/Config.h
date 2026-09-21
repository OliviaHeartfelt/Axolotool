#pragma once

#include "../../NDParser.h"
#include "../../NDConcepts.h"

namespace NDWireSourceDetails::Config {

    // 1. Wire Source
    struct FullWireSourceRecord {
        muuid::uuid id;
        std::optional<muuid::uuid> globalSourceId;
        QString name;
    };
    struct CreateWireSourceRecord {
        muuid::uuid id;
        std::optional<muuid::uuid> globalSourceId = std::nullopt;
        QString name;

        static std::optional<CreateWireSourceRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto globalSourceId = NDParser::parse<muuid::uuid>(obj, "globalSourceId", true);

            auto name = NDParser::parse<QString>(obj, "name", true);
            if (!name) return std::nullopt;

            return CreateWireSourceRecord{
                .id = *optID,
                .globalSourceId = globalSourceId,
                .name = *name
            };
        }
    };
    struct UpdateWireSourceRecord {
        std::optional<muuid::uuid> id =             std::nullopt;
        std::optional<muuid::uuid> globalSourceId = std::nullopt;
        std::optional<QString> name =               std::nullopt;
    };

    // 2. Wire Contributor
    struct FullWireContributorRecord {
        muuid::uuid id;
        muuid::uuid sourceId;
        QString name;
    };
    struct CreateWireContributorRecord {
        muuid::uuid id;
        muuid::uuid sourceId;
        QString name;

        static std::optional<CreateWireContributorRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto sourceId = NDParser::parse<muuid::uuid>(obj, "sourceId");
            if (!sourceId) return std::nullopt;

            auto name = NDParser::parse<QString>(obj, "name", true);
            if (!name) return std::nullopt;

            return CreateWireContributorRecord{
                .id = *optID,
                .sourceId = *sourceId,
                .name = *name
            };
        }
    };
    struct UpdateWireContributorRecord {
        std::optional<muuid::uuid> id =       std::nullopt;
        std::optional<muuid::uuid> sourceId = std::nullopt;
        std::optional<QString> name =         std::nullopt;
    };

    // 3. Wire Style
    struct FullWireStyleRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        QColor color;
        int wireThickness;
        std::optional<std::vector<uint8_t>> metadata;
    };
    struct CreateWireStyleRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        QColor color = Qt::gray;
        int wireThickness = 2;
        std::optional<std::vector<uint8_t>> metadata = std::nullopt;

        static std::optional<CreateWireStyleRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto contributorId = NDParser::parse<muuid::uuid>(obj, "contributorId");
            if (!contributorId) return std::nullopt;

            auto name = NDParser::parse<QString>(obj, "name", true);
            if (!name) return std::nullopt;

            auto color = NDParser::parse<QColor>(obj, "color", true);
            if (!color) return std::nullopt;

            auto wireThickness = NDParser::parse<int>(obj, "wireThickness", true);
            if (!wireThickness) return std::nullopt;

            return CreateWireStyleRecord{
                .id = *optID,
                .contributorId = *contributorId,
                .name = *name,
                .color = *color,
                .wireThickness = *wireThickness
            };
        }
    };
    struct UpdateWireStyleRecord {
        std::optional<muuid::uuid> id =            std::nullopt;
        std::optional<muuid::uuid> contributorId = std::nullopt;
        std::optional<QString> name =              std::nullopt;
        std::optional<QColor> color =              std::nullopt;
        std::optional<int> wireThickness =         std::nullopt;
        std::variant<std::monostate, std::optional<std::vector<uint8_t>>> metadata = std::monostate{};
    };

    // 4. Wire Data
    struct FullWireDataRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        std::optional<std::vector<uint8_t>> data;
    };
    struct CreateWireDataRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        std::optional<std::vector<uint8_t>> data = std::nullopt;

        static std::optional<CreateWireDataRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto contributorId = NDParser::parse<muuid::uuid>(obj, "contributorId");
            if (!contributorId) return std::nullopt;

            auto name = NDParser::parse<QString>(obj, "name", true);
            if (!name) return std::nullopt;

            return CreateWireDataRecord{
                .id = *optID,
                .contributorId = *contributorId,
                .name = *name,
            };
        }
    };
    struct UpdateWireDataRecord {
        std::optional<muuid::uuid> id = std::nullopt;
        std::optional<muuid::uuid> contributorId = std::nullopt;
        std::optional<QString> name = std::nullopt;
        std::variant<std::monostate, std::optional<std::vector<uint8_t>>> data = std::monostate{};
    };
}