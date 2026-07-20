#pragma once

namespace NDPinSourceDetails::Init {

    inline bool createPinSourceTable(QSqlQuery& query);
    inline bool createPinContributorTable(QSqlQuery& query);
    inline bool createPinFlowTable(QSqlQuery& query);
    inline bool createPinTypeTable(QSqlQuery& query);
    inline bool createPinStyleTable(QSqlQuery& query);


    inline bool createAllTables(QSqlQuery& query) {
        return createPinSourceTable(query)
            && createPinContributorTable(query)
            && createPinFlowTable(query)
            && createPinTypeTable(query)
            && createPinStyleTable(query);
    }
    inline bool createPinSourceTable(QSqlQuery& query) {
        QString createPinSourceTable = R"(
            CREATE TABLE IF NOT EXISTS pin_source (
                id               BLOB NOT NULL,
                global_source_id BLOB,
                name             TEXT NOT NULL,

                UNIQUE(name),

                PRIMARY KEY (id),
                FOREIGN KEY (global_source_id) REFERENCES global_source(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(createPinSourceTable)) {
            qCritical() << "Failed to create pin_source table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinContributorTable(QSqlQuery& query) {
        QString createPinContributorTable = R"(
            CREATE TABLE IF NOT EXISTS pin_contributor (
                id        BLOB NOT NULL,
                source_id BLOB NOT NULL,
                name      TEXT NOT NULL,

                UNIQUE(source_id, name),

                PRIMARY KEY (id),
                FOREIGN KEY (source_id) REFERENCES pin_source(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(createPinContributorTable)) {
            qCritical() << "Failed to create pin_contributor table:" << query.lastError().text();
            return false;
        }
        return true;
    }

    inline bool createPinFlowTable(QSqlQuery& query) {
        QString createPinFlowTable = R"(
            CREATE TABLE IF NOT EXISTS pin_flow (
                id             BLOB NOT NULL,
                contributor_id BLOB NOT NULL ,
                name           TEXT NOT NULL,
                degree         REAL DEFAULT 0.0,

                UNIQUE(contributor_id, name),

                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES pin_contributor(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(createPinFlowTable)) {
            qCritical() << "Failed to create flow table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinTypeTable(QSqlQuery& query) {
        QString createPinTypeTable = R"(
            CREATE TABLE IF NOT EXISTS pin_type (
                id             BLOB NOT NULL,
                contributor_id BLOB NOT NULL,
                name           TEXT NOT NULL,
                bit_size       INTEGER DEFAULT 0,

                UNIQUE(contributor_id, name),

                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES pin_contributor(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(createPinTypeTable)) {
            qCritical() << "Failed to create  type table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinStyleTable(QSqlQuery& query) {
        QString createPinStyleTable = R"(
            CREATE TABLE IF NOT EXISTS pin_style (
                id             BLOB NOT NULL,
                contributor_id BLOB NOT NULL,
                name           TEXT NOT NULL,
                color          INTEGER NOT NULL,
                wire_thickness INTEGER DEFAULT 2,

                UNIQUE(contributor_id, name),

                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES pin_contributor(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(createPinStyleTable)) {
            qCritical() << "Failed to create pin style table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}