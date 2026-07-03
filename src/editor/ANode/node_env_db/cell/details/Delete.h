#pragma once

#include "../../../../Utility/Utility.h"

namespace NDCellDetails::Delete {

    inline bool remove(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare("DELETE FROM node_cells WHERE cell_id = :id;");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));
    
        if (!query.exec()) {
            qWarning() << "Failed to remove cell:" << query.lastError().text();
            return false;
        }
        return true;
    }
}