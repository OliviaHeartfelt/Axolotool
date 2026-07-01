#pragma once

namespace NDWidgetSourceDetails::Config {

    template<typename T>
    concept ByteConvertible = requires(const T t, QByteArray byteArray) {
        { T::byteArrayToClass(byteArray) } -> std::same_as<std::optional<T>>;
        { t.classToByteArray() } -> std::same_as<QByteArray>;
    };

    // 1. Source
    struct FullWidgetSourceRecord {
        muuid::uuid id;
        QString name;
    };
    struct CreateWidgetSourceRecord {
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
        muuid::uuid sourceId;
        QString name;
    };
    struct UpdateWidgetContributorRecord {
        std::optional<QString> name = std::nullopt;
    };


    // 3. Type
    template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
    struct FullWidgetTypeRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        Metadata metadata;
    };
    template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
    struct CreateWidgetTypeRecord {
        muuid::uuid contributorId;
        QString name;
        std::optional<Metadata> metadata = std::nullopt;
    };
    template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
    struct UpdateWidgetTypeRecord {
        std::optional<QString> name = std::nullopt;
        std::variant<std::monostate, std::optional<Metadata>> metadata = std::monostate{};
    };


    // 4. Data
    template<NDWidgetSourceDetails::Config::ByteConvertible Data>
    struct FullWidgetDataRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        QString name;
        Data data;
    };
    template<NDWidgetSourceDetails::Config::ByteConvertible Data>
    struct CreateWidgetDataRecord {
        muuid::uuid contributorId;
        QString name;
        std::optional<Data> data = std::nullopt;
    };
    template<NDWidgetSourceDetails::Config::ByteConvertible Data>
    struct UpdateWidgetDataRecord {
        std::optional<QString> name = std::nullopt;
        std::variant<std::monostate, std::optional<Data>> data = std::monostate{};
    };
}

/*
CREATE TABLE IF NOT EXISTS widget_source (
    id   BLOB PRIMARY KEY,
    name TEXT NOT NULL,
    UNIQUE(name)
);
CREATE TABLE IF NOT EXISTS widget_contributor (
    id        BLOB PRIMARY KEY,
    source_id BLOB NOT NULL REFERENCES widget_source(id) ON DELETE CASCADE,
    name      TEXT NOT NULL,
    UNIQUE(source_id, name)
);
CREATE TABLE IF NOT EXISTS widget_type (
    id             BLOB PRIMARY KEY,
    contributor_id BLOB NOT NULL REFERENCES widget_source(id) ON DELETE CASCADE,
    name           TEXT NOT NULL,
    metadata       BLOB,
    UNIQUE(contributor_id, name)
);
CREATE TABLE IF NOT EXISTS widget_data (
    id             BLOB PRIMARY KEY,
    contributor_id BLOB NOT NULL REFERENCES widget_source(id) ON DELETE CASCADE,
    name           TEXT NOT NULL,
    data           BLOB,
    UNIQUE(contributor_id, name)
);
*/