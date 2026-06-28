#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDPinDetails::Read {
	
    inline std::optional<NDPinDetails::Config::FullPinRecord> get(QSqlQuery& query, const muuid::uuid& id, const bool continueAtFail = false) {
        const auto pinData = getPin(query, id);
        if (!pinData) return std::nullopt;

        const auto allowFlows = getAllowFlows(query, id, continueAtFail);
        if (!allowFlows) return std::nullopt;

        const auto allowTypes = getAllowTypes(query, id, continueAtFail);
        if (!allowTypes) return std::nullopt;

        return (NDPinDetails::Config::FullPinRecord{
            pinData->id,
            pinData->contributorId,
            pinData->flowId,
            pinData->typeId,
            pinData->styleId,
            *allowFlows,
            *allowTypes
        });
    }
    inline std::optional<NDPinDetails::Config::PinRecord> getPin(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, flow_id, type_id, style_id 
            FROM pin 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec() || !query.next()) return std::nullopt;

        auto parseNullableUUID = [](const QVariant& variant) -> std::optional<std::optional<muuid::uuid>> {
            if (variant.isNull())
                return std::optional<muuid::uuid>{};

            auto parsed = Utility::UUID::bytesToUuid(variant.toByteArray());
            if (!parsed) 
                return std::nullopt;

            return parsed;
        };

        auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        auto flowOpt = parseNullableUUID(query.value(1));
        if (!flowOpt) return std::nullopt;

        auto typeOpt = parseNullableUUID(query.value(2));
        if (!typeOpt) return std::nullopt; 

        auto styleOpt = parseNullableUUID(query.value(3));
        if (!styleOpt) return std::nullopt; 

        return NDPinDetails::Config::PinRecord{
            id,
            *contributorId,
            *flowOpt,
            *typeOpt,
            *styleOpt
        };
    }
    inline std::optional<QList<muuid::uuid>> getAllowFlows(QSqlQuery& query, const muuid::uuid& pinId, const bool continueAtFail = false) {
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
    inline std::optional<QList<muuid::uuid>> getAllowTypes(QSqlQuery& query, const muuid::uuid& pinId, const bool continueAtFail = false) {
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