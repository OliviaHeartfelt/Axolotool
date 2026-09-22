#pragma once

namespace NDWireDetails::Init {

    inline bool createWireCoreTable(QSqlQuery& query);
    inline bool createWireTemporaryTable(QSqlQuery& query);
    inline bool createWireTable(QSqlQuery& query);
    inline bool createSymmetricWireRegistryTable(QSqlQuery& query);
    inline bool createAsymmetricWireRegistryTable(QSqlQuery& query);

    inline bool createAllTables(QSqlQuery& query) {
        return createWireCoreTable(query)
            && createWireTemporaryTable(query)
            && createWireTable(query)
            && createSymmetricWireRegistryTable(query)
            && createAsymmetricWireRegistryTable(query);
    }
    inline bool createWireCoreTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS wire_core (
                id                BLOB NOT NULL,
                contributor_id    BLOB NOT NULL,
                visual_factory_id BLOB NOT NULL,
                style_id          BLOB,
                data_id           BLOB,
                name              TEXT NOT NULL,

                UNIQUE(contributor_id, name),
            
                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES wire_contributor(id) ON DELETE CASCADE  ON UPDATE CASCADE,
                FOREIGN KEY (style_id)       REFERENCES wire_style(id)       ON DELETE SET NULL ON UPDATE CASCADE,
                FOREIGN KEY (data_id)        REFERENCES wire_data(id)        ON DELETE SET NULL ON UPDATE CASCADE
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
                FOREIGN KEY (core_id)   REFERENCES wire_core(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create wire temporary table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWireTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS wire (
                id                   BLOB NOT NULL,
                core_id              BLOB NOT NULL,
                
                origin_id            BLOB NOT NULL,
                origin_canvas_hint_x REAL NOT NULL,
                origin_canvas_hint_y REAL NOT NULL,
                
                target_id            BLOB NOT NULL,
                target_canvas_hint_x REAL NOT NULL,
                target_canvas_hint_y REAL NOT NULL,
                
                state                BLOB,
                
                PRIMARY KEY (id),
                FOREIGN KEY (core_id)   REFERENCES wire_core(id)  ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (origin_id) REFERENCES node_cells(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (target_id) REFERENCES node_cells(id) ON DELETE CASCADE ON UPDATE CASCADE,

                CONSTRAINT chk_no_self_loop CHECK (origin_id != target_id)
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create wire table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createSymmetricWireRegistryTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS symmetric_wire_registry (
                pin_type_id  BLOB NOT NULL,
                wire_core_id BLOB NOT NULL,
                
                PRIMARY KEY (pin_type_id),
                FOREIGN KEY (pin_type_id)  REFERENCES pin_type(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (wire_core_id) REFERENCES wire_core(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create symetric wire registry table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createAsymmetricWireRegistryTable(QSqlQuery& query) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS asymmetric_wire_registry (
                origin_pin_type_id BLOB NOT NULL,
                target_pin_type_id BLOB NOT NULL,
                wire_core_id       BLOB NOT NULL,
                
                PRIMARY KEY (origin_pin_type_id, target_pin_type_id),
                FOREIGN KEY (origin_pin_type_id) REFERENCES pin_type(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (target_pin_type_id) REFERENCES pin_type(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (wire_core_id)       REFERENCES wire_core(id) ON DELETE CASCADE ON UPDATE CASCADE,

                CONSTRAINT chk_not_eq_type CHECK (origin_pin_type_id != target_pin_type_id)
            );
        )";

        if (!query.exec(sql)) {
            qCritical() << "Failed to create asymetric wire registry table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}