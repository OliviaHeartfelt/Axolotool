#pragma once

#include "NDConfig.h"
#include "../../Utility/Utility.h"

namespace NDCell {

    bool createTable(QSqlQuery& query) {
        QString createCellsTable = R"(
            CREATE TABLE IF NOT EXISTS node_cells (
                cell_id BLOB PRIMARY KEY,
                node_id BLOB,
                layout_row SMALLINT NOT NULL,
                layout_col SMALLINT NOT NULL,
                layout_row_span SMALLINT NOT NULL,
                layout_col_span SMALLINT NOT NULL,
                cell_type TEXT NOT NULL,
                FOREIGN KEY(node_id) REFERENCES nodes(node_id) ON DELETE CASCADE,
                UNIQUE(node_id, layout_row, layout_col)
            );
        )"

        if (!query.exec(createCellsTable)) {
            qCritical() << "Failed to create node_cells table:" << query.lastError().text();
            return false;
        }
        return true;
    }

    bool isCellAvailable(QSqlDatabase& db, const muuid::uuid& nodeId, const short row, const short col, const short rowSpan, const short colSpan) {
        QSqlQuery query(db);
        query.prepare(R"(
            SELECT 1 FROM node_cells
            WHERE node_id = :node_id
              AND :new_row < (layout_row + layout_row_span)
              AND (:new_row + :new_row_span) > layout_row
              AND :new_col < (layout_col + layout_col_span)
              AND (:new_col + :new_col_span) > layout_col
            LIMIT 1;
        )");
        query.bindValue(":node_id", Utility::uuid::uuidToBytes(nodeId));
        query.bindValue(":new_row", row);
        query.bindValue(":new_row_span", rowSpan);
        query.bindValue(":new_col", col);
        query.bindValue(":new_col_span", colSpan);

        return !(query.exec() && query.next());
    }

    std::optional<muuid::uuid> create(QSqlDatabase& db, QSqlQuery& query, const muuid::uuid& nodeId, const NDConfig::CellSpawnInfo& cell, bool overrideOnCollision = false) {
        if (!isCellAvailable(db, nodeId, cell.row, cell.col, cell.rowSpan, cell.colSpan)) {
            
            if (!overrideOnCollision) {
                qWarning() << "Cell insertion rejected: Space is occupied."
                return std::nullopt;
            }

            query.prepare(R"(
                DELETE FROM node_cells
                WHERE node_id = :node_id
                  AND :new_row < (layout_row + layout_row_span)
                  AND (:new_row + :new_row_span) > layout_row
                  AND :new_col < (layout_col + layout_col_span)
                  AND (:new_col + :new_col_span) > layout_col;
            )");
            query.bindValue(":node_id", Utility::uuid::uuidToBytes(nodeId));
            query.bindValue(":new_row", cell.row);
            query.bindValue(":new_row_span", cell.rowSpan);
            query.bindValue(":new_col", cell.col);
            query.bindValue(":new_col_span", cell.colSpan);

            if (!query.exec()) {
                qWarning() << "Failed to evict overlapping cells during overwrite:" << query.lastError().text();
                return std::nullopt;
            }
        }

        query.prepare(R"(
            INSERT INTO node_cells (cell_id, node_id, layout_row, layout_col, layout_row_span, layout_col_span, cell_type)
            VALUES (:cell_id, :node_id, :row, :col, :row_span, :col_span, :type);
        )");
        muuid::uuid newCellId = muuid::uuid::generate_unix_time_based();
        query.bindValue(":cell_id", Utility::uuid::uuidToBytes(newCellId));
        query.bindValue(":node_id", Utility::uuid::uuidToBytes(nodeId));
        query.bindValue(":row", cell.row);
        query.bindValue(":col", cell.col);
        query.bindValue(":row_span", cell.rowSpan);
        query.bindValue(":col_span", cell.colSpan);
        query.bindValue(":type", cell.cellType);

        if (!query.exec()) {
            qWarning() << "Failed to execute Cell creation query:" << query.lastError().text();
            return std::nullopt;
        }
        return newCellId;
    }

    void remove(QSqlDatabase& db, const muuid::uuid& ID) {
        QSqlQuery query(db);
        query.prepare("DELETE FROM node_cells WHERE cell_id = :id;");
        query.bindValue(":id", Utility::uuid::uuidToBytes(ID));

        if (!query.exec()) {
            qWarning() << "Failed to remove cell:" << query.lastError().text();
        }
    }
}