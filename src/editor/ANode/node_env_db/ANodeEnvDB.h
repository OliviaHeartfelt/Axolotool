#pragma once

#include "cell/NDCell.h"
#include "node/NDNode.h"
#include "node_source/NDNodeSource.h"

#include "pin/NDPin.h"
#include "pin_source/NDPinSource.h"

#include "widget/NDWidget.h"
#include "widget_source/NDWidgetSource.h"

#include "wire/NDWire.h"
#include "wire_source/NDWireSource.h"

#include "global_source/NDGlobalSource.h"

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
namespace NDWire {         struct ComponentFriendTag; }
namespace NDWireSource {   struct ComponentFriendTag; }
namespace NDGlobalSource { struct ComponentFriendTag; }

namespace ANodeEnvDB {

	class ANodeEnvDB {
		QString connectionName;
		QString dbPath;

        bool createCoreTables() {
            if (!globalSource.createAllTables()) return false;
            if (!nodeSource.createAllTables())   return false;
            if (!node.createAllTables())         return false;
            if (!cell.createAllTables())         return false;
            if (!pinSource.createAllTables())    return false;
            if (!pin.createAllTables())          return false;
            if (!widgetSource.createAllTables()) return false;
            if (!widget.createAllTables())       return false;
            if (!wireSource.createAllTables())   return false;
            if (!wire.createAllTables())         return false;
            return true;
        }
        bool configureDatabasePragmas(QSqlQuery& query) {
            if (!query.exec("PRAGMA foreign_keys = ON;")) {
                qCritical() << "CRITICAL: Failed to enable Foreign Keys:" << query.lastError().text();
                return false;
            }
            if (!query.exec("PRAGMA journal_mode=WAL;")) {
                qCritical() << "CRITICAL: Failed to enable WAL mode:" << query.lastError().text();
                return false;
            }
            return true;
        }


    public:
        struct Version {
            static unsigned int current() {
                return NDConfig::currentSchemaVersion();
            }
            static std::optional<unsigned int> tryReadSchemaVersion(QSqlQuery& query) {
                if (!query.exec("PRAGMA user_version;")) {
                    qCritical() << "Failed to read database schema version:" << query.lastError().text();
                    return std::nullopt;
                }

                if (!query.next()) {
                    qCritical() << "Database failed to return user_version row.";
                    return std::nullopt;
                }

                return query.value(0).toUInt();
            }

            static bool checkAndRunMigration(QSqlQuery& query) {
                const auto storedVersion = tryReadSchemaVersion(query);
                if (!storedVersion) return false;

                if (*storedVersion != current()) {
                    /* migrate logic */
                }

                return true;
            }
        };

        class StorageKey {
            friend class NDNode::ComponentFriendTag;
            friend class NDNodeSource::ComponentFriendTag;
            friend class NDCell::ComponentFriendTag;

            friend class NDPin::ComponentFriendTag;
            friend class NDPinSource::ComponentFriendTag;

            friend class NDWidget::ComponentFriendTag;
            friend class NDWidgetSource::ComponentFriendTag;

            friend class NDWire::ComponentFriendTag;
            friend class NDWireSource::ComponentFriendTag;

            friend class NDGlobalSource::ComponentFriendTag;

            StorageKey() = default;
        };

		ANodeEnvDB(const QString& connectionName, const QString& dbPath) : connectionName(connectionName), dbPath(dbPath), 
            node(this), 
            nodeSource(this),
            cell(this),
            pin(this),
            pinSource(this),
            widget(this),
            widgetSource(this),
            wire(this),
            wireSource(this),
            globalSource(this)
        {}

		~ANodeEnvDB() {
			close();
		}

        QSqlDatabase getDatabase(StorageKey) const { return QSqlDatabase::database(connectionName); }

        NDNode::Component<ANodeEnvDB>         node;
        NDNodeSource::Component<ANodeEnvDB>   nodeSource;
        NDCell::Component<ANodeEnvDB>         cell;

        NDPin::Component<ANodeEnvDB>          pin;
        NDPinSource::Component<ANodeEnvDB>    pinSource;

        NDWidget::Component<ANodeEnvDB>       widget;
        NDWidgetSource::Component<ANodeEnvDB> widgetSource;

        NDWire::Component<ANodeEnvDB>         wire;
        NDWireSource::Component<ANodeEnvDB>   wireSource;

        NDGlobalSource::Component<ANodeEnvDB> globalSource;

        bool isOpen() const {
            if (QSqlDatabase::contains(connectionName)) {
                return QSqlDatabase::database(connectionName).isOpen();
            }
            return false;
        }
        bool open() {
            if (isOpen()) return true;

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

            if (!configureDatabasePragmas(query))      return false;
            if (!createCoreTables())                   return false;
            if (!Version::checkAndRunMigration(query)) return false;

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