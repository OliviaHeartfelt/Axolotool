#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDWidgetDetails::Read {

    // 1. Widget Core
    inline std::optional<NDWidgetDetails::Config::FullWidgetCoreRecord> getWidgetCore(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, type_id, data_id
            FROM widget_core 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWidgetCore query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> typeId = NDHelpers::parseNullableUUID(query.value(1));
        if (typeId.isCorrupted()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> dataId = NDHelpers::parseNullableUUID(query.value(2));
        if (dataId.isCorrupted()) return std::nullopt;

        return NDWidgetDetails::Config::FullWidgetCoreRecord{
            id,
            *contributorId,
            typeId.value,
            dataId.value
        };
    }
    inline std::optional<QList<NDWidgetDetails::Config::FullWidgetCoreRecord>> getContributorWidgetCores(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWidgetDetails::Config::FullWidgetCoreRecord> list;

        query.prepare(R"(
            SELECT id, type_id, data_id
            FROM widget_core 
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWidgetCores query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id =     Utility::UUID::bytesToUuid(query.value(0).toByteArray());

            const NDHelpers::NullableField<muuid::uuid> typeId = NDHelpers::parseNullableUUID(query.value(1));
            const NDHelpers::NullableField<muuid::uuid> dataId = NDHelpers::parseNullableUUID(query.value(2));

            if (!id || typeId.isCorrupted() || dataId.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWidgetDetails::Config::FullWidgetCoreRecord{
                *id,
                contributorId,
                typeId.value,
                dataId.value
            });
        }
        return list;
    }
    inline std::optional<QList<NDWidgetDetails::Config::FullWidgetCoreRecord>> getAllWidgetCores(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWidgetDetails::Config::FullWidgetCoreRecord> list;

        query.prepare(R"(
            SELECT wc.id, wc.contributor_id, wc.type_id, wc.data_id
            FROM widget_core wc
            INNER JOIN widget_contributor c ON wc.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWidgetCores query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id =            Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            const NDHelpers::NullableField<muuid::uuid> typeId = NDHelpers::parseNullableUUID(query.value(2));
            const NDHelpers::NullableField<muuid::uuid> dataId = NDHelpers::parseNullableUUID(query.value(3));

            if (!id || !contributorId || typeId.isCorrupted() || dataId.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWidgetDetails::Config::FullWidgetCoreRecord{
                *id,
                *contributorId,
                typeId.value,
                dataId.value
            });
        }
        return list;
    }

    // 2. Widget
    inline std::optional<NDWidgetDetails::Config::FullWidgetRecord> getWidget(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT core_id, state, w_size, h_size
            FROM widget 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWidget query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto coreId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!coreId) return std::nullopt;

        return NDWidgetDetails::Config::FullWidgetRecord{
            id,
            *coreId,
            NDHelpers::extractRawBytes(query.value(1)),
            query.value(2).toReal(),
            query.value(3).toReal()
        };
    }

    inline std::optional<QList<NDWidgetDetails::Config::FullWidgetRecord>> getContributorWidgets(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWidgetDetails::Config::FullWidgetRecord> list;

        query.prepare(R"(
            SELECT w.id, w.core_id, w.state, w.w_size, w.h_size
            FROM widget w
            INNER JOIN widget_core core ON w.core_id = core.id
            WHERE core.contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWidgets query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id =     Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            if (!id || !coreId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWidgetDetails::Config::FullWidgetRecord{
                *id,
                *coreId,
                NDHelpers::extractRawBytes(query.value(2)),
                query.value(3).toReal(),
                query.value(4).toReal()
            });
        }
        return list;
    }

    inline std::optional<QList<NDWidgetDetails::Config::FullWidgetRecord>> getAllWidgets(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWidgetDetails::Config::FullWidgetRecord> list;

        query.prepare(R"(
            SELECT w.id, w.core_id, w.state, w.w_size, w.h_size
            FROM widget w
            INNER JOIN widget_core core ON w.core_id = core.id
            INNER JOIN widget_contributor c ON core.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWidgets query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id =     Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            if (!id || !coreId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWidgetDetails::Config::FullWidgetRecord{
                *id,
                *coreId,
                NDHelpers::extractRawBytes(query.value(2)),
                query.value(3).toReal(),
                query.value(4).toReal()
            });
        }
        return list;
    }
}