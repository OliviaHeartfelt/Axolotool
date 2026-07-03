#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDNodeDetails::Update {

    inline bool updateNode(QSqlQuery& query, const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeRecord& newProperties) {
        QStringList clauses;

        if (newProperties.title)  clauses.append("title = :title");
        if (newProperties.rowNum) clauses.append("row_num = :row_num");
        if (newProperties.colNum) clauses.append("col_num = :col_num");
        if (newProperties.pos)    clauses.append("canvas_x = :canvas_x, canvas_y = :canvas_y");
        if (newProperties.width)  clauses.append("canvas_w = :canvas_w");
        if (newProperties.height) clauses.append("canvas_h = :canvas_h");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE nodes SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.title)  query.bindValue(":title", *newProperties.title);
        if (newProperties.rowNum) query.bindValue(":row_num", *newProperties.rowNum);
        if (newProperties.colNum) query.bindValue(":col_num", *newProperties.colNum);
        if (newProperties.width)  query.bindValue(":canvas_w", *newProperties.width);
        if (newProperties.height) query.bindValue(":canvas_h", *newProperties.height);

        if (newProperties.pos) {
            query.bindValue(":canvas_x", newProperties.pos->x());
            query.bindValue(":canvas_y", newProperties.pos->y());
        }

        if (!query.exec()) {
            qWarning() << "Failed to update node geometry:" << query.lastError().text();
            return false;
        }
        return true;
    }
}