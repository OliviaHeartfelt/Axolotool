#pragma once

#include "../../../../Utility/Utility.h"

namespace NDWidgetDetails::Delete {

    inline bool removeWidgetCore(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM widget_core 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete widget core:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeWidget(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM widget 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete widget:" << query.lastError().text();
            return false;
        }
        return true;
    }
}