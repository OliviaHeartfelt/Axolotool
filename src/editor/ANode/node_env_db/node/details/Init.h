#pragma once

namespace NDNodeDetails::Init {

    inline bool createTable(QSqlQuery& query) {
        QString createNodesTable = R"(
            CREATE TABLE IF NOT EXISTS nodes (
                id BLOB PRIMARY KEY,
                title TEXT NOT NULL,
                row_num SMALLINT NOT NULL,
                col_num SMALLINT NOT NULL,
                canvas_x REAL DEFAULT 0.0,
                canvas_y REAL DEFAULT 0.0,
                canvas_w REAL DEFAULT -1.0,
                canvas_h REAL DEFAULT -1.0,

                CONSTRAINT chk_row_num CHECK (row_num >= 1),
                CONSTRAINT chk_col_num CHECK (row_num >= 1)
            );
        )";

        if (!query.exec(createNodesTable)) {
            qCritical() << "Failed to create nodes table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}