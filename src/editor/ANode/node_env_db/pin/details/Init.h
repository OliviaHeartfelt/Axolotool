#pragma once

namespace NDPinDetails::Init {

    inline bool createAllTables(QSqlQuery& query) {
        return createPinTable(query)
            && createPinAllowFlowTable(query)
            && createPinAllowTypeTable(query);
    }
    inline bool createPinTable(QSqlQuery& query) {
        QString createPinTableQuery = R"(
            CREATE TABLE IF NOT EXISTS pin (
                id             BLOB PRIMARY KEY,
                contributor_id BLOB NOT NULL,
                flow_id        BLOB,
                type_id        BLOB,
                style_id       BLOB,

                FOREIGN KEY (contributor_id) REFERENCES pin_contributor(id) ON DELETE CASCADE,
                FOREIGN KEY (flow_id)        REFERENCES pin_flow(id)        ON DELETE SET NULL,
                FOREIGN KEY (type_id)        REFERENCES pin_type(id)        ON DELETE SET NULL,
                FOREIGN KEY (style_id)       REFERENCES pin_style(id)       ON DELETE SET NULL
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
                FOREIGN KEY (pin_id)  REFERENCES pin(id)      ON DELETE CASCADE,
                FOREIGN KEY (flow_id) REFERENCES pin_flow(id) ON DELETE CASCADE
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
                FOREIGN KEY (pin_id)  REFERENCES pin(id)      ON DELETE CASCADE,
                FOREIGN KEY (type_id) REFERENCES pin_type(id) ON DELETE CASCADE
            );
        )";

        if (!query.exec(createPinAllowTypeTableQuery)) {
            qCritical() << "Failed to create pin_allow_type table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}