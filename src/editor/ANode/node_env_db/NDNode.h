#pragma once

#include "../../Utility/Utility.h"

namespace NDNode {

    bool createTable(QSqlQuery& query) {
        QString createNodesTable = R"(
            CREATE TABLE IF NOT EXISTS nodes (
                node_id BLOB PRIMARY KEY,
                title TEXT NOT NULL,
                row_num SMALLINT NOT NULL,
                col_num SMALLINT NOT NULL,
                canvas_x REAL DEFAULT 0.0,
                canvas_y REAL DEFAULT 0.0,
                canvas_w REAL DEFAULT -1.0,
                canvas_h REAL DEFAULT -1.0
            );
        )";

        if (!query.exec(createNodesTable)) {
            qCritical() << "Failed to create nodes table:" << query.lastError().text();
            return false;
        }
        return true;
    }

    std::optional<muuid::uuid> create(QSqlQuery& query, const QString& title, const short rowNum, const short colNum, const QPointF pos = { 0.0, 0.0 }) {
        query.prepare(R"(
            INSERT INTO nodes (node_id, title, row_num, col_num, canvas_x, canvas_y, canvas_w, canvas_h)
            VALUES (:node_id, :title, :row, :col, :x, :y, -1.0, -1.0);
        )");
        muuid::uuid newNodeId = muuid::uuid::generate_unix_time_based();
        query.bindValue(":node_id", Utility::uuid::uuidToBytes(newNodeId));
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

    void remove(QSqlDatabase& db, const muuid::uuid& id) {
        QSqlQuery query(db);
        query.prepare("DELETE FROM nodes WHERE node_id = :id;");
        query.bindValue(":id", Utility::uuid::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to remove node:" << query.lastError().text();
        }
    }
}