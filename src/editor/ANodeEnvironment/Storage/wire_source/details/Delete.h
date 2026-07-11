#pragma once

#include "../../../../Utility/Utility.h"

namespace NDWireSourceDetails::Delete {

    inline bool removeWireSource(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM wire_source
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete wire source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeWireContributor(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM wire_contributor
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete wire contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeWireStyle(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM wire_style
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete wire style:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeWireData(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM wire_data
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete wire data:" << query.lastError().text();
            return false;
        }
        return true;
    }
}