#pragma once

#include "cell/NDCell.h"
#include "node/NDNode.h"
#include "node_source/NDNodeSource.h"

#include "pin/NDPin.h"
#include "pin_source/NDPinSource.h"

#include "widget/NDWidget.h"
#include "widget_source/NDWidgetSource.h"

#include "NDConfig.h"
#include "NDConcepts.h"
#include "../../Utility/Utility.h"

namespace NDNode {         struct ComponentFriendTag; }
namespace NDNodeSource {   struct ComponentFriendTag; }
namespace NDCell {         struct ComponentFriendTag; }
namespace NDPin {          struct ComponentFriendTag; }
namespace NDPinSource {    struct ComponentFriendTag; }
namespace NDWidget {       struct ComponentFriendTag; }
namespace NDWidgetSource { struct ComponentFriendTag; }


namespace ANodeEnvDB {

	class ANodeEnvDB {
		QString connectionName;
		QString dbPath;

        bool createCoreTables() {
            if (!node.createAllTables())         return false;
            if (!cell.createAllTables())         return false;
            if (!pin.createAllTables())          return false;
            if (!pinSource.createAllTables())    return false;
            if (!widget.createAllTables())       return false;
            if (!widgetSource.createAllTables()) return false;
            return true;
        }

	public:
        class StorageKey {
            friend class NDNode::ComponentFriendTag;
            friend class NDNodeSource::ComponentFriendTag;
            friend class NDCell::ComponentFriendTag;
            friend class NDPin::ComponentFriendTag;
            friend class NDPinSource::ComponentFriendTag;
            friend class NDWidget::ComponentFriendTag;
            friend class NDWidgetSource::ComponentFriendTag;
            StorageKey() = default;
        };

		ANodeEnvDB(const QString& connectionName, const QString& dbPath) : connectionName(connectionName), dbPath(dbPath), 
            node(this), 
            nodeSource(this),
            cell(this),
            pin(this),
            pinSource(this),
            widget(this),
            widgetSource(this)
        {}

		~ANodeEnvDB() {
			close();
		}

        QSqlDatabase getDatabase(StorageKey) const { return QSqlDatabase::database(connectionName); }

        NDNode::Component<ANodeEnvDB> node;
        NDNodeSource::Component<ANodeEnvDB> nodeSource;
        NDCell::Component<ANodeEnvDB> cell;
        NDPin::Component<ANodeEnvDB> pin;
        NDPinSource::Component<ANodeEnvDB> pinSource;
        NDWidget::Component<ANodeEnvDB> widget;
        NDWidgetSource::Component<ANodeEnvDB> widgetSource;

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
            if (!query.exec("PRAGMA foreign_keys = ON;")) { qCritical() << "Failed to enable Foreign Keys:" << query.lastError().text(); }
            if (!query.exec("PRAGMA journal_mode=WAL;")) {  qCritical() << "Failed to enable WAL mode:"     << query.lastError().text(); }

            if (!createCoreTables()) { return false; }

            if (!query.exec("PRAGMA user_version;")) {
                qCritical() << "Failed to read database schema version:" << query.lastError().text();
                return false;
            }

            std::optional<unsigned int> storedVersion = std::nullopt;
            if (query.next()) {
                storedVersion = query.value(0).toInt();
            }
            else {
                qCritical() << "Database failed to return user_version row.";
                return false;
            }

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