#pragma once

namespace NDPinDetails::Init {

    inline bool createPinCoreTable(QSqlQuery& query);
    inline bool createPinTable(QSqlQuery& query);
    inline bool createPinAllowFlowTable(QSqlQuery& query);
    inline bool createPinAllowTypeTable(QSqlQuery& query);

    inline bool createAllTables(QSqlQuery& query) {
        return createPinCoreTable(query)
            && createPinTable(query)
            && createPinAllowFlowTable(query)
            && createPinAllowTypeTable(query);
    }
    inline bool createPinCoreTable(QSqlQuery& query) {
        QString createPinTableQuery = R"(
            CREATE TABLE IF NOT EXISTS pin_core (
                id                BLOB NOT NULL,
                contributor_id    BLOB NOT NULL,
                visual_factory_id BLOB NOT NULL,
                flow_id           BLOB,
                type_id           BLOB,
                style_id          BLOB,

                PRIMARY KEY (id),
                FOREIGN KEY (contributor_id) REFERENCES pin_contributor(id) ON DELETE CASCADE  ON UPDATE CASCADE,
                FOREIGN KEY (flow_id)        REFERENCES pin_flow(id)        ON DELETE SET NULL ON UPDATE CASCADE,
                FOREIGN KEY (type_id)        REFERENCES pin_type(id)        ON DELETE SET NULL ON UPDATE CASCADE,
                FOREIGN KEY (style_id)       REFERENCES pin_style(id)       ON DELETE SET NULL ON UPDATE CASCADE
            );
        )";

        if (!query.exec(createPinTableQuery)) {
            qCritical() << "Failed to create pin table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinTable(QSqlQuery& query) {
        QString createPinTableQuery = R"(
            CREATE TABLE IF NOT EXISTS pin (
                id      BLOB NOT NULL,
                core_id BLOB NOT NULL,

                PRIMARY KEY (id),
                FOREIGN KEY (core_id) REFERENCES pin_core(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(createPinTableQuery)) {
            qCritical() << "Failed to create pin table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinAllowFlowTable(QSqlQuery& query) {
        QString createPinAllowFlowTableQuery = R"(
            CREATE TABLE IF NOT EXISTS pin_allow_flow (
                pin_id  BLOB NOT NULL,
                flow_id BLOB NOT NULL,
                
                PRIMARY KEY (pin_id, flow_id),
                FOREIGN KEY (pin_id)  REFERENCES pin_core(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (flow_id) REFERENCES pin_flow(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(createPinAllowFlowTableQuery)) {
            qCritical() << "Failed to create pin_allow_flow table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinAllowTypeTable(QSqlQuery& query) {
        QString createPinAllowTypeTableQuery = R"(
            CREATE TABLE IF NOT EXISTS pin_allow_type (
                pin_id  BLOB NOT NULL,
                type_id BLOB NOT NULL,
                
                PRIMARY KEY (pin_id, type_id),
                FOREIGN KEY (pin_id)  REFERENCES pin_core(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (type_id) REFERENCES pin_type(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )";

        if (!query.exec(createPinAllowTypeTableQuery)) {
            qCritical() << "Failed to create pin_allow_type table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}