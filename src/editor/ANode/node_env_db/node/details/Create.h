#pragma once

#include "../../../../Utility/Utility.h"

namespace NDNodeDetails::Create {

    inline std::optional<muuid::uuid> create(QSqlQuery& query, const QString& title, const short rowNum, const short colNum, const QPointF pos = { 0.0, 0.0 }) {
        query.prepare(R"(
            INSERT INTO nodes (node_id, title, row_num, col_num, canvas_x, canvas_y, canvas_w, canvas_h)
            VALUES (:node_id, :title, :row, :col, :x, :y, -1.0, -1.0);
        )");
        muuid::uuid newNodeId = muuid::uuid::generate_unix_time_based();

        query.bindValue(":node_id", Utility::UUID::uuidToBytes(newNodeId));
        query.bindValue(":title", title);
        query.bindValue(":row", rowNum);
        query.bindValue(":col", colNum);
        query.bindValue(":x", pos.x());
        query.bindValue(":y", pos.y());

        if (!query.exec()) {
            qWarning() << "Failed to execute Node creation query:" << query.lastError().text();
            return std::nullopt;
        }
        return newNodeId;
    }
}