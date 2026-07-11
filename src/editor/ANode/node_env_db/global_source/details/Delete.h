#pragma once

#include "../../../../Utility/Utility.h"

namespace NDGlobalSourceDetails::Delete {

    inline bool removeGlobalSource(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            DELETE FROM global_source
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to delete global source:" << query.lastError().text();
            return false;
        }
        return true;
    }
}