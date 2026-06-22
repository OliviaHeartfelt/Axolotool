#pragma once

namespace NDCellDetails::Init {

    inline bool createTable(QSqlQuery& query) {
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
        )";
    
        if (!query.exec(createCellsTable)) {
            qCritical() << "Failed to create node_cells table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}