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
#include "NDParser.h"

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

    struct ANodeEnvDBVersion {
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

	class ANodeEnvDB {
		QString m_connectionBaseName;
        std::unique_ptr<NDPool::DatabasePool> pool;
		QString m_dbPath;
        QString m_pluginPath;

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

        bool importDoc(QSqlQuery& query, const QJsonObject& doc) {
            
            if (!globalSource.importGlobalSource(doc, query)) return false;

            if (!nodeSource.importNodeSource(doc, query)) return false;
            if (!nodeSource.importNodeContributor(doc, query)) return false;
            if (!nodeSource.importNodeData(doc, query)) return false;
            if (!nodeSource.importNodeType(doc, query)) return false;
            if (!node.importNodeCore(doc, query)) return false;

            if (!pinSource.importPinSource(doc, query)) return false;
            if (!pinSource.importPinContributor(doc, query)) return false;
            if (!pinSource.importPinFlow(doc, query)) return false;
            if (!pinSource.importPinType(doc, query)) return false;
            if (!pinSource.importPinStyle(doc, query)) return false;
            if (!pin.importPinCore(doc, query)) return false;

            if (!widgetSource.importWidgetSource(doc, query)) return false;
            if (!widgetSource.importWidgetContributor(doc, query)) return false;
            if (!widgetSource.importWidgetData(doc, query)) return false;
            if (!widgetSource.importWidgetType(doc, query)) return false;
            if (!widget.importWidgetCore(doc, query)) return false;

            if (!wireSource.importWireSource(doc, query)) return false;
            if (!wireSource.importWireContributor(doc, query)) return false;
            if (!wireSource.importWireData(doc, query)) return false;
            if (!wireSource.importWireStyle(doc, query)) return false;
            if (!wire.importWireCore(doc, query)) return false;
            if (!wire.importSymmetricWire(doc, query)) return false;
            if (!wire.importAsymmetricWire(doc, query)) return false;

            return true;
        }

    public:
		ANodeEnvDB(const QString& dbPath, const QString& connectionBaseName, const QString pluginRelativePath = QStringLiteral("plugin")) :
            m_connectionBaseName(connectionBaseName),
            m_dbPath(dbPath),
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
        {
            QString appDir = QCoreApplication::applicationDirPath();
            m_pluginPath = QDir(appDir).filePath(pluginRelativePath);
        }

		~ANodeEnvDB() {
			close();
		}

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

        bool open(int poolSize = 4, bool loadManifets = true) {
            if (pool) return true;

            pool = std::make_unique<NDPool::DatabasePool>(m_dbPath, m_connectionBaseName, poolSize);
            {
                auto lease = pool->acquire();

                if (!lease.db().isOpen()) {
                    pool.reset();
                    return false;
                }

                QSqlQuery query(lease.db());

                if (!createCoreTables(lease.db())) {
                    pool.reset();
                    return false;
                }

                if (!ANodeEnvDBVersion::checkAndRunMigration(query)) {
                    pool.reset();
                    return false;
                }
            }
            if (loadManifets) loadAllManifests();

            return true;
        }

        void close() {
            if (pool) {
                pool.reset();
            }
        }

        bool isOpen() const {
            return pool != nullptr;
        }
        NDPool::DatabasePool& getPool() const { return *pool; }

        bool loadManifest(QSqlQuery& query, const QString& fileName) {
            QString filePath = m_pluginPath + "/" + fileName + ".json";;
            QFileInfo fileInfo(filePath);

            QString canonicalPath = fileInfo.canonicalFilePath();
            if (canonicalPath.isEmpty()) return false;

            QString canonicalRoot = QFileInfo(m_pluginPath).canonicalFilePath();
            if (canonicalRoot.isEmpty() || !canonicalPath.startsWith(canonicalRoot + "/")) {
                qWarning() << "Rejected manifest outside plugin directory:" << canonicalPath;
                return false;
            }

            auto optDoc = NDParser::parseManifest(canonicalPath);
            if (!optDoc) return false;

            return importDoc(query, *optDoc);
        }
        bool loadManifest(const QString& fileName) {
            return NDHelpers::useTransaction(getPool(), [&](QSqlQuery& query) {
                return loadManifest(query, fileName);
            });
        }

        bool loadAllManifests(QSqlQuery& query) {
            QDirIterator it(m_pluginPath, QStringList() << "*.json", QDir::Files, QDirIterator::Subdirectories);
            bool allOk = true;
            while (it.hasNext()) {
                it.next();
                QString canonicalPath = it.fileInfo().canonicalFilePath();
                if (canonicalPath.isEmpty()) { 
                    allOk = false;
                    continue;
                }

                QString canonicalRoot = QFileInfo(m_pluginPath).canonicalFilePath();
                if (canonicalRoot.isEmpty() || !canonicalPath.startsWith(canonicalRoot + "/")) {
                    qWarning() << "Rejected manifest outside plugin directory:" << canonicalPath;
                    allOk = false;
                    continue;
                }
                
                auto optDoc = NDParser::parseManifest(canonicalPath);
                if (!optDoc) { 
                    allOk = false;
                    continue; 
                }
                if (!importDoc(query, *optDoc)) allOk = false;
            }
            return allOk;
        }
        bool loadAllManifests() {
            return NDHelpers::useTransaction(getPool(), [&](QSqlQuery& query) {
                return loadAllManifests(query);
            });
        }
	};
    static_assert(NDConcepts::DatabaseProvider<ANodeEnvDB>);
}