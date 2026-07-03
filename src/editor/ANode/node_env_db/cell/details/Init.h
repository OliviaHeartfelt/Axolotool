#pragma once

namespace NDCellDetails::Init {

    inline bool createTable(QSqlQuery& query) {
        QString createCellsTable = R"(
            CREATE TABLE IF NOT EXISTS node_cells (
                cell_id         BLOB PRIMARY KEY,
                node_id         BLOB,
                name            TEXT,
                layout_row      SMALLINT NOT NULL,
                layout_col      SMALLINT NOT NULL,
                layout_row_span SMALLINT NOT NULL DEFAULT 1,
                layout_col_span SMALLINT NOT NULL DEFAULT 1,
                is_out          INTEGER NOT NULL DEFAULT 0,
                pin_id          BLOB,
                widget_id       BLOB,

                UNIQUE(node_id, layout_row, layout_col),

                FOREIGN KEY(node_id)    REFERENCES nodes(id) ON DELETE CASCADE,
                FOREIGN KEY(pin_id)     REFERENCES pin(id)        ON DELETE SET NULL,
                FOREIGN KEY(widget_id)  REFERENCES widget_id(id)  ON DELETE SET NULL,

                CONSTRAINT chk_row_span         CHECK (row_span >= 1),
                CONSTRAINT chk_col_span         CHECK (col_span >= 1),
                CONSTRAINT chk_cell_visibility  CHECK (is_out IN (0, 1)),

                CONSTRAINT chk_exclusive_contentCHECK (
                    (pin_id IS NULL     AND widget_id IS NULL) OR
                    (pin_id IS NOT NULL AND widget_id IS NULL) OR
                    (pin_id IS NULL     AND widget_id IS NOT NULL)
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