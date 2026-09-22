#pragma once

#include "../../../../Utility/Utility.h"

namespace NDWireDetails::Delete {

    inline bool removeWireCore(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM wire_core 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete wire core:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeWireTemporary(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM wire_temporary 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete wire temporary:" << query.lastError().text();
            return false;
        }
        return true;
    }

    inline bool removeWire(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM wire 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete wire:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeSymmetricWire(QSqlQuery& query, const muuid::uuid& pinTypeId) {
        query.prepare(R"(
            DELETE FROM symmetric_wire_registry 
            WHERE pin_type_id = :pin_type_id;
        )");
        query.bindValue(":pin_type_id", Utility::UUID::uuidToBytes(pinTypeId));

        if (!query.exec()) {
            qCritical() << "Failed to delete symmetric wire:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeAsymmetricWire(QSqlQuery& query, const muuid::uuid& originPinTypeId, const muuid::uuid& tragetPinTypeId) {
        query.prepare(R"(
            DELETE FROM asymmetric_wire_registry 
            WHERE origin_pin_type_id = :origin_pin_type_id AND target_pin_type_id = :target_pin_type_id;
        )");
        query.bindValue(":origin_pin_type_id", Utility::UUID::uuidToBytes(originPinTypeId));
        query.bindValue(":target_pin_type_id", Utility::UUID::uuidToBytes(tragetPinTypeId));

        if (!query.exec()) {
            qCritical() << "Failed to delete asymmetric wire:" << query.lastError().text();
            return false;
        }
        return true;
    }
}