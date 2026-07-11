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

    inline bool removeWirePins(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM wire_pins 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete wire pins:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeWireWidgets(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM wire_widgets 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete wire widgets:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeWireArbitrary(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM wire_arbitrary 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete wire arbitrary:" << query.lastError().text();
            return false;
        }
        return true;
    }
}