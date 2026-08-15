#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDPinDetails::Read {

    // Pin
    inline std::optional<NDPinDetails::Config::FullPinRecord> getPin(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT core_id
            FROM pin 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getPin query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        auto coreId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!coreId) return std::nullopt;

        return NDPinDetails::Config::FullPinRecord{
            id,
            *coreId
        };
    }
    inline std::optional<QList<NDPinDetails::Config::FullPinRecord>> getCorePins(QSqlQuery& query, const muuid::uuid& coreId, const bool continueAtFail = false) {
        QList<NDPinDetails::Config::FullPinRecord> list;
        
        query.prepare(R"(
            SELECT id
            FROM pin
            WHERE core_id = :core_id;
        )");
        query.bindValue(":core_id", Utility::UUID::uuidToBytes(coreId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getPin query:" << query.lastError().text();
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

            list.append(NDPinDetails::Config::FullPinRecord{
                *id,
                coreId
            });
        }
        return list;
    }
    inline std::optional<QList<NDPinDetails::Config::FullPinRecord>> getContributorPins(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDPinDetails::Config::FullPinRecord> list;

        query.prepare(R"(
            SELECT p.id, p.core_id
            FROM pin p
            INNER JOIN pin_core pc ON p.core_id = pc.id
            WHERE pc.contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getPin query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());

            if (!id || !coreId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDPinDetails::Config::FullPinRecord{
                *id,
                *coreId
                });
        }
        return list;
    }
    inline std::optional<QList<NDPinDetails::Config::FullPinRecord>> getAllPins(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDPinDetails::Config::FullPinRecord> list;

        query.prepare(R"(
            SELECT p.id, p.core_id
            FROM pin p
            INNER JOIN pin_core core ON p.core_id = core.id
            INNER JOIN pin_contributor ctr ON core.contributor_id = ctr.id
            WHERE ctr.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllPins query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());

            if (!id || !coreId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDPinDetails::Config::FullPinRecord{
                *id,
                *coreId
                });
        }
        return list;
    }

    // Pin Core
    inline std::optional<NDPinDetails::Config::FullPinCoreRecord> getPinCore(QSqlQuery& query, const muuid::uuid& id);
    inline std::optional<QList<muuid::uuid>> getAllowFlows(QSqlQuery& query, const muuid::uuid& pinId, const bool continueAtFail = false);
    inline std::optional<QList<muuid::uuid>> getAllowTypes(QSqlQuery& query, const muuid::uuid& pinId, const bool continueAtFail = false);
	
    inline std::optional<NDPinDetails::Config::CompletePinCore> getFullPinCore(QSqlQuery& query, const muuid::uuid& id, const bool continueAtFail = false) {
        const auto pinData = getPinCore(query, id);
        if (!pinData) return std::nullopt;

        const auto allowFlows = getAllowFlows(query, id, continueAtFail);
        if (!allowFlows) return std::nullopt;

        const auto allowTypes = getAllowTypes(query, id, continueAtFail);
        if (!allowTypes) return std::nullopt;

        return (NDPinDetails::Config::CompletePinCore{
            pinData->id,
            pinData->contributorId,
            pinData->visualFactoryId,
            pinData->flowId,
            pinData->typeId,
            pinData->styleId,
            *allowFlows,
            *allowTypes
        });
    }
    inline std::optional<NDPinDetails::Config::FullPinCoreRecord> getPinCore(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, visual_factory_id, flow_id, type_id, style_id 
            FROM pin_core 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getPinCore query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        auto visualFactoryId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
        if (!visualFactoryId) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> flow = NDHelpers::parseNullableUUID(query.value(2));
        if (flow.isCorrupted()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> type = NDHelpers::parseNullableUUID(query.value(3));
        if (type.isCorrupted()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> style = NDHelpers::parseNullableUUID(query.value(4));
        if (style.isCorrupted()) return std::nullopt;

        return NDPinDetails::Config::FullPinCoreRecord{
            id,
            *contributorId,
            *visualFactoryId,
            flow.value,
            type.value,
            style.value
        };
    }
    inline std::optional<QList<NDPinDetails::Config::FullPinCoreRecord>> getContributorPinCores(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDPinDetails::Config::FullPinCoreRecord> list;

        query.prepare(R"(
            SELECT id, visual_factory_id, flow_id, type_id, style_id 
            FROM pin_core 
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getPin query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            auto visualFactoryId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            const NDHelpers::NullableField<muuid::uuid> flow = NDHelpers::parseNullableUUID(query.value(2));
            const NDHelpers::NullableField<muuid::uuid> type = NDHelpers::parseNullableUUID(query.value(3));
            const NDHelpers::NullableField<muuid::uuid> style = NDHelpers::parseNullableUUID(query.value(4));

            if (!id || !visualFactoryId || flow.isCorrupted() || type.isCorrupted() || style.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDPinDetails::Config::FullPinCoreRecord{
                *id,
                contributorId,
                *visualFactoryId,
                flow.value,
                type.value,
                style.value
            });
        }
        return list;
    }
    inline std::optional<QList<NDPinDetails::Config::FullPinCoreRecord>> getAllPinCores(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDPinDetails::Config::FullPinCoreRecord> list;

        query.prepare(R"(
            SELECT core.id, core.contributor_id, core.visual_factory_id, core.flow_id, core.type_id, core.style_id 
            FROM pin_core core
            INNER JOIN pin_contributor ctr ON core.contributor_id = ctr.id
            WHERE ctr.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getPin query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            auto visualFactoryId = Utility::UUID::bytesToUuid(query.value(2).toByteArray());

            const NDHelpers::NullableField<muuid::uuid> flow = NDHelpers::parseNullableUUID(query.value(3));
            const NDHelpers::NullableField<muuid::uuid> type = NDHelpers::parseNullableUUID(query.value(4));
            const NDHelpers::NullableField<muuid::uuid> style = NDHelpers::parseNullableUUID(query.value(5));

            if (!id || !contributorId || !visualFactoryId || flow.isCorrupted() || type.isCorrupted() || style.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDPinDetails::Config::FullPinCoreRecord{
                *id,
                *contributorId,
                *visualFactoryId,
                flow.value,
                type.value,
                style.value
                });
        }
        return list;
    }

    inline std::optional<QList<muuid::uuid>> getAllowFlows(QSqlQuery& query, const muuid::uuid& pinId, const bool continueAtFail) {
        QList<muuid::uuid> list;

        query.prepare(R"(
            SELECT flow_id
            FROM pin_allow_flow 
            WHERE pin_id = :pin_id;
        )");
        query.bindValue(":pin_id", Utility::UUID::uuidToBytes(pinId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllowFlows query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto flowId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            if (!flowId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(*flowId);
        }
        return list;
    }
    inline std::optional<QList<muuid::uuid>> getAllowTypes(QSqlQuery& query, const muuid::uuid& pinId, const bool continueAtFail) {
        QList<muuid::uuid> list;

        query.prepare(R"(
            SELECT type_id
            FROM pin_allow_type 
            WHERE pin_id = :pin_id;
        )");
        query.bindValue(":pin_id", Utility::UUID::uuidToBytes(pinId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllowTypes query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto flowId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            if (!flowId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(*flowId);
        }
        return list;
    }
}