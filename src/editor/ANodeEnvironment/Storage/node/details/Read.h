#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDNodeDetails::Read {

    // 1. Node Core
    inline std::optional<NDNodeDetails::Config::FullNodeCoreRecord> getNodeCore(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, type_id, data_id, name, default_row_num, default_col_num, default_node_w, default_node_h
            FROM node_core 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node core:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> typeId = NDHelpers::parseNullableUUID(query.value(1));
        if (typeId.isCorrupted()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> dataId = NDHelpers::parseNullableUUID(query.value(2));
        if (dataId.isCorrupted()) return std::nullopt;

        return NDNodeDetails::Config::FullNodeCoreRecord{
            id,
            *contributorId,
            typeId.value,
            dataId.value,
            query.value(3).toString(),
            static_cast<short>(query.value(4).toInt()),
            static_cast<short>(query.value(5).toInt()),
            query.value(6).toReal(),
            query.value(7).toReal()
        };
    }

    inline std::optional<QList<NDNodeDetails::Config::FullNodeCoreRecord>> getContributorNodeCores(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = true) {
        QList<NDNodeDetails::Config::FullNodeCoreRecord> list;

        query.prepare(R"(
            SELECT id, type_id, data_id, name, default_row_num, default_col_num, default_node_w, default_node_h
            FROM node_core 
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node cores:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const NDHelpers::NullableField<muuid::uuid> typeId = NDHelpers::parseNullableUUID(query.value(1));
            const NDHelpers::NullableField<muuid::uuid> dataId = NDHelpers::parseNullableUUID(query.value(2));

            if (!id || typeId.isCorrupted() || dataId.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDNodeDetails::Config::FullNodeCoreRecord{
                *id,
                contributorId,
                typeId.value,
                dataId.value,
                query.value(3).toString(),
                static_cast<short>(query.value(4).toInt()),
                static_cast<short>(query.value(5).toInt()),
                query.value(6).toReal(),
                query.value(7).toReal()
                });
        }
        return list;
    }

    inline std::optional<QList<NDNodeDetails::Config::FullNodeCoreRecord>> getAllNodeCores(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = true) {
        QList<NDNodeDetails::Config::FullNodeCoreRecord> list;

        query.prepare(R"(
            SELECT nc.id, nc.contributor_id, nc.type_id, nc.data_id, nc.name, nc.default_row_num, nc.default_col_num, nc.default_node_w, nc.default_node_h
            FROM node_core nc
            INNER JOIN node_contributor c ON nc.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node cores:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const NDHelpers::NullableField<muuid::uuid> typeId = NDHelpers::parseNullableUUID(query.value(2));
            const NDHelpers::NullableField<muuid::uuid> dataId = NDHelpers::parseNullableUUID(query.value(3));

            if (!id || !contributorId || typeId.isCorrupted() || dataId.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDNodeDetails::Config::FullNodeCoreRecord{
                *id,
                *contributorId,
                typeId.value,
                dataId.value,
                query.value(4).toString(),
                static_cast<short>(query.value(5).toInt()),
                static_cast<short>(query.value(6).toInt()),
                query.value(7).toReal(),
                query.value(8).toReal()
                });
        }
        return list;
    }

    // 2. Node
    template<NDConcepts::ByteConvertible State>
    inline std::optional<NDNodeDetails::Config::FullNodeRecord<State>> getNode(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT core_id, name, row_num, col_num, canvas_x, canvas_y, node_w, node_h, state
            FROM node 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to fetch node:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto coreId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!coreId) return std::nullopt;

        const NDHelpers::NullableField<State> state = NDHelpers::parseNullableByteConvertible<State>(query.value(8));
        if (state.isCorrupted()) return std::nullopt;

        return NDNodeDetails::Config::FullNodeRecord<State>{
            id,
                * coreId,
                query.value(1).toString(),
                static_cast<short>(query.value(2).toInt()),
                static_cast<short>(query.value(3).toInt()),
                QPointF(query.value(4).toReal(), query.value(5).toReal()),
                query.value(6).toReal(),
                query.value(7).toReal(),
                state.value
        };
    }

    template<NDConcepts::ByteConvertible State>
    inline std::optional<QList<NDNodeDetails::Config::FullNodeRecord<State>>> getContributorNodes(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = true) {
        QList<NDNodeDetails::Config::FullNodeRecord<State>> list;

        query.prepare(R"(
            SELECT n.id, n.core_id, n.name, n.row_num, n.col_num, n.canvas_x, n.canvas_y, n.node_w, n.node_h, n.state
            FROM node n
            INNER JOIN node_core nc ON n.core_id = nc.id
            WHERE nc.contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qWarning() << "Failed to fetch nodes:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const NDHelpers::NullableField<State> state = NDHelpers::parseNullableByteConvertible<State>(query.value(9));

            if (!id || !coreId || state.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDNodeDetails::Config::FullNodeRecord<State>{
                *id,
                    * coreId,
                    query.value(2).toString(),
                    static_cast<short>(query.value(3).toInt()),
                    static_cast<short>(query.value(4).toInt()),
                    QPointF(query.value(5).toReal(), query.value(6).toReal()),
                    query.value(7).toReal(),
                    query.value(8).toReal(),
                    state.value
            });
        }
        return list;
    }

    template<NDConcepts::ByteConvertible State>
    inline std::optional<QList<NDNodeDetails::Config::FullNodeRecord<State>>> getAllNodes(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = true) {
        QList<NDNodeDetails::Config::FullNodeRecord<State>> list;

        query.prepare(R"(
            SELECT n.id, n.core_id, n.name, n.row_num, n.col_num, n.canvas_x, n.canvas_y, n.node_w, n.node_h, n.state
            FROM node n
            INNER JOIN node_core nc ON n.core_id = nc.id
            INNER JOIN node_contributor c ON nc.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qWarning() << "Failed to fetch nodes:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const NDHelpers::NullableField<State> state = NDHelpers::parseNullableByteConvertible<State>(query.value(9));

            if (!id || !coreId || state.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDNodeDetails::Config::FullNodeRecord<State>{
                *id,
                    * coreId,
                    query.value(2).toString(),
                    static_cast<short>(query.value(3).toInt()),
                    static_cast<short>(query.value(4).toInt()),
                    QPointF(query.value(5).toReal(), query.value(6).toReal()),
                    query.value(7).toReal(),
                    query.value(8).toReal(),
                    state.value
            });
        }
        return list;
    }
}