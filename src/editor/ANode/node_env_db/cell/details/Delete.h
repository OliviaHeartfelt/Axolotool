#pragma once

#include "../../../../Utility/Utility.h"

namespace NDCellDetails::Delete {

    inline bool remove(QSqlQuery& query, const muuid::uuid& id) {
        const QByteArray cellBytesId = Utility::UUID::uuidToBytes(id);

        query.prepare("DELETE FROM pin WHERE id = (SELECT pin_id FROM node_cells WHERE id = :id);");
        query.bindValue(":id", cellBytesId);
        if (!query.exec()) {
            qWarning() << "Failed to clean up associated pin for cell:" << query.lastError().text();
            return false;
        }

        query.prepare("DELETE FROM widget WHERE id = (SELECT widget_id FROM node_cells WHERE id = :id);");
        query.bindValue(":id", cellBytesId);
        if (!query.exec()) {
            qWarning() << "Failed to clean up associated widget for cell:" << query.lastError().text();
            return false;
        }

        query.prepare("DELETE FROM node_cells WHERE id = :id;");
        query.bindValue(":id", cellBytesId);
        if (!query.exec()) {
            qWarning() << "Failed to remove cell:" << query.lastError().text();
            return false;
        }

        return true;
    }
}