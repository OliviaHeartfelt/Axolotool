#pragma once

namespace NDWidgetDetails::Init {

    inline bool createAllTables(QSqlQuery& query) {
        return createWidgetCoreTable(query) 
            && createWidgetTable(query);
    }
    inline bool createWidgetCoreTable(QSqlQuery& query) {
        QString createPinSourceTable = R"(
            CREATE TABLE IF NOT EXISTS widget_core (
                id             BLOB PRIMARY KEY,
                contributor_id BLOB NOT NULL REFERENCES widget_contributor(id) ON DELETE CASCADE,
                type_id        BLOB REFERENCES (id) ON DELETE CASCADE,
                data_id        BLOB REFERENCES (id) ON DELETE CASCADE
            );
        )";

        if (!query.exec(createPinSourceTable)) {
            qCritical() << "Failed to create widget core table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWidgetTable(QSqlQuery& query) {
        QString createPinSourceTable = R"(
            CREATE TABLE IF NOT EXISTS widget (
                id      BLOB PRIMARY KEY,
                core_id BLOB NOT NULL REFERENCES widget_core(id) ON DELETE CASCADE,
                state   BLOB
                w_size  REAL DEFAULT 0.0
                h_size  REAL DEFAULT 0.0
            );
        )";

        if (!query.exec(createPinSourceTable)) {
            qCritical() << "Failed to create widget table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}