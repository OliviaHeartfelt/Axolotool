#pragma once

namespace NDGlobalSourceDetails::Init {

    inline bool createGlobalSourceTable(QSqlQuery& query);

    inline bool createAllTables(QSqlQuery& query) {
        return createGlobalSourceTable(query);
    }
    inline bool createGlobalSourceTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS global_source (
                id   BLOB NOT NULL,
                name TEXT NOT NULL,
                name TEXT,
            
                UNIQUE(name),
                PRIMARY KEY (id)
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create global source table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}