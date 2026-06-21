export module ANodeEnvDB;

import NDNode;
import NDCell;
import NDConfig;

export namespace ANodeEnvDB {

	class ANodeEnvDB {
		QString connectionName;
		QString dbPath;

        QSqlDatabase getDatabase() const { return QSqlDatabase::database(connectionName); }

	public:
		ANodeEnvDB(const QString& connectionName, const QString& dbPath) : connectionName(connectionName), dbPath(dbPath) {}
		~ANodeEnvDB() {
			close();
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
        std::optional<int> createNode(const QString& title, const short rowNum, const short colNum, const QList<NDConfig::CellSpawnInfo>& cells = {}, const QPointF pos = { 0.0, 0.0 }) {           
            QSqlDatabase db = getDatabase();
            if (!db.transaction()) { 
                qWarning() << "Could not start database transaction.";
                return std::nullopt;
            }

            QSqlQuery query(db);

            std::optional<int> newNodeId = NDNode::create(query, title, rowNum, colNum, pos);
            if (!newNodeId) { 
                db.rollback(); 
                return std::nullopt;
            }

            for (const auto& cell : cells) {
                if (!NDCell::create(db, query, newNodeId.value(), cell)) {
                    qWarning() << "Aborting entire node topology build due to cell insertion failure.";
                    db.rollback();
                    return std::nullopt;
                }
            }

            if (!db.commit()) {
                qWarning() << "Failed to commit node topology transaction.";
                db.rollback();
                return std::nullopt;
            }
            return newNodeId;
        }
        //bool removeNode() {

        //}

        // Cell
        std::optional<int> createCell(const int nodeId, const NDConfig::CellSpawnInfo& cell, bool overrideOnCollision = false) {
            QSqlDatabase db = getDatabase();
            if (!db.transaction()) {
                qWarning() << "Failed to start transaction for standalone cell creation.";
                return std::nullopt;
            }

            QSqlQuery query(db);

            std::optional<int> insertedId = std::nullopt;
            if (auto insertedId = NDCell::create(db, query, nodeId, cell, overrideOnCollision)) {

                if (!db.commit()) {
                    qWarning() << "Failed to commit cell transaction to disk.";
                    db.rollback();
                    return std::nullopt;
                }
                return insertedId;
            }

            db.rollback();
            return std::nullopt;
        }
        //bool removeCell() {

        //}

    private:
        bool createCoreTables() {
            QSqlDatabase db = getDatabase();
            if (containsAllCoreTables(db.tables())) return true;

            if (!db.transaction()) {
                qWarning() << "Failed to start schema initialization transaction.";
                return false;
            }

            QSqlQuery query(getDatabase());

            if (!NDNode::createTable(query) || !NDCell::createTable(query)) {
                qCritical() << "Schema initialization failed! Rolling back changes.";
                db.rollback();
                return false;
            }

            if (!db.commit()) {
                qCritical() << "Failed to commit schema initialization transaction.";
                db.rollback();
                return false;
            }

            return true;
        }
        bool containsAllCoreTables(const QStringList& strList) {
            if (!strList.contains("nodes", Qt::CaseInsensitive)) return false;
            if (!strList.contains("node_cells", Qt::CaseInsensitive)) return false;
            return true;
        }
	};
}