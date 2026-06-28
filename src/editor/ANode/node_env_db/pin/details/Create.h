#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDPinDetails::Create {

    inline std::optional<muuid::uuid> create(
        QSqlQuery& query,
        const NDPinDetails::Config::CreatePinRecord& newPin
    ) {
        muuid::uuid pinId = muuid::uuid::generate_unix_time_based();
        query.prepare(R"(
            INSERT INTO pin (id, contributor_id, flow_id, type_id, style_id)
            VALUES (:id, :contributor_id, :flow, :type, :style);
        )");

        query.bindValue(":id",    Utility::UUID::uuidToBytes(pinId));
        query.bindValue(":id",    Utility::UUID::uuidToBytes(newPin.contributorId));
        query.bindValue(":flow",  newPin.flowId  ? Utility::UUID::uuidToBytes(*newPin.flowId)  : QVariant());
        query.bindValue(":type",  newPin.typeId  ? Utility::UUID::uuidToBytes(*newPin.typeId)  : QVariant());
        query.bindValue(":style", newPin.styleId ? Utility::UUID::uuidToBytes(*newPin.styleId) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert pin:" << query.lastError().text();
            return std::nullopt;
        }

        if (!createAllowFlows(query, pinId, newPin.allowFlowList)) return std::nullopt;
        if (!createAllowTypes(query, pinId, newPin.allowTypeList)) return std::nullopt;

        return pinId;
    }
    inline bool createAllowFlows(QSqlQuery& query, const muuid::uuid pinId, const QList<muuid::uuid>& allowedFlows) {
        if (!allowedFlows.isEmpty()) {
            query.prepare(R"(
                INSERT INTO pin_allow_flow (pin_id, flow_id)
                VALUES (:pin_id, :flow_id);
            )");

            const auto pinBytes = Utility::UUID::uuidToBytes(pinId);
            for (const auto& fId : allowedFlows) {
                query.bindValue(":pin_id", pinBytes);
                query.bindValue(":flow_id", Utility::UUID::uuidToBytes(fId));

                if (!query.exec()) {
                    qCritical() << "Failed to insert flow to allow list:" << query.lastError().text();
                    return false;
                }
            }
        }
        return true;
    }
    inline bool createAllowTypes(QSqlQuery& query, const muuid::uuid pinId, const QList<muuid::uuid>& allowedTypes) {
        if (!allowedTypes.isEmpty()) {
            query.prepare(R"(
                INSERT INTO pin_allow_type (pin_id, type_id)
                VALUES (:pin_id, :type_id);
            )");

            const auto pinBytes = Utility::UUID::uuidToBytes(pinId);
            for (const auto& tId : allowedTypes) {
                query.bindValue(":pin_id", pinBytes);
                query.bindValue(":type_id", Utility::UUID::uuidToBytes(tId));

                if (!query.exec()) {
                    qCritical() << "Failed to insert type to allow list:" << query.lastError().text();
                    return false;
                }
            }
        }
        return true;
    }
}