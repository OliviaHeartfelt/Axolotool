#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDHelpers.h"
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

        if (!query.exec()) {
            qCritical() << "Failed to execute getPin query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> flow = NDHelpers::parseNullableUUID(query.value(1));
        if (flow.isCorrupted()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> type = NDHelpers::parseNullableUUID(query.value(2));
        if (type.isCorrupted()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> style = NDHelpers::parseNullableUUID(query.value(3));
        if (style.isCorrupted()) return std::nullopt;

        return NDPinDetails::Config::PinRecord{
            id,
            *contributorId,
            flow.value,
            type.value,
            style.value
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