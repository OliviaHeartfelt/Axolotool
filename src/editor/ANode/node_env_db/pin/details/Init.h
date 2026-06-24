#pragma once

namespace NDPinDetails::Init {
	
    inline bool createAllTables(QSqlQuery& query) {
        return createPinTable(query)
            && createPinAllowFlowTable(query)
            && createPinAllowTypeTable(query);
    }
    inline bool createPinTable(QSqlQuery& query) {
        QString createPinTable = R"(
            CREATE TABLE IF NOT EXISTS pin (
                id        BLOB PRIMARY KEY,
                flow_id   BLOB REFERENCES flow(id)  ON DELETE SET NULL,
                type_id   BLOB REFERENCES type(id)  ON DELETE SET NULL,
                style_id  BLOB REFERENCES style(id) ON DELETE SET NULL
            );
        )";

        if (!query.exec(createPinTable)) {
            qCritical() << "Failed to create pin table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinAllowFlowTable(QSqlQuery& query) {
        QString createPinAllowFlowTable = R"(
            CREATE TABLE IF NOT EXISTS pin_allow_flow (
                pin_id  BLOB NOT NULL REFERENCES pin(id)  ON DELETE CASCADE,
                flow_id BLOB NOT NULL REFERENCES flow(id) ON DELETE CASCADE,
                PRIMARY KEY (pin_id, flow_id)
            );
        )";

        if (!query.exec(createPinAllowFlowTable)) {
            qCritical() << "Failed to create pin table:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinAllowTypeTable(QSqlQuery& query) {
        QString createPinAllowTypeTable = R"(
            CREATE TABLE IF NOT EXISTS pin_allow_type (
                pin_id  BLOB NOT NULL REFERENCES pin(id)  ON DELETE CASCADE,
                type_id BLOB NOT NULL REFERENCES type(id) ON DELETE CASCADE,
                PRIMARY KEY (pin_id, type_id)
            );
        )";

        if (!query.exec(createPinAllowTypeTable)) {
            qCritical() << "Failed to create pin table:" << query.lastError().text();
            return false;
        }
        return true;
    }
}