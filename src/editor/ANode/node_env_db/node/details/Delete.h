#pragma once

#include "../../../../Utility/Utility.h"

namespace NDNodeDetails::Delete {

    inline bool remove(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare("DELETE FROM nodes WHERE id = :id;");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to remove node:" << query.lastError().text();
            return false;
        }
        return true;
    }
}