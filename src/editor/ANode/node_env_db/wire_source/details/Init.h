#pragma once

namespace NDWireSourceDetails::Init {

    inline bool createWireSourceTable(QSqlQuery& query);
    inline bool createWireContributorTable(QSqlQuery& query);
    inline bool createWireStyleTable(QSqlQuery& query);
    inline bool createWireDataTable(QSqlQuery& query);

    inline bool createAllTables(QSqlQuery& query) {
        return createWireSourceTable(query)
            && createWireContributorTable(query)
            && createWireStyleTable(query)
            && createWireDataTable(query);
    }
    inline bool createWireSourceTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS wire_source (
                id   BLOB NOT NULL,
                name TEXT NOT NULL,
            
                UNIQUE(name),
                PRIMARY KEY (id)
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create wire source table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWireContributorTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS wire_contributor (
                id        BLOB NOT NULL,
                source_id BLOB NOT NULL,
                name      TEXT NOT NULL,
            
                UNIQUE(source_id, name),
                PRIMARY KEY (id),
                FOREIGN KEY (source_id) REFERENCES wire_source(id) ON DELETE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create wire contributor table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWireStyleTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS wire_style (
                id             BLOB NOT NULL,
                contributor_id BLOB NOT NULL,
                name           TEXT NOT NULL,
                color          INTEGER NOT NULL DEFAULT 4286611584,
                wire_thickness INTEGER NOT NULL DEFAULT 2,
                metadata       BLOB,
            
                UNIQUE(contributor_id, name),
                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES wire_contributor(id) ON DELETE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create wire style table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWireDataTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS wire_data (
                id             BLOB NOT NULL,
                contributor_id BLOB NOT NULL,
                name           TEXT NOT NULL,
                data           BLOB,
            
                UNIQUE(contributor_id, name),
                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES wire_contributor(id) ON DELETE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create wire data table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}