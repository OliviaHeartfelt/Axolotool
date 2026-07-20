#pragma once

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
    };
    struct UpdateWireDataRecord {
        std::optional<muuid::uuid> id = std::nullopt;
        std::optional<muuid::uuid> contributorId = std::nullopt;
        std::optional<QString> name = std::nullopt;
        std::variant<std::monostate, std::optional<std::vector<uint8_t>>> data = std::monostate{};
    };
}