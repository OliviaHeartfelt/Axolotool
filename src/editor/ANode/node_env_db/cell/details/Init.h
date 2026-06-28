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
                pin_item_id TEXT REFERENCES pin_items(id) ON DELETE SET NULL,
                widget_id TEXT REFERENCES widgets(id) ON DELETE SET NULL,

                FOREIGN KEY(node_id) REFERENCES nodes(node_id) ON DELETE CASCADE,
                UNIQUE(node_id, layout_row, layout_col),

                CHECK (
                    (pin_item_id IS NULL AND widget_id IS NULL) OR
                    (pin_item_id IS NOT NULL AND widget_id IS NULL) OR
                    (pin_item_id IS NULL AND widget_id IS NOT NULL)
                )
            );
        )";
    
        if (!query.exec(createCellsTable)) {
            qCritical() << "Failed to create node_cells table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}