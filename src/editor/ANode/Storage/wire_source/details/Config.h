#pragma once

#include "../../NDConcepts.h"

namespace NDWireSourceDetails::Config {

    // 1. Wire Source
    struct FullWireSourceRecord {
        muuid::uuid id;
        QString name;
    };
    struct CreateWireSourceRecord {
        muuid::uuid id;
        QString name;
    };
    struct UpdateWireSourceRecord {
        std::optional<QString> name = std::nullopt;
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
        std::optional<QString> name = std::nullopt;
    };

    // 3. Wire Style
    template<NDConcepts::ByteConvertible Metadata>
    struct FullWireStyleRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        QColor color;
        int wireThickness;
        std::optional<Metadata> metadata;
    };
    template<NDConcepts::ByteConvertible Metadata>
    struct CreateWireStyleRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        QColor color = Qt::gray;
        int wireThickness = 2;
        std::optional<Metadata> metadata = std::nullopt;
    };
    template<NDConcepts::ByteConvertible Metadata>
    struct UpdateWireStyleRecord {
        std::optional<QString> name = std::nullopt;
        std::optional<QColor> color = std::nullopt;
        std::optional<int> wireThickness = std::nullopt;
        std::variant<std::monostate, std::optional<Metadata>> metadata = std::monostate{};
    };

    // 4. Wire Data
    template<NDConcepts::ByteConvertible Data>
    struct FullWireDataRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        std::optional<Data> data;
    };
    template<NDConcepts::ByteConvertible Data>
    struct CreateWireDataRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        std::optional<Data> data = std::nullopt;
    };
    template<NDConcepts::ByteConvertible Data>
    struct UpdateWireDataRecord {
        std::optional<QString> name = std::nullopt;
        std::variant<std::monostate, std::optional<Data>> data = std::monostate{};
    };
}