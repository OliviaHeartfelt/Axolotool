#pragma once

#include "../../../../Utility/Utility.h"

namespace NDPinDetails::Create {

    inline std::optional<muuid::uuid> create(
        QSqlQuery& query,
        const muuid::uuid& flowId,
        const muuid::uuid& typeId,
        const muuid::uuid& styleId,
        const QList<muuid::uuid>& allowedFlows = {},
        const QList<muuid::uuid>& allowedTypes = {})
    {
        muuid::uuid pinId = muuid::uuid::generate_unix_time_based();

        query.prepare(R"(
            INSERT INTO pin (id, flow_id, type_id, style_id)
            VALUES (:id, :flow, :type, :style);
        )");
        query.bindValue(":id",    Utility::UUID::uuidToBytes(pinId));
        query.bindValue(":flow",  Utility::UUID::uuidToBytes(flowId));
        query.bindValue(":type",  Utility::UUID::uuidToBytes(typeId));
        query.bindValue(":style", Utility::UUID::uuidToBytes(styleId));

        if (!query.exec()) {
            qCritical() << "Failed to insert pin:" << query.lastError().text();
            return std::nullopt;
        }

        if (!createAllowFlows(query, pinId, allowedFlows)) return std::nullopt;
        if (!createAllowTypes(query, pinId, allowedTypes)) return std::nullopt;

        return pinId;
    }
    inline bool createAllowFlows(QSqlQuery& query, muuid::uuid pinId, const QList<muuid::uuid>& allowedFlows) {
        if (!allowedFlows.isEmpty()) {
            query.prepare(R"(
                INSERT INTO pin_allow_flow (pin_id, flow_id)
                VALUES (:pin_id, :flow_id);
            )");

            const auto pinBytes = Utility::UUID::uuidToBytes(pinId);
            for (const auto& fId : allowedFlows) {
                query.bindValue(":pin_id", pinBytes);
                query.bindValue(":flow_id", Utility::UUID::uuidToBytes(fId));
                if (!query.exec()) return false;
            }
        }
        return true;
    }
    inline bool createAllowTypes(QSqlQuery& query, muuid::uuid pinId, const QList<muuid::uuid>& allowedTypes) {
        if (!allowedTypes.isEmpty()) {
            query.prepare(R"(
                INSERT INTO pin_allow_type (pin_id, type_id)
                VALUES (:pin_id, :type_id);
            )");

            const auto pinBytes = Utility::UUID::uuidToBytes(pinId);
            for (const auto& tId : allowedTypes) {
                query.bindValue(":pin_id", pinBytes);
                query.bindValue(":type_id", Utility::UUID::uuidToBytes(tId));
                if (!query.exec()) return false;
            }
        }
        return true;
    }
}