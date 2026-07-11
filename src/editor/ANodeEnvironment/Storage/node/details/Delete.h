#pragma once

#include "../../../../Utility/Utility.h"

namespace NDNodeDetails::Delete {

    inline bool removeNodeCore(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare("DELETE FROM node_core WHERE id = :id;");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to remove node core:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeNode(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare("DELETE FROM node WHERE id = :id;");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to remove node:" << query.lastError().text();
            return false;
        }
        return true;
    }
}