#pragma once

namespace NDNodeDetails::Init {

    inline bool createNodeTable(QSqlQuery& query);
    inline bool createNodeCoreTable(QSqlQuery& query);

    inline bool createAllTables(QSqlQuery& query) {
        return createNodeCoreTable(query)
            && createNodeTable(query);
    }

    inline bool createNodeCoreTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS node_core (
                id                      BLOB NOT NULL,
                contributor_id          BLOB NOT NULL,
                type_id                 BLOB,
                data_id                 BLOB,
                name                    TEXT NOT NULL,
                default_row_num         SMALLINT NOT NULL,
                default_col_num         SMALLINT NOT NULL,
                default_node_w          REAL DEFAULT -1.0,
                default_node_h          REAL DEFAULT -1.0,
                cell_visual_fallback_id BLOB,

                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES node_contributor(id) ON DELETE CASCADE  ON UPDATE CASCADE,
                FOREIGN KEY (type_id)        REFERENCES node_type(id)        ON DELETE SET NULL ON UPDATE CASCADE,
                FOREIGN KEY (data_id)        REFERENCES node_data(id)        ON DELETE SET NULL ON UPDATE CASCADE,

                CONSTRAINT chk_row_num CHECK (default_row_num >= 1),
                CONSTRAINT chk_col_num CHECK (default_col_num >= 1)
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create node core table:" << query.lastError().text();
            return false;
        }
        return true;
    }

    inline bool createNodeTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS node (
                id       BLOB NOT NULL,
                core_id  BLOB NOT NULL,
                name     TEXT NOT NULL,
                row_num  SMALLINT NOT NULL,
                col_num  SMALLINT NOT NULL,
                canvas_x REAL DEFAULT 0.0,
                canvas_y REAL DEFAULT 0.0,
                node_w   REAL DEFAULT -1.0,
                node_h   REAL DEFAULT -1.0,
                state    BLOB,

                PRIMARY KEY (id),
                FOREIGN KEY (core_id) REFERENCES node_core(id) ON DELETE CASCADE ON UPDATE CASCADE,

                CONSTRAINT chk_row_num CHECK (row_num >= 1),
                CONSTRAINT chk_col_num CHECK (col_num >= 1)
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create node table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}