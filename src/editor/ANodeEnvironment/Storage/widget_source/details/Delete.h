#pragma once

#include "../../../../Utility/Utility.h"

namespace NDWidgetSourceDetails::Delete {

    inline bool removeWidgetSource(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM widget_source
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete widget source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeWidgetContributor(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM widget_contributor 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete widget contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeWidgetType(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM widget_type 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete widget type:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeWidgetData(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM widget_data 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete widget data:" << query.lastError().text();
            return false;
        }
        return true;
    }
}