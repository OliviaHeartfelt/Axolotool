#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDWireSourceDetails::Read {

    // 1. Source
    inline std::optional<NDWireSourceDetails::Config::FullWireSourceRecord> getWireSource(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT global_source_id, name
            FROM wire_source 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWireSource query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> globalSourceId = NDHelpers::parseNullableUUID(query.value(0));
        if (globalSourceId.isCorrupted()) return std::nullopt;

        return NDWireSourceDetails::Config::FullWireSourceRecord{
            id,
            globalSourceId.value,
            query.value(1).toString()
        };
    }

    inline std::optional<QList<NDWireSourceDetails::Config::FullWireSourceRecord>> getAllWireSources(QSqlQuery& query, const bool continueAtFail = false) {
        QList<NDWireSourceDetails::Config::FullWireSourceRecord> list;

        query.prepare(R"(
            SELECT id, global_source_id, name
            FROM wire_source;
        )");

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWireSources query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const NDHelpers::NullableField<muuid::uuid> globalSourceId = NDHelpers::parseNullableUUID(query.value(1));

            if (!id || globalSourceId.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireSourceDetails::Config::FullWireSourceRecord{
                *id,
                globalSourceId.value,
                query.value(2).toString()
            });
        }
        return list;
    }

    // 2. Contributor
    inline std::optional<NDWireSourceDetails::Config::FullWireContributorRecord> getWireContributor(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT source_id, name
            FROM wire_contributor 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWireContributor query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto sourceId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!sourceId) return std::nullopt;

        return NDWireSourceDetails::Config::FullWireContributorRecord{
            id,
            *sourceId,
            query.value(1).toString()
        };
    }

    inline std::optional<QList<NDWireSourceDetails::Config::FullWireContributorRecord>> getAllWireContributors(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWireSourceDetails::Config::FullWireContributorRecord> list;

        query.prepare(R"(
            SELECT id, name
            FROM wire_contributor
            WHERE source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWireContributors query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());

            if (!id) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireSourceDetails::Config::FullWireContributorRecord{
                *id,
                sourceId,
                query.value(1).toString()
            });
        }
        return list;
    }

    // 3. Style
    template<NDConcepts::ByteConvertible Metadata>
    inline std::optional<NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>> getWireStyle(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, name, color, wire_thickness, metadata
            FROM wire_style 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWireStyle query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        const NDHelpers::NullableField<Metadata> metadata = NDHelpers::parseNullableByteConvertible<Metadata>(query.value(4));
        if (metadata.isCorrupted()) return std::nullopt;

        return NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>{
            id,
            *contributorId,
            query.value(1).toString(),
            QColor::fromRgba(query.value(2).toUInt()),
            query.value(3).toInt(),
            metadata.value
        };
    }

    template<NDConcepts::ByteConvertible Metadata>
    inline std::optional<QList<NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>>> getContributorWireStyles(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>> list;

        query.prepare(R"(
            SELECT id, name, color, wire_thickness, metadata
            FROM wire_style
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWireStyles query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const NDHelpers::NullableField<Metadata> metadata = NDHelpers::parseNullableByteConvertible<Metadata>(query.value(4));

            if (!id || metadata.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>{
                *id,
                contributorId,
                query.value(1).toString(),
                QColor::fromRgba(query.value(2).toUInt()),
                query.value(3).toInt(),
                metadata.value
            });
        }
        return list;
    }

    template<NDConcepts::ByteConvertible Metadata>
    inline std::optional<QList<NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>>> getAllWireStyles(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>> list;

        query.prepare(R"(
            SELECT ws.id, ws.contributor_id, ws.name, ws.color, ws.wire_thickness, ws.metadata
            FROM wire_style ws
            INNER JOIN wire_contributor c ON ws.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWireStyles query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id =            Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const NDHelpers::NullableField<Metadata> metadata = NDHelpers::parseNullableByteConvertible<Metadata>(query.value(5));

            if (!id || !contributorId || metadata.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>{
                *id,
                *contributorId,
                query.value(2).toString(),
                QColor::fromRgba(query.value(3).toUInt()),
                query.value(4).toInt(),
                metadata.value
            });
        }
        return list;
    }

    // 4. Data
    template<NDConcepts::ByteConvertible Data>
    inline std::optional<NDWireSourceDetails::Config::FullWireDataRecord<Data>> getWireData(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, name, data
            FROM wire_data 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWireData query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        const NDHelpers::NullableField<Data> data = NDHelpers::parseNullableByteConvertible<Data>(query.value(2));
        if (data.isCorrupted()) return std::nullopt;

        return NDWireSourceDetails::Config::FullWireDataRecord<Data>{
            id,
            *contributorId,
            query.value(1).toString(),
            data.value
        };
    }

    template<NDConcepts::ByteConvertible Data>
    inline std::optional<QList<NDWireSourceDetails::Config::FullWireDataRecord<Data>>> getContributorWireData(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWireSourceDetails::Config::FullWireDataRecord<Data>> list;

        query.prepare(R"(
            SELECT id, name, data
            FROM wire_data
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWireData query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const NDHelpers::NullableField<Data> data = NDHelpers::parseNullableByteConvertible<Data>(query.value(2));

            if (!id || data.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireSourceDetails::Config::FullWireDataRecord<Data>{
                *id,
                contributorId,
                query.value(1).toString(),
                data.value
            });
        }
        return list;
    }

    template<NDConcepts::ByteConvertible Data>
    inline std::optional<QList<NDWireSourceDetails::Config::FullWireDataRecord<Data>>> getAllWireData(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWireSourceDetails::Config::FullWireDataRecord<Data>> list;

        query.prepare(R"(
            SELECT wd.id, wd.contributor_id, wd.name, wd.data
            FROM wire_data wd
            INNER JOIN wire_contributor c ON wd.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWireData query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id =            Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const NDHelpers::NullableField<Data> data = NDHelpers::parseNullableByteConvertible<Data>(query.value(3));

            if (!id || !contributorId || data.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireSourceDetails::Config::FullWireDataRecord<Data>{
                *id,
                *contributorId,
                query.value(2).toString(),
                data.value
            });
        }
        return list;
    }
}