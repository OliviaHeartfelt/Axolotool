#pragma once

#include "../../NDConcepts.h"

namespace NDWidgetSourceDetails::Config {

    // 1. Source
    struct FullWidgetSourceRecord {
        muuid::uuid id;
        QString name;
    };
    struct CreateWidgetSourceRecord {
        muuid::uuid id;
        QString name;
    };
    struct UpdateWidgetSourceRecord {
        std::optional<QString> name = std::nullopt;
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
        std::optional<QString> name = std::nullopt;
    };


    // 3. Type
    template<NDConcepts::ByteConvertible Metadata>
    struct FullWidgetTypeRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        std::optional<Metadata> metadata;
    };
    template<NDConcepts::ByteConvertible Metadata>
    struct CreateWidgetTypeRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        std::optional<Metadata> metadata = std::nullopt;
    };
    template<NDConcepts::ByteConvertible Metadata>
    struct UpdateWidgetTypeRecord {
        std::optional<QString> name = std::nullopt;
        std::variant<std::monostate, std::optional<Metadata>> metadata = std::monostate{};
    };


    // 4. Data
    template<NDConcepts::ByteConvertible Data>
    struct FullWidgetDataRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        std::optional<Data> data;
    };
    template<NDConcepts::ByteConvertible Data>
    struct CreateWidgetDataRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        std::optional<Data> data = std::nullopt;
    };
    template<NDConcepts::ByteConvertible Data>
    struct UpdateWidgetDataRecord {
        std::optional<QString> name = std::nullopt;
        std::variant<std::monostate, std::optional<Data>> data = std::monostate{};
    };
}