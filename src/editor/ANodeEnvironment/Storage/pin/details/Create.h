#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDPinDetails::Create {

    inline bool create( QSqlQuery& query, const NDPinDetails::Config::CreatePinRecord& newPin) {
        query.prepare(R"(
            INSERT INTO pin (id,  contributor_id,  flow_id,  type_id,  style_id)
            VALUES (        :id, :contributor_id, :flow_id, :type_id, :style_id);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newPin.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newPin.contributorId));
        query.bindValue(":flow_id",        newPin.flowId ? Utility::UUID::uuidToBytes(*newPin.flowId) : QVariant());
        query.bindValue(":type_id",        newPin.typeId ? Utility::UUID::uuidToBytes(*newPin.typeId) : QVariant());
        query.bindValue(":style_id",       newPin.styleId ? Utility::UUID::uuidToBytes(*newPin.styleId) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert pin:" << query.lastError().text();
            return false;
        }

        if (!createAllowFlows(query, newPin.id, newPin.allowFlowList)) return false;
        if (!createAllowTypes(query, newPin.id, newPin.allowTypeList)) return false;

        return true;
    }
    inline bool createAllowFlows(QSqlQuery& query, const muuid::uuid pinId, const QList<muuid::uuid>& newAllowedFlows) {
        if (!newAllowedFlows.isEmpty()) {
            query.prepare(R"(
                INSERT INTO pin_allow_flow (pin_id,  flow_id)
                VALUES (                   :pin_id, :flow_id);
            )");

            const auto pinBytes = Utility::UUID::uuidToBytes(pinId);
            for (const auto& fId : newAllowedFlows) {
                query.bindValue(":pin_id",  pinBytes);
                query.bindValue(":flow_id", Utility::UUID::uuidToBytes(fId));

                if (!query.exec()) {
                    qCritical() << "Failed to insert flow to allow list:" << query.lastError().text();
                    return false;
                }
            }
        }
        return true;
    }
    inline bool createAllowTypes(QSqlQuery& query, const muuid::uuid pinId, const QList<muuid::uuid>& newAllowedTypes) {
        if (!newAllowedTypes.isEmpty()) {
            query.prepare(R"(
                INSERT INTO pin_allow_type (pin_id, type_id)
                VALUES (:pin_id, :type_id);
            )");

            const auto pinBytes = Utility::UUID::uuidToBytes(pinId);
            for (const auto& tId : newAllowedTypes) {
                query.bindValue(":pin_id",  pinBytes);
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