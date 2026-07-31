#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDWireDetails::Read {

    // 1. Wire Core
    inline std::optional<NDWireDetails::Config::FullWireCoreRecord> getWireCore(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, style_id, data_id, name
            FROM wire_core 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWireCore query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> styleId = NDHelpers::parseNullableUUID(query.value(1));
        if (styleId.isCorrupted()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> dataId = NDHelpers::parseNullableUUID(query.value(2));
        if (dataId.isCorrupted()) return std::nullopt;

        return NDWireDetails::Config::FullWireCoreRecord{
            id,
            *contributorId,
            styleId.value,
            dataId.value,
            query.value(3).toString()
        };
    }
    inline std::optional<QList<NDWireDetails::Config::FullWireCoreRecord>> getContributorWireCores(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireCoreRecord> list;

        query.prepare(R"(
            SELECT id, style_id, data_id, name
            FROM wire_core 
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWireCores query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());

            const NDHelpers::NullableField<muuid::uuid> styleId = NDHelpers::parseNullableUUID(query.value(1));
            const NDHelpers::NullableField<muuid::uuid> dataId = NDHelpers::parseNullableUUID(query.value(2));

            if (!id || styleId.isCorrupted() || dataId.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireCoreRecord{
                *id,
                contributorId,
                styleId.value,
                dataId.value,
                query.value(3).toString()
            });
        }
        return list;
    }
    inline std::optional<QList<NDWireDetails::Config::FullWireCoreRecord>> getAllWireCores(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireCoreRecord> list;

        query.prepare(R"(
            SELECT wc.id, wc.contributor_id, wc.style_id, wc.data_id, wc.name
            FROM wire_core wc
            INNER JOIN widget_contributor c ON wc.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWireCores query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            const NDHelpers::NullableField<muuid::uuid> styleId = NDHelpers::parseNullableUUID(query.value(2));
            const NDHelpers::NullableField<muuid::uuid> dataId = NDHelpers::parseNullableUUID(query.value(3));

            if (!id || !contributorId || styleId.isCorrupted() || dataId.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireCoreRecord{
                *id,
                *contributorId,
                styleId.value,
                dataId.value,
                query.value(4).toString()
            });
        }
        return list;
    }

    // 2. Wire Temporary
    inline std::optional<NDWireDetails::Config::FullWireTemporaryRecord> getWireTemporary(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT core_id
            FROM wire_temporary 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWireTemporary query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto coreId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!coreId) return std::nullopt;

        return NDWireDetails::Config::FullWireTemporaryRecord{
            id,
            *coreId
        };
    }
    inline std::optional<QList<NDWireDetails::Config::FullWireTemporaryRecord>> getContributorWireTemporary(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireTemporaryRecord> list;

        query.prepare(R"(
            SELECT wt.id, wt.core_id
            FROM wire_temporary wt
            INNER JOIN wire_core cr ON wt.core_id = cr.id
            WHERE cr.contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWireTemporary query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            if (!id || !coreId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireTemporaryRecord{
                *id,
                *coreId,
            });
        }
        return list;
    }
    inline std::optional<QList<NDWireDetails::Config::FullWireTemporaryRecord>> getAllWireTemporary(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireTemporaryRecord> list;

        query.prepare(R"(
            SELECT wt.id, wt.core_id
            FROM wire_temporary wt
            INNER JOIN wire_core cr ON wt.core_id = cr.id
            INNER JOIN widget_contributor c ON cr.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWireTemporary query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            if (!id || !coreId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireTemporaryRecord{
                *id,
                *coreId
            });
        }
        return list;
    }

    // 3. Wire Pins
    inline std::optional<NDWireDetails::Config::FullWireRecord> getWire(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT core_id, origin_id, target_id, origin_canvas_hint_x, origin_canvas_hint_y, target_canvas_hint_x, target_canvas_hint_y, state
            FROM wire
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWirePin query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto coreId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!coreId) return std::nullopt;

        const auto originId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
        if (!originId) return std::nullopt;

        const auto targetId = Utility::UUID::bytesToUuid(query.value(2).toByteArray());
        if (!targetId) return std::nullopt;

        return NDWireDetails::Config::FullWireRecord{
            id,
            *coreId,
            *originId,
            *targetId,
            QPointF(query.value(3).toReal(), query.value(4).toReal()),
            QPointF(query.value(5).toReal(), query.value(6).toReal()),
            NDHelpers::extractRawBytes(query.value(7))
        };
    }
    inline std::optional<QList<NDWireDetails::Config::FullWireRecord>> getContributorWires(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireRecord> list;

        query.prepare(R"(
            SELECT w.id, w.core_id, w.origin_id, w.target_id, w.origin_canvas_hint_x, w.origin_canvas_hint_y, w.target_canvas_hint_x, w.target_canvas_hint_y, w.state
            FROM wire w
            INNER JOIN wire_core cr ON w.core_id = cr.id
            WHERE cr.contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWirePins query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id =       Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId =   Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const auto originId = Utility::UUID::bytesToUuid(query.value(2).toByteArray());
            const auto targetId = Utility::UUID::bytesToUuid(query.value(3).toByteArray());

            if (!id || !coreId || !originId || !targetId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireRecord{
                *id,
                *coreId,
                *originId,
                *targetId,
                QPointF(query.value(4).toReal(), query.value(5).toReal()),
                QPointF(query.value(6).toReal(), query.value(7).toReal()),
                NDHelpers::extractRawBytes(query.value(8))
            });
        }
        return list;
    }
    inline std::optional<QList<NDWireDetails::Config::FullWireRecord>> getAllWires(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireRecord> list;

        query.prepare(R"(
            SELECT w.id, w.core_id, w.origin_id, w.target_id, w.origin_canvas_hint_x, w.origin_canvas_hint_y, w.target_canvas_hint_x, w.target_canvas_hint_y, w.state
            FROM wire w
            INNER JOIN wire_core core ON w.core_id = core.id
            INNER JOIN widget_contributor c ON core.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWirePins query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id =       Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId =   Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const auto originId = Utility::UUID::bytesToUuid(query.value(2).toByteArray());
            const auto targetId = Utility::UUID::bytesToUuid(query.value(3).toByteArray());

            if (!id || !coreId || !originId || !targetId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireRecord{
                *id,
                *coreId,
                *originId,
                *targetId,
                QPointF(query.value(4).toReal(), query.value(5).toReal()),
                QPointF(query.value(6).toReal(), query.value(7).toReal()),
                NDHelpers::extractRawBytes(query.value(8))
            });
        }
        return list;
    }

    inline std::optional<QList<NDWireDetails::Config::FullWireRecord>> getWiresInView(QSqlQuery& query, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireRecord> list;

        query.prepare(R"(
            SELECT id, core_id, origin_id, target_id, origin_canvas_hint_x, origin_canvas_hint_y, target_canvas_hint_x, target_canvas_hint_y, state
            FROM wire
        )");

        if (!query.exec()) {
            qCritical() << "Failed to execute getWiresInView query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const auto originId = Utility::UUID::bytesToUuid(query.value(2).toByteArray());
            const auto targetId = Utility::UUID::bytesToUuid(query.value(3).toByteArray());

            if (!id || !coreId || !originId || !targetId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireRecord{
                *id,
                *coreId,
                *originId,
                *targetId,
                QPointF(query.value(4).toReal(), query.value(5).toReal()),
                QPointF(query.value(6).toReal(), query.value(7).toReal()),
                NDHelpers::extractRawBytes(query.value(8))
                });
        }
        return list;
    }
}