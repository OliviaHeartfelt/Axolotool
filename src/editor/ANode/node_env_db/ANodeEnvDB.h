#pragma once

#include "node/NDNode.h"
#include "cell/NDCell.h"

#include "NDConfig.h"
#include "NDConcepts.h"
#include "../../Utility/Utility.h"

namespace ANodeEnvDB {

	class ANodeEnvDB {
		QString connectionName;
		QString dbPath;

        QSqlDatabase getDatabase() const { return QSqlDatabase::database(connectionName); }

		ANodeEnvDB(const QString& connectionName, const QString& dbPath) : connectionName(connectionName), dbPath(dbPath) {}
		~ANodeEnvDB() {
			close();
		}
	public:



        bool containsAllCoreTables(const QStringList& strList) const {
            if (!strList.contains("nodes", Qt::CaseInsensitive)) return false;
            if (!strList.contains("node_cells", Qt::CaseInsensitive)) return false;
            return true;
        }

        bool open() {
            QSqlDatabase db;

            if (QSqlDatabase::contains(connectionName)) {
                db = QSqlDatabase::database(connectionName);
            }
            else {
                db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
                db.setDatabaseName(dbPath);
            }

            if (!db.open()) {
                qCritical() << "Failed to open Node DB:" << db.lastError().text();
                return false;
            }

            QSqlQuery query(db);
            if (!query.exec("PRAGMA foreign_keys = ON;")) { qWarning() << "Failed to enable Foreign Keys:" << query.lastError().text(); }
            if (!query.exec("PRAGMA journal_mode=WAL;")) {  qWarning() << "Failed to enable WAL mode:"     << query.lastError().text(); }

            if (!createCoreTables()) { return false; }

            if (!query.exec("PRAGMA user_version;")) {
                qCritical() << "Failed to read database schema version:" << query.lastError().text();
                return false;
            }
            query.next();
            int storedVersion = query.value(0).toInt();

            if (storedVersion < NDConfig::currentSchemaVersion()) {
                /* upgrade logic */
            }

            return true;
        }

        void close() {
            if (QSqlDatabase::contains(connectionName)) {
                auto db = QSqlDatabase::database(connectionName);
                if (db.isOpen()) db.close();
            }
        }

        // Node
        std::optional<muuid::uuid> createNode(const QString& title, const short rowNum, const short colNum, const QList<NDCell::RecordInfo>& cells = {}, const QPointF pos = {0.0, 0.0}) {
            QSqlDatabase db = getDatabase();
            if (!containsAllCoreTables(db.tables())) return std::nullopt;
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return std::nullopt;

            std::optional<muuid::uuid> nodeId = NDNode::create(query, title, rowNum, colNum, pos);
            if (!nodeId) return std::nullopt;

            for (const auto& cell : cells)
                if (!NDCell::create(query, nodeId.value(), cell)) return std::nullopt;

            if (!tr.commit()) return std::nullopt;
            return nodeId;
        }
        bool removeNode(const muuid::uuid& ID) {
            QSqlDatabase db = getDatabase();
            if (!containsAllCoreTables(db.tables())) return false;

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            NDNode::remove(db, ID);
            return tr.commit();
        }

        // Cell
        std::optional<muuid::uuid> createCell(const muuid::uuid& nodeId, const NDCell::RecordInfo& cell, bool overrideOnCollision = false) {
            QSqlDatabase db = getDatabase();
            if (!containsAllCoreTables(db.tables())) return std::nullopt;

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return std::nullopt;

            QSqlQuery query(db);
            muuid::uuid newCellId = muuid::uuid::generate_unix_time_based();

            std::optional<muuid::uuid> insertedId = NDCell::create(query, nodeId, cell, overrideOnCollision);
            tr.rollbackIf(!insertedId, "Aborting cell topology build due to insertion failure.");

            if (!tr.commit()) return std::nullopt;
            return insertedId;
        }
        bool removeCell(const muuid::uuid& ID) {
            QSqlDatabase db = getDatabase();
            if (!containsAllCoreTables(db.tables())) return false;

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            NDCell::remove(db, ID);
            return tr.commit();
        }

    private:
        bool createCoreTables() {
            QSqlDatabase db = getDatabase();
            if (containsAllCoreTables(db.tables())) return true;

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            QSqlQuery query(db);
            if ( tr.rollbackIf(!NDNode::createTable(query) || !NDCell::createTable(query), "Schema initialization failed! Rolling back changes.") ) return false;

            return tr.commit();
        }
	};
    //static_assert(NDConcepts::DatabaseProvider<ANodeEnvDB>);
}