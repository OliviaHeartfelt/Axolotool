#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDWidgetSourceDetails::Read {

    // 1. Source
    inline std::optional<NDWidgetSourceDetails::Config::FullWidgetSourceRecord> getWidgetSource(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT global_source_id, name
            FROM widget_source 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWidgetSource query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> globalSourceId = NDHelpers::parseNullableUUID(query.value(0));
        if (globalSourceId.isCorrupted()) return std::nullopt;

        return NDWidgetSourceDetails::Config::FullWidgetSourceRecord{
            id,
            globalSourceId.value,
            query.value(1).toString()
        };
    }

    // 2. Contributor
    inline std::optional<NDWidgetSourceDetails::Config::FullWidgetContributorRecord> getWidgetContributor(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT source_id, name
            FROM widget_contributor 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWidgetContributor query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto sourceId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!sourceId) return std::nullopt;

        return NDWidgetSourceDetails::Config::FullWidgetContributorRecord{
            id,
            *sourceId,
            query.value(1).toString()
        };
    }
    inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetContributorRecord>> getAllWidgetContributors(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWidgetSourceDetails::Config::FullWidgetContributorRecord> list;
        
        query.prepare(R"(
            SELECT id, name
            FROM widget_contributor 
            WHERE source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWidgetContributors query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            QString name =  query.value(1).toString();

            if (!id) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWidgetSourceDetails::Config::FullWidgetContributorRecord{
                *id,
                sourceId,
                name
            });
        }
        return list;
    }

    // 3. Type
    template<NDConcepts::ByteConvertible Metadata>
    inline std::optional<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>> getWidgetType(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, name, metadata
            FROM widget_type 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWidgetType query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        const NDHelpers::NullableField<Metadata> metadata = NDHelpers::parseNullableByteConvertible<Metadata>(query.value(2));
        if (metadata.isCorrupted()) return std::nullopt;

        return NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>{
            id,
            *contributorId,
            query.value(1).toString(),
            metadata.value
        };
    }
    template<NDConcepts::ByteConvertible Metadata>
    inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>>> getContributorWidgetTypes(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>> list;
        
        query.prepare(R"(
            SELECT id, name, metadata
            FROM widget_type 
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWidgetTypes query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id =       Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            QString name =        query.value(1).toString();

            const NDHelpers::NullableField<Metadata> metadata = NDHelpers::parseNullableByteConvertible<Metadata>(query.value(2));

            if (!id || metadata.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>{
                *id,
                contributorId,
                name,
                metadata.value
            });
        }
        return list;
    }
    template<NDConcepts::ByteConvertible Metadata>
    inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>>> getAllWidgetTypes(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>> list;

        query.prepare(R"(
            SELECT t.id, t.contributor_id, t.name, t.metadata
            FROM widget_type t
            INNER JOIN widget_contributor c ON t.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWidgetTypes query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id =            Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            QString name =             query.value(2).toString();

            const NDHelpers::NullableField<Metadata> metadata = NDHelpers::parseNullableByteConvertible<Metadata>(query.value(3));

            if (!id || !contributorId || metadata.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>{
                *id,
                *contributorId,
                name,
                metadata.value
            });
        }
        return list;
    }

    // 4. Data
    template<NDConcepts::ByteConvertible Data>
    inline std::optional<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>> getWidgetData(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, name, data
            FROM widget_data 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWidgetData query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        const NDHelpers::NullableField<Data> data = NDHelpers::parseNullableByteConvertible<Data>(query.value(2));
        if (data.isCorrupted()) return std::nullopt;

        return NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>{
            id,
            *contributorId,
            query.value(1).toString(),
            data.value
        };
    }
    template<NDConcepts::ByteConvertible Data>
    inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>>> getContributorWidgetData(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>> list;

        query.prepare(R"(
            SELECT id, name, data
            FROM widget_data 
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWidgetData query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id =   Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            QString name =    query.value(1).toString();

            const NDHelpers::NullableField<Data> data = NDHelpers::parseNullableByteConvertible<Data>(query.value(2));

            if (!id || data.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>{
                *id,
                contributorId,
                name,
                data.value
            });
        }
        return list;
    }
    template<NDConcepts::ByteConvertible Data>
    inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>>> getAllWidgetData(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>> list;

        query.prepare(R"(
            SELECT d.id, d.contributor_id, d.name, d.data
            FROM widget_data d
            INNER JOIN widget_contributor c ON d.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWidgetData query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id =            Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            QString name =             query.value(2).toString();

            const NDHelpers::NullableField<Data> data = NDHelpers::parseNullableByteConvertible<Data>(query.value(3));

            if (!id || !contributorId || data.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>{
                *id,
                *contributorId,
                name,
                data.value
            });
        }
        return list;
    }
}