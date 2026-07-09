#pragma once

namespace NDWireDetails::Init {

    inline bool createWireCoreTable(QSqlQuery& query);
    inline bool createWirePinsTable(QSqlQuery& query);
    inline bool createWireWidgetsTable(QSqlQuery& query);
    inline bool createWireArbitraryTable(QSqlQuery& query);

    inline bool createAllTables(QSqlQuery& query) {
        return createWireCoreTable(query)
            && createWireTemporaryTable(query)
            && createWirePinsTable(query)
            && createWireWidgetsTable(query)
            && createWireArbitraryTable(query);
    }
    inline bool createWireCoreTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS wire_core (
                id             BLOB NOT NULL,
                contributor_id BLOB NOT NULL,
                style_id       BLOB,
                data_id        BLOB,
                name           TEXT NOT NULL,

                UNIQUE(contributor_id, name),
            
                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES wire_contributor(id) ON DELETE CASCADE,
                FOREIGN KEY (style_id)       REFERENCES wire_style(id)       ON DELETE SET NULL,
                FOREIGN KEY (data_id)        REFERENCES wire_data(id)        ON DELETE SET NULL
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create wire core table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWireTemporaryTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS wire_temporary (
                id        BLOB NOT NULL,
                core_id   BLOB NOT NULL,

                PRIMARY KEY (id),
                FOREIGN KEY (core_id)   REFERENCES wire_core(id) ON DELETE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create wire temporary table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWirePinsTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS wire_pins (
                id        BLOB NOT NULL,
                core_id   BLOB NOT NULL,
                
                origin_id BLOB NOT NULL,
                origin_canvas_hint_x REAL NOT NULL,
                origin_canvas_hint_y REAL NOT NULL,
            
                target_id BLOB NOT NULL,
                target_canvas_hint_x REAL NOT NULL,
                target_canvas_hint_y REAL NOT NULL,
            
                state    BLOB,
            
                PRIMARY KEY (id),
                FOREIGN KEY (core_id)   REFERENCES wire_core(id) ON DELETE CASCADE,
            
                FOREIGN KEY (origin_id) REFERENCES pin(id)       ON DELETE CASCADE,
                FOREIGN KEY (target_id) REFERENCES pin(id)       ON DELETE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create wire pins table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWireWidgetsTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS wire_widgets (
                id        BLOB NOT NULL,
                core_id   BLOB NOT NULL,
            
                origin_id BLOB NOT NULL,
                origin_canvas_hint_x REAL NOT NULL,
                origin_canvas_hint_y REAL NOT NULL,
            
                target_id BLOB NOT NULL,
                target_canvas_hint_x REAL NOT NULL,
                target_canvas_hint_y REAL NOT NULL,
            
                state     BLOB,
            
                PRIMARY KEY (id),
                FOREIGN KEY (core_id)   REFERENCES wire_core(id) ON DELETE CASCADE,
            
                FOREIGN KEY (origin_id) REFERENCES widget(id)    ON DELETE CASCADE,
                FOREIGN KEY (target_id) REFERENCES widget(id)    ON DELETE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create wire widgets table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWireArbitraryTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS wire_arbitrary (
                id               BLOB NOT NULL,
                core_id          BLOB NOT NULL,
            
                origin_pin_id    BLOB,
                origin_widget_id BLOB,
                origin_canvas_hint_x REAL NOT NULL,
                origin_canvas_hint_y REAL NOT NULL,
            
                target_pin_id    BLOB,
                target_widget_id BLOB,
                target_canvas_hint_x REAL NOT NULL,
                target_canvas_hint_y REAL NOT NULL,
            
                state            BLOB,
            
                CONSTRAINT chk_exclusive_origin CHECK (
                    (origin_pin_id IS NULL     AND origin_widget_id IS NOT NULL) OR 
                    (origin_pin_id IS NOT NULL AND origin_widget_id IS NULL) 
                ),
                CONSTRAINT chk_exclusive_target CHECK (
                    (target_pin_id IS NULL     AND target_widget_id IS NOT NULL) OR 
                    (target_pin_id IS NOT NULL AND target_widget_id IS NULL) 
                ),
            
                PRIMARY KEY (id),
                FOREIGN KEY (core_id)          REFERENCES wire_core(id) ON DELETE CASCADE,
            
                FOREIGN KEY (origin_pin_id)    REFERENCES pin(id)       ON DELETE CASCADE,
                FOREIGN KEY (origin_widget_id) REFERENCES widget(id)    ON DELETE CASCADE,
            
                FOREIGN KEY (target_pin_id)    REFERENCES pin(id)       ON DELETE CASCADE,
                FOREIGN KEY (target_widget_id) REFERENCES widget(id)    ON DELETE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create wire arbitrary table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}