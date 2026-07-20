#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDNodeSourceDetails::Read {

    // 1. Node Source
    inline std::optional<NDNodeSourceDetails::Config::FullNodeSourceRecord> getNodeSource(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT global_source_id, name
            FROM node_source 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node source:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> globalSourceId = NDHelpers::parseNullableUUID(query.value(0));
        if (globalSourceId.isCorrupted()) return std::nullopt;

        return NDNodeSourceDetails::Config::FullNodeSourceRecord{
            id,
            globalSourceId.value,
            query.value(1).toString()
        };
    }
    inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeSourceRecord>> getAllNodeSources(QSqlQuery& query, const bool continueAtFail = true) {
        QList<NDNodeSourceDetails::Config::FullNodeSourceRecord> list;

        query.prepare(R"(
            SELECT id, global_source_id, name
            FROM node_source;
        )");

        if (!query.exec()) {
            qWarning() << "Failed to fetch node sources:" << query.lastError().text();
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

            list.append(NDNodeSourceDetails::Config::FullNodeSourceRecord{
                *id,
                globalSourceId.value,
                query.value(2).toString()
            });
        }
        return list;
    }

    // 2. Node Contributor
    inline std::optional<NDNodeSourceDetails::Config::FullNodeContributorRecord> getNodeContributor(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT source_id, name
            FROM node_contributor 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node contributor:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto sourceId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!sourceId) return std::nullopt;

        return NDNodeSourceDetails::Config::FullNodeContributorRecord{
            id,
            *sourceId,
            query.value(1).toString()
        };
    }
    inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeContributorRecord>> getAllNodeContributors(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = true) {
        QList<NDNodeSourceDetails::Config::FullNodeContributorRecord> list;

        query.prepare(R"(
            SELECT id, name
            FROM node_contributor 
            WHERE source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node contributors:" << query.lastError().text();
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

            list.append(NDNodeSourceDetails::Config::FullNodeContributorRecord{
                *id,
                sourceId,
                query.value(1).toString()
            });
        }
        return list;
    }

    // 3. Node Type
    inline std::optional<NDNodeSourceDetails::Config::FullNodeTypeRecord> getNodeType(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, name, metadata
            FROM node_type 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node type:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        return NDNodeSourceDetails::Config::FullNodeTypeRecord{
            id,
            *contributorId,
            query.value(1).toString(),
            NDHelpers::extractRawBytes(query.value(2))
        };
    }
    inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeTypeRecord>> getContributorNodeTypes(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = true) {
        QList<NDNodeSourceDetails::Config::FullNodeTypeRecord> list;

        query.prepare(R"(
            SELECT id, name, metadata
            FROM node_type 
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node type:" << query.lastError().text();
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

            list.append(NDNodeSourceDetails::Config::FullNodeTypeRecord{
                *id,
                contributorId,
                query.value(1).toString(),
                NDHelpers::extractRawBytes(query.value(2))
            });
        }
        return list;
    }
    inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeTypeRecord>> getAllNodeTypes(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = true) {
        QList<NDNodeSourceDetails::Config::FullNodeTypeRecord> list;

        query.prepare(R"(
            SELECT nt.id, nt.contributor_id, nt.name, nt.metadata
            FROM node_type nt
            INNER JOIN node_contributor c ON nt.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node type:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            if (!id || !contributorId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDNodeSourceDetails::Config::FullNodeTypeRecord{
                *id,
                *contributorId,
                query.value(2).toString(),
                NDHelpers::extractRawBytes(query.value(2))
            });
        }
        return list;
    }

    // 4. Node Data
    inline std::optional<NDNodeSourceDetails::Config::FullNodeDataRecord> getNodeData(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, name, data
            FROM node_data 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node data:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        return NDNodeSourceDetails::Config::FullNodeDataRecord{
            id,
            *contributorId,
            query.value(1).toString(),
            NDHelpers::extractRawBytes(query.value(2))
        };
    }
    inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeDataRecord>> getContributorNodeData(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = true) {
        QList<NDNodeSourceDetails::Config::FullNodeDataRecord> list;

        query.prepare(R"(
            SELECT id, name, data
            FROM node_data 
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node data:" << query.lastError().text();
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

            list.append(NDNodeSourceDetails::Config::FullNodeDataRecord{
                *id,
                contributorId,
                query.value(1).toString(),
                NDHelpers::extractRawBytes(query.value(2))
            });
        }
        return list;
    }
    inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeDataRecord>> getAllNodeData(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = true) {
        QList<NDNodeSourceDetails::Config::FullNodeDataRecord> list;

        query.prepare(R"(
            SELECT nd.id, nd.contributor_id, nd.name, nd.data
            FROM node_data nd
            INNER JOIN node_contributor c ON nd.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node data:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            if (!id || !contributorId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDNodeSourceDetails::Config::FullNodeDataRecord{
                *id,
                *contributorId,
                query.value(2).toString(),
                NDHelpers::extractRawBytes(query.value(3))
            });
        }
        return list;
    }
}