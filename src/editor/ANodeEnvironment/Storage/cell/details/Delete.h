#pragma once

#include "../../../../Utility/Utility.h"

namespace NDCellDetails::Delete {

    inline bool removeCell(QSqlQuery& query, const muuid::uuid& id) {
        const QByteArray cellBytesId = Utility::UUID::uuidToBytes(id);

        query.prepare("DELETE FROM widget WHERE id = (SELECT widget_id FROM cell WHERE id = :id);");
        query.bindValue(":id", cellBytesId);
        if (!query.exec()) {
            qWarning() << "Failed to clean up associated widget for cell:" << query.lastError().text();
            return false;
        }

        query.prepare("DELETE FROM cell WHERE id = :id;");
        query.bindValue(":id", cellBytesId);
        if (!query.exec()) {
            qWarning() << "Failed to remove cell:" << query.lastError().text();
            return false;
        }

        return true;
    }
    inline bool removeCellOrigin(QSqlQuery& query, const muuid::uuid& id) {
        const QByteArray cellBytesId = Utility::UUID::uuidToBytes(id);

        query.prepare("DELETE FROM cell_origin WHERE id = :id;");
        query.bindValue(":id", cellBytesId);
        if (!query.exec()) {
            qWarning() << "Failed to remove cell origin:" << query.lastError().text();
            return false;
        }

        return true;
    }
}