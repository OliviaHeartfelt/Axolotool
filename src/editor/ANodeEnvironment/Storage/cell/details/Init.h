#pragma once

namespace NDCellDetails::Init {

    inline bool createCellTable(QSqlQuery& query);
    inline bool createCellOriginTable(QSqlQuery& query);

    inline bool createAllTables(QSqlQuery& query) {
        return createCellTable(query)
            && createCellOriginTable(query);
    }

    inline bool createCellTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS cell (
                id              BLOB NOT NULL,
                node_id         BLOB NOT NULL,
                name            TEXT,
                layout_row      SMALLINT NOT NULL,
                layout_col      SMALLINT NOT NULL,
                layout_row_span SMALLINT NOT NULL DEFAULT 1,
                layout_col_span SMALLINT NOT NULL DEFAULT 1,

                pin_core_id     BLOB,
                widget_id       BLOB,

                PRIMARY KEY(id),
                FOREIGN KEY(node_id)     REFERENCES node(id)     ON DELETE CASCADE  ON UPDATE CASCADE,
                FOREIGN KEY(pin_core_id) REFERENCES pin_core(id) ON DELETE SET NULL ON UPDATE CASCADE,
                FOREIGN KEY(widget_id)   REFERENCES widget(id)   ON DELETE SET NULL ON UPDATE CASCADE,

                CONSTRAINT chk_row_span        CHECK (layout_row_span >= 1),
                CONSTRAINT chk_col_span        CHECK (layout_col_span >= 1),

                CONSTRAINT chk_exclusive_content CHECK (
                    (pin_core_id IS NOT NULL) + (widget_id IS NOT NULL) <= 1
                )   
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create cell table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createCellOriginTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS cell_origin (
                id              BLOB NOT NULL,
                node_core_id    BLOB NOT NULL,
                name            TEXT,
                layout_row      SMALLINT NOT NULL,
                layout_col      SMALLINT NOT NULL,
                layout_row_span SMALLINT NOT NULL DEFAULT 1,
                layout_col_span SMALLINT NOT NULL DEFAULT 1,

                pin_core_id     BLOB,
                widget_core_id  BLOB,

                UNIQUE(node_core_id, layout_row, layout_col),

                PRIMARY KEY(id),
                FOREIGN KEY(node_core_id)   REFERENCES node_core(id)   ON DELETE CASCADE  ON UPDATE CASCADE,
                FOREIGN KEY(pin_core_id)    REFERENCES pin_core(id)    ON DELETE SET NULL ON UPDATE CASCADE,
                FOREIGN KEY(widget_core_id) REFERENCES widget_core(id) ON DELETE SET NULL ON UPDATE CASCADE,

                CONSTRAINT chk_row_span CHECK (layout_row_span >= 1),
                CONSTRAINT chk_col_span CHECK (layout_col_span >= 1),

                CONSTRAINT chk_exclusive_content CHECK (
                    (pin_core_id IS NOT NULL) + (widget_core_id IS NOT NULL) <= 1
                )
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create cell origin table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}