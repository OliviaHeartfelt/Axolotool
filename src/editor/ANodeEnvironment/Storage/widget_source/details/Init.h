#pragma once

namespace NDWidgetSourceDetails::Init {

    inline bool createWidgetSourceTable(QSqlQuery& query);
    inline bool createWidgetContributorTable(QSqlQuery& query);
    inline bool createWidgetTypeTable(QSqlQuery& query);
    inline bool createWidgetDataTable(QSqlQuery& query);

    inline bool createAllTables(QSqlQuery& query) {
        return createWidgetSourceTable(query) &&
            createWidgetContributorTable(query) &&
            createWidgetTypeTable(query) &&
            createWidgetDataTable(query);
    }
    inline bool createWidgetSourceTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS widget_source (
                id               BLOB NOT NULL,
                global_source_id BLOB,
                name             TEXT NOT NULL,

                UNIQUE(name),

                PRIMARY KEY (id),
                FOREIGN KEY (global_source_id) REFERENCES global_source(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create widget source table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWidgetContributorTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS widget_contributor (
                id        BLOB NOT NULL,
                source_id BLOB NOT NULL,
                name      TEXT NOT NULL,

                UNIQUE(source_id, name),

                PRIMARY KEY (id),
                FOREIGN KEY (source_id) REFERENCES widget_source(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create widget contributor table:" << query.lastError().text();
            return false;
        }
        return true;
    }        
    inline bool createWidgetTypeTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS widget_type (
                id             BLOB NOT NULL,
                contributor_id BLOB NOT NULL,
                name           TEXT NOT NULL,
                metadata       BLOB,

                UNIQUE(contributor_id, name),

                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES widget_contributor(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create widget type table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWidgetDataTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS widget_data (
                id             BLOB NOT NULL,
                contributor_id BLOB NOT NULL,
                name           TEXT NOT NULL,
                data           BLOB,

                UNIQUE(contributor_id, name),

                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES widget_contributor(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create widget data table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}