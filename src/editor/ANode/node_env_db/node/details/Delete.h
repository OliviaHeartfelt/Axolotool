#pragma once

#include "../../../../Utility/Utility.h"

namespace NDNodeDetails::Delete {

    inline bool remove(QSqlDatabase& db, const muuid::uuid& id) {
        QSqlQuery query(db);
        query.prepare("DELETE FROM nodes WHERE node_id = :id;");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to remove node:" << query.lastError().text();
            return false;
        }
        return true;
    }
}