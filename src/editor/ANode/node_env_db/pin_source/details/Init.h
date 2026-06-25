#pragma once

namespace NDPinSourceDetails::Init {

    inline bool createAllTables(QSqlQuery& query) {
        return createPinSourceTable(query)
            && createPinFlowTable(query)
            && createPinTypeTable(query)
            && createPinStyleTable(query);
    }
    inline bool createPinSourceTable(QSqlQuery& query) {
        QString createPinSourceTable = R"(
            CREATE TABLE IF NOT EXISTS pin_source (
                id BLOB PRIMARY KEY,
                name TEXT NOT NULL
            );
        )";

        if (!query.exec(createPinSourceTable)) {
            qCritical() << "Failed to create pin source table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinFlowTable(QSqlQuery& query) {
        QString createPinFlowTable = R"(
            CREATE TABLE IF NOT EXISTS flow (
                id BLOB PRIMARY KEY,
                source_id BLOB REFERENCES pin_source(id) ON DELETE CASCADE,
                name TEXT NOT NULL,
                degree REAL DEFAULT 0.0,
                UNIQUE(source_id, name)
            );
        )";

        if (!query.exec(createPinFlowTable)) {
            qCritical() << "Failed to create pin flow table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinTypeTable(QSqlQuery& query) {
        QString createPinTypeTable = R"(
            CREATE TABLE IF NOT EXISTS type (
                id BLOB PRIMARY KEY,
                source_id BLOB REFERENCES pin_source(id) ON DELETE CASCADE,
                name TEXT NOT NULL,
                bit_size INTEGER DEFAULT 0,
                UNIQUE(source_id, name)
            );
        )";

        if (!query.exec(createPinTypeTable)) {
            qCritical() << "Failed to create pin type table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinStyleTable(QSqlQuery& query) {
        QString createPinStyleTable = R"(
            CREATE TABLE IF NOT EXISTS style (
                id BLOB PRIMARY KEY,
                source_id BLOB REFERENCES pin_source(id) ON DELETE CASCADE,
                name TEXT NOT NULL,
                color INTEGER NOT NULL,
                wire_thickness INTEGER DEFAULT 2,
                UNIQUE(source_id, name)
            );
        )";

        if (!query.exec(createPinStyleTable)) {
            qCritical() << "Failed to create pin style table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}