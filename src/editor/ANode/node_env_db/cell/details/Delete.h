#pragma once

#include "../../../../Utility/Utility.h"

namespace NDCellDetails::Delete {

    inline bool remove(QSqlDatabase& db, const muuid::uuid& ID) {
        QSqlQuery query(db);
        query.prepare("DELETE FROM node_cells WHERE cell_id = :id;");
        query.bindValue(":id", Utility::UUID::uuidToBytes(ID));
    
        if (!query.exec()) {
            qWarning() << "Failed to remove cell:" << query.lastError().text();
            return false;
        }
        return true;
    }
}