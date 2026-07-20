#pragma once

namespace NDCellDetails::Init {

    inline bool createNodeCellTable(QSqlQuery& query);

    inline bool createAllTables(QSqlQuery& query) {
        return createNodeCellTable(query);
    }

    inline bool createNodeCellTable(QSqlQuery& query) {
        QString createCellsTable = R"(
            CREATE TABLE IF NOT EXISTS node_cells (
                id              BLOB NOT NULL,
                node_id         BLOB NOT NULL,
                name            TEXT,
                layout_row      SMALLINT NOT NULL,
                layout_col      SMALLINT NOT NULL,
                layout_row_span SMALLINT NOT NULL DEFAULT 1,
                layout_col_span SMALLINT NOT NULL DEFAULT 1,
                is_out          INTEGER NOT NULL DEFAULT 0,

                pin_template_id BLOB,
                pin_instance_id BLOB,
                widget_id       BLOB,

                UNIQUE(node_id, layout_row, layout_col),

                PRIMARY KEY(id),
                FOREIGN KEY(node_id)         REFERENCES node(id)     ON DELETE CASCADE  ON UPDATE CASCADE,
                FOREIGN KEY(pin_template_id) REFERENCES pin_core(id) ON DELETE SET NULL ON UPDATE CASCADE,
                FOREIGN KEY(pin_instance_id) REFERENCES pin(id)      ON DELETE SET NULL ON UPDATE CASCADE,
                FOREIGN KEY(widget_id)       REFERENCES widget(id)   ON DELETE SET NULL ON UPDATE CASCADE,

                CONSTRAINT chk_row_span        CHECK (layout_row_span >= 1),
                CONSTRAINT chk_col_span        CHECK (layout_col_span >= 1),
                CONSTRAINT chk_cell_visibility CHECK (is_out IN (0, 1)),

                CONSTRAINT chk_exclusive_content CHECK (
                    (pin_template_id IS NOT NULL) + (pin_instance_id IS NOT NULL) + (widget_id IS NOT NULL) <= 1
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