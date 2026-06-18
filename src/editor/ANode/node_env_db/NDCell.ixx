export module NDCell;

import NDConfig;

export namespace NDCell {

    bool isCellAvailable(QSqlDatabase& db, int nodeId, short row, short col, short rowSpan, short colSpan) {
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
        query.bindValue(":node_id", nodeId);
        query.bindValue(":new_row", row);
        query.bindValue(":new_row_span", rowSpan);
        query.bindValue(":new_col", col);
        query.bindValue(":new_col_span", colSpan);

        return !(query.exec() && query.next());
    }

    bool create(QSqlDatabase& db, QSqlQuery& query, const int nodeId, const NDConfig::CellSpawnInfo& cell, bool overrideOnCollision = false) {
        if (!isCellAvailable(db, nodeId, cell.row, cell.col, cell.rowSpan, cell.colSpan)) {
            if (!overrideOnCollision) {
                qWarning() << "Cell insertion rejected: Space is occupied.";
                return false;
            }
            QSqlQuery deleteQuery(db);
            deleteQuery.prepare(R"(
                    DELETE FROM node_cells
                    WHERE node_id = :node_id
                      AND :new_row < (layout_row + layout_row_span)
                      AND (:new_row + :new_row_span) > layout_row
                      AND :new_col < (layout_col + layout_col_span)
                      AND (:new_col + :new_col_span) > layout_col;
                )");
            deleteQuery.bindValue(":node_id", nodeId);
            deleteQuery.bindValue(":new_row", cell.row);
            deleteQuery.bindValue(":new_row_span", cell.rowSpan);
            deleteQuery.bindValue(":new_col", cell.col);
            deleteQuery.bindValue(":new_col_span", cell.colSpan);

            if (!deleteQuery.exec()) {
                qWarning() << "Failed to evict overlapping cells during overwrite:" << deleteQuery.lastError().text();
                return false;
            }
        }

        query.prepare(R"(
            INSERT INTO node_cells (node_id, layout_row, layout_col, layout_row_span, layout_col_span, cell_type)
            VALUES (:node_id, :row, :col, :row_span, :col_span, :type);
        )");
        query.bindValue(":node_id", nodeId);
        query.bindValue(":row", cell.row);
        query.bindValue(":col", cell.col);
        query.bindValue(":row_span", cell.rowSpan);
        query.bindValue(":col_span", cell.colSpan);
        query.bindValue(":type", cell.cellType);

        if (!query.exec()) {
            qWarning() << "Failed to execute Cell creation query:" << query.lastError().text();
            return false;
        }
        return true;
    }

    void remove(QSqlDatabase& db, const int ID) {
        QSqlQuery query(db);
        query.prepare("DELETE FROM node_cells WHERE cell_id = :id;");
        query.bindValue(":id", ID);

        if (!query.exec()) {
            qWarning() << "Failed to remove cell:" << query.lastError().text();
        }
    }
}