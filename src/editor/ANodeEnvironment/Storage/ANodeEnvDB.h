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
#include "NDHelpers.h"
#include "NDPool.h"

namespace ANodeEnvDB {

    namespace Helpers {
        using namespace ::NDHelpers;
    }

    namespace Config {
        namespace GlobalSource {using namespace ::NDGlobalSource::Config; }
        namespace NodeSource {  using namespace ::NDNodeSource::Config; }
        namespace Node {        using namespace ::NDNode::Config; }
        namespace Cell {        using namespace ::NDCell::Config; }
        namespace PinSource {   using namespace ::NDPinSource::Config; }
        namespace Pin {         using namespace ::NDPin::Config; }
        namespace WidgetSource {using namespace ::NDWidgetSource::Config; }
        namespace Widget {      using namespace ::NDWidget::Config; }
        namespace WireSource {  using namespace ::NDWireSource::Config; }
        namespace Wire {        using namespace ::NDWire::Config; }
    }

	class ANodeEnvDB {
		QString m_connectionBaseName;
        std::unique_ptr<NDPool::DatabasePool> pool;
		QString dbPath;

        bool createCoreTables(QSqlDatabase& db) {
            QSqlQuery query(db);

            if (!globalSource.createAllTables(query)) return false;
            if (!nodeSource.createAllTables(query))   return false;
            if (!node.createAllTables(query))         return false;
            if (!cell.createAllTables(query))         return false;
            if (!pinSource.createAllTables(query))    return false;
            if (!pin.createAllTables(query))          return false;
            if (!widgetSource.createAllTables(query)) return false;
            if (!widget.createAllTables(query))       return false;
            if (!wireSource.createAllTables(query))   return false;
            if (!wire.createAllTables(query))         return false;
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

		ANodeEnvDB(const QString& dbPath, const QString& connectionBaseName) :
            dbPath(dbPath), m_connectionBaseName(connectionBaseName),
            node(this), nodeSource(this), cell(this), pin(this), pinSource(this), widget(this), widgetSource(this), wire(this), wireSource(this), globalSource(this)
        {}

		~ANodeEnvDB() {
			close();
		}

        NDPool::DatabasePool& getPool() const { return *pool; }

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
            if (QSqlDatabase::contains(m_connectionBaseName)) {
                return QSqlDatabase::database(m_connectionBaseName).isOpen();
            }
            return false;
        }
        bool open(int poolSize = 4) {
            if (pool) return true;

            pool = std::make_unique<NDPool::DatabasePool>(dbPath, m_connectionBaseName, poolSize);

            auto lease = pool->acquire();
            QSqlQuery query(lease.db());

            if (!createCoreTables(lease.db())) return false;

            if (!Version::checkAndRunMigration(query)) return false;

            return true;
        }

        void close() {
            if (QSqlDatabase::contains(m_connectionBaseName)) {
                auto db = QSqlDatabase::database(m_connectionBaseName);
                if (db.isOpen()) db.close();
            }
        }
	};
    static_assert(NDConcepts::DatabaseProvider<ANodeEnvDB>);
}