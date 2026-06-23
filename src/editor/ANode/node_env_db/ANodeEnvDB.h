#pragma once

#include "cell/NDCell.h"
#include "node/NDNode.h"

#include "NDConfig.h"
#include "NDConcepts.h"
#include "../../Utility/Utility.h"

namespace NDNode { struct ComponentFriendTag; }
namespace NDCell { struct ComponentFriendTag; }

namespace ANodeEnvDB {

	class ANodeEnvDB {
		QString connectionName;
		QString dbPath;

        bool createCoreTables() {
            if (!node.createTable()) return false;
            if (!cell.createTable()) return false;
            return true;
        }

	public:
        class StorageKey {
            friend class NDNode::ComponentFriendTag;
            friend class NDCell::ComponentFriendTag;
            StorageKey() = default;
        };

		ANodeEnvDB(const QString& connectionName, const QString& dbPath) : connectionName(connectionName), dbPath(dbPath), node(this), cell(this) {}
		~ANodeEnvDB() {
			close();
		}

        QSqlDatabase getDatabase(StorageKey) const { return QSqlDatabase::database(connectionName); }

        NDNode::Component<ANodeEnvDB> node;
        NDCell::Component<ANodeEnvDB> cell;


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
	};
    static_assert(NDConcepts::DatabaseProvider<ANodeEnvDB>);
}