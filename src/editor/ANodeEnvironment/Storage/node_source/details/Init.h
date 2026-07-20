#pragma once

namespace NDNodeSourceDetails::Init {

    inline bool createNodeSourceTable(QSqlQuery& query);
    inline bool createNodeContributorTable(QSqlQuery& query);
    inline bool createNodeTypeTable(QSqlQuery& query);
    inline bool createNodeDataTable(QSqlQuery& query);

    inline bool createAllTables(QSqlQuery& query) {
        return createNodeSourceTable(query)
            && createNodeContributorTable(query)
            && createNodeTypeTable(query)
            && createNodeDataTable(query);
    }

    inline bool createNodeSourceTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS node_source (
                id               BLOB NOT NULL,
                global_source_id BLOB,
                name             TEXT NOT NULL,

                UNIQUE(name),

                PRIMARY KEY (id),
                FOREIGN KEY (global_source_id) REFERENCES global_source(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create node source table:" << query.lastError().text();
            return false;
        }
        return true;
    }

    inline bool createNodeContributorTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS node_contributor (
                id        BLOB NOT NULL,
                source_id BLOB NOT NULL,
                name      TEXT NOT NULL,

                UNIQUE(source_id, name),

                PRIMARY KEY (id),
                FOREIGN KEY (source_id) REFERENCES node_source(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create node contributor table:" << query.lastError().text();
            return false;
        }
        return true;
    }

    inline bool createNodeTypeTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS node_type (
                id             BLOB NOT NULL,
                contributor_id BLOB NOT NULL,
                name           TEXT NOT NULL,
                metadata       BLOB,

                UNIQUE(contributor_id, name),

                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES node_contributor(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create node type table:" << query.lastError().text();
            return false;
        }
        return true;
    }

    inline bool createNodeDataTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS node_data (
                id             BLOB NOT NULL,
                contributor_id BLOB NOT NULL,
                name           TEXT NOT NULL,
                data           BLOB,

                UNIQUE(contributor_id, name),

                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES node_contributor(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create node data table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}