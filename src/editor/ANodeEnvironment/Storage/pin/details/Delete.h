#pragma once

#include "../../../../Utility/Utility.h"

namespace NDPinDetails::Delete {
	
    inline bool removePinCore(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM pin_core 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete pin core:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removePin(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM pin 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete pin:" << query.lastError().text();
            return false;
        }
        return true;
    }

    inline bool removeAllowFlow(QSqlQuery& query, const muuid::uuid& pinId) {
        query.prepare(R"(
            DELETE FROM pin_allow_flow 
            WHERE pin_id = :pin_id;
        )");
        query.bindValue(":pin_id", Utility::UUID::uuidToBytes(pinId));

        if (!query.exec()) {
            qCritical() << "Failed to delete allowed flow:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeAllowFlow(QSqlQuery& query, const muuid::uuid& pinId, const muuid::uuid& flowId) {
        query.prepare(R"(
            DELETE FROM pin_allow_flow 
            WHERE pin_id = :pin_id AND flow_id = :flow_id;
        )");
        query.bindValue(":pin_id",  Utility::UUID::uuidToBytes(pinId));
        query.bindValue(":flow_id", Utility::UUID::uuidToBytes(flowId));

        if (!query.exec()) {
            qCritical() << "Failed to delete allowed flows:" << query.lastError().text();
            return false;
        }
        return true;
    }

    inline bool removeAllowType(QSqlQuery& query, const muuid::uuid& pinId) {
        query.prepare(R"(
            DELETE FROM pin_allow_type 
            WHERE pin_id = :pin_id;
        )");
        query.bindValue(":pin_id", Utility::UUID::uuidToBytes(pinId));

        if (!query.exec()) {
            qCritical() << "Failed to delete allowed type:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeAllowType(QSqlQuery& query, const muuid::uuid& pinId, const muuid::uuid& typeId) {
        query.prepare(R"(
            DELETE FROM pin_allow_type 
            WHERE pin_id = :pin_id AND type_id = :type_id;
        )");
        query.bindValue(":pin_id",  Utility::UUID::uuidToBytes(pinId));
        query.bindValue(":type_id", Utility::UUID::uuidToBytes(typeId));

        if (!query.exec()) {
            qCritical() << "Failed to delete allowed types:" << query.lastError().text();
            return false;
        }
        return true;
    }
}