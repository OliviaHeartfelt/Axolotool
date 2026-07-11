#pragma once

namespace NDWidgetDetails::Init {

    inline bool createWidgetCoreTable(QSqlQuery& query);
    inline bool createWidgetTable(QSqlQuery& query);

    inline bool createAllTables(QSqlQuery& query) {
        return createWidgetCoreTable(query) 
            && createWidgetTable(query);
    }
    inline bool createWidgetCoreTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS widget_core (
                id             BLOB NOT NULL,
                contributor_id BLOB NOT NULL,
                type_id        BLOB,
                data_id        BLOB,

                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES widget_contributor(id) ON DELETE CASCADE,
                FOREIGN KEY (type_id)        REFERENCES widget_type(id)        ON DELETE CASCADE,
                FOREIGN KEY (data_id)        REFERENCES widget_data(id)        ON DELETE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create widget core table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWidgetTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS widget (
                id      BLOB NOT NULL,
                core_id BLOB NOT NULL,
                state   BLOB,
                w_size  REAL DEFAULT 0.0,
                h_size  REAL DEFAULT 0.0,

                PRIMARY KEY (id),
                FOREIGN KEY (core_id) REFERENCES widget_core(id) ON DELETE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create widget table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}