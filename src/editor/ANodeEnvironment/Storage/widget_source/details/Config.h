#pragma once

#include "../../NDParser.h"
#include "../../NDConcepts.h"

namespace NDWidgetSourceDetails::Config {

    // 1. Source
    struct FullWidgetSourceRecord {
        muuid::uuid id;
        std::optional<muuid::uuid> globalSourceId;
        QString name;
    };
    struct CreateWidgetSourceRecord {
        muuid::uuid id;
        std::optional<muuid::uuid> globalSourceId = std::nullopt;
        QString name;

        static std::optional<CreateWidgetSourceRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto globalSourceId = NDParser::parse<muuid::uuid>(obj, "globalSourceId", true);

            auto name = NDParser::parse<QString>(obj, "name");
            if (!name) return std::nullopt;

            return CreateWidgetSourceRecord{
                .id = *optID,
                .globalSourceId = globalSourceId,
                .name = *name
            };
        }
    };
    struct UpdateWidgetSourceRecord {
        std::optional<muuid::uuid> id =             std::nullopt;
        std::optional<muuid::uuid> globalSourceId = std::nullopt;
        std::optional<QString> name =               std::nullopt;
    };


    // 2. Contributor
    struct FullWidgetContributorRecord {
        muuid::uuid id;
        muuid::uuid sourceId;
        QString name;
    };
    struct CreateWidgetContributorRecord {
        muuid::uuid id;
        muuid::uuid sourceId;
        QString name;

        static std::optional<CreateWidgetContributorRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto sourceId = NDParser::parse<muuid::uuid>(obj, "sourceId", true);
            if (!sourceId) return std::nullopt;

            auto name = NDParser::parse<QString>(obj, "name");
            if (!name) return std::nullopt;

            return CreateWidgetContributorRecord{
                .id = *optID,
                .sourceId = *sourceId,
                .name = *name
            };
        }
    };
    struct UpdateWidgetContributorRecord {
        std::optional<muuid::uuid> id =       std::nullopt;
        std::optional<muuid::uuid> sourceId = std::nullopt;
        std::optional<QString> name =         std::nullopt;
    };


    // 3. Type
    struct FullWidgetTypeRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        std::optional<std::vector<uint8_t>> metadata;
    };
    struct CreateWidgetTypeRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        std::optional<std::vector<uint8_t>> metadata = std::nullopt;

        static std::optional<CreateWidgetTypeRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto contributorId = NDParser::parse<muuid::uuid>(obj, "contributorId", true);
            if (!contributorId) return std::nullopt;

            auto name = NDParser::parse<QString>(obj, "name");
            if (!name) return std::nullopt;

            return CreateWidgetTypeRecord{
                .id = *optID,
                .contributorId = *contributorId,
                .name = *name
            };
        }
    };
    struct UpdateWidgetTypeRecord {
        std::optional<muuid::uuid> id =            std::nullopt;
        std::optional<muuid::uuid> contributorId = std::nullopt;
        std::optional<QString> name =              std::nullopt;
        std::variant<std::monostate, std::optional<std::vector<uint8_t>>> metadata = std::monostate{};
    };


    // 4. Data
    struct FullWidgetDataRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        std::optional<std::vector<uint8_t>> data;
    };
    struct CreateWidgetDataRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        std::optional<std::vector<uint8_t>> data = std::nullopt;

        static std::optional<CreateWidgetDataRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto contributorId = NDParser::parse<muuid::uuid>(obj, "contributorId", true);
            if (!contributorId) return std::nullopt;

            auto name = NDParser::parse<QString>(obj, "name");
            if (!name) return std::nullopt;

            return CreateWidgetDataRecord{
                .id = *optID,
                .contributorId = *contributorId,
                .name = *name
            };
        }
    };
    struct UpdateWidgetDataRecord {
        std::optional<muuid::uuid> id =            std::nullopt;
        std::optional<muuid::uuid> contributorId = std::nullopt;
        std::optional<QString> name =              std::nullopt;
        std::variant<std::monostate, std::optional<std::vector<uint8_t>>> data = std::monostate{};
    };
}