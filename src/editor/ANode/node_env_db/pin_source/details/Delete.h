#pragma once

#include "../../../../Utility/Utility.h"

namespace NDPinSourceDetails::Delete {

    inline bool removeSource(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM pin_source 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete pin source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeContributor(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM pin_contributor 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete pin source contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeFlow(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM flow 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete pin flow source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeType(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM type 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete type source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeStyle(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM style 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete style source:" << query.lastError().text();
            return false;
        }
        return true;
    }
}