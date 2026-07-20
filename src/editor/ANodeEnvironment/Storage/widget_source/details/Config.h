#pragma once

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
    };
    struct UpdateWidgetDataRecord {
        std::optional<muuid::uuid> id =            std::nullopt;
        std::optional<muuid::uuid> contributorId = std::nullopt;
        std::optional<QString> name =              std::nullopt;
        std::variant<std::monostate, std::optional<std::vector<uint8_t>>> data = std::monostate{};
    };
}