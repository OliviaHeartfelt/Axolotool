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
                id   BLOB PRIMARY KEY,
                name TEXT NOT NULL,
                UNIQUE(name)
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
                id        BLOB PRIMARY KEY,
                source_id BLOB NOT NULL REFERENCES widget_source(id) ON DELETE CASCADE,
                name      TEXT NOT NULL,
                UNIQUE(source_id, name)
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
                id             BLOB PRIMARY KEY,
                contributor_id BLOB NOT NULL REFERENCES widget_contributor(id) ON DELETE CASCADE,
                name           TEXT NOT NULL,
                metadata       BLOB,
                UNIQUE(contributor_id, name)
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
                id             BLOB PRIMARY KEY,
                contributor_id BLOB NOT NULL REFERENCES widget_contributor(id) ON DELETE CASCADE,
                name           TEXT NOT NULL,
                data           BLOB,
                UNIQUE(contributor_id, name)
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create widget data table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}