export module ANodeEnvDB;

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

        struct CellSpawnInfo {
            QString cellType;
            short row;
            short col;
            short rowSpan = 1;
            short colSpan = 1;
        };

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

            if (!query.exec("PRAGMA foreign_keys = ON;")) {
                qWarning() << "Failed to enable Foreign Keys:" << query.lastError().text();
            }

            if (!query.exec("PRAGMA journal_mode=WAL;")) {
                qWarning() << "Failed to enable WAL mode:" << query.lastError().text();
            }

            return createCoreTables();
        }

        void close() {
            if (QSqlDatabase::contains(connectionName)) {
                auto db = QSqlDatabase::database(connectionName);
                if (db.isOpen()) db.close();
            }
        }

        // Node
        int createNode(const QString& title, const short rowNum, const short colNum, const QList<CellSpawnInfo>& cells, const QPointF pos = { 0.0, 0.0 }) {
            QSqlDatabase db = getDatabase();
            if (!db.transaction()) { 
                qWarning() << "Could not start database transaction.";
                return -1;
            }

            QSqlQuery query(db);

            int newNodeId = createNodeHelper(query, title, rowNum, colNum, pos);
            if (newNodeId == -1) { 
                db.rollback(); 
                return -1;
            }

            for (const auto& cell : cells) {
                if (!createCellHelper(db, query, newNodeId, cell)) {
                    qWarning() << "Aborting entire node topology build due to cell insertion failure.";
                    db.rollback();
                    return -1;
                }
            }

            if (!db.commit()) {
                qWarning() << "Failed to commit node topology transaction.";
                db.rollback();
                return -1;
            }
            return newNodeId;
        }

        // Cell
        int createCell(const int nodeId, const CellSpawnInfo& cell, bool overrideOnCollision = false) {
            QSqlDatabase db = getDatabase();
            if (!db.transaction()) {
                qWarning() << "Failed to start transaction for standalone cell creation.";
                return -1;
            }

            QSqlQuery query(db);
            if (!createCellHelper(db, query, nodeId, cell, overrideOnCollision)) {
                db.rollback();
                return -1;
            }

            if (!db.commit()) {
                qWarning() << "Failed to commit cell transaction to disk.";
                db.rollback();
                return -1;
            }
            return query.lastInsertId().toInt();
        }

    private:
        bool createCoreTables() {
            QSqlQuery query(getDatabase());

            QString createNodesTable = R"(
                CREATE TABLE IF NOT EXISTS nodes (
                    node_id INTEGER PRIMARY KEY AUTOINCREMENT,
                    title TEXT NOT NULL,
                    row_num SMALLINT NOT NULL,
                    col_num SMALLINT NOT NULL,
                    canvas_x REAL DEFAULT 0.0,
                    canvas_y REAL DEFAULT 0.0,
                    canvas_w REAL DEFAULT -1.0,
                    canvas_h REAL DEFAULT -1.0
                );
            )";

            if (!query.exec(createNodesTable)) {
                qCritical() << "Failed to create nodes table:" << query.lastError().text();
                return false;
            }

            QString createCellsTable = R"(
                CREATE TABLE IF NOT EXISTS node_cells (
                    cell_id INTEGER PRIMARY KEY AUTOINCREMENT,
                    node_id INTEGER NOT NULL,
                    layout_row SMALLINT NOT NULL,
                    layout_col SMALLINT NOT NULL,
                    layout_row_span SMALLINT NOT NULL,
                    layout_col_span SMALLINT NOT NULL,
                    cell_type TEXT NOT NULL,
                    FOREIGN KEY(node_id) REFERENCES nodes(node_id) ON DELETE CASCADE,
                    UNIQUE(node_id, layout_row, layout_col)
                );
            )";

            if (!query.exec(createCellsTable)) {
                qCritical() << "Failed to create node_cells table:" << query.lastError().text();
                return false;
            }

            return true;
        }

        bool isNodeSpaceAvailable(int nodeId, short row, short col, short rowSpan, short colSpan) {
            QSqlQuery query(getDatabase());
            query.prepare(R"(
                SELECT 1 FROM node_cells
                WHERE node_id = :node_id
                  AND :new_row < (layout_row + layout_row_span)
                  AND (:new_row + :new_row_span) > layout_row
                  AND :new_col < (layout_col + layout_col_span)
                  AND (:new_col + :new_col_span) > layout_col
                LIMIT 1;
            )");

            query.bindValue(":node_id", nodeId);
            query.bindValue(":new_row", row);
            query.bindValue(":new_row_span", rowSpan);
            query.bindValue(":new_col", col);
            query.bindValue(":new_col_span", colSpan);

            return !(query.exec() && query.next());
        }

        bool createNodeHelper(QSqlQuery& query, const QString& title, const short rowNum, const short colNum, const QPointF pos = { 0.0, 0.0 }) {
            query.prepare(R"(
                INSERT INTO nodes (title, row_num, col_num, canvas_x, canvas_y, canvas_w, canvas_h)
                VALUES (:title, :row, :col, :x, :y, -1.0, -1.0);
            )");
            query.bindValue(":title", title);
            query.bindValue(":row", rowNum);
            query.bindValue(":col", colNum);
            query.bindValue(":x", pos.x());
            query.bindValue(":y", pos.y());

            if (!query.exec()) {
                qWarning() << "Failed to execute Node creation query:" << query.lastError().text();
                return true;
            }
            return query.lastInsertId().toInt();
        }
        bool createCellHelper(QSqlDatabase& db, QSqlQuery& query, const int nodeId, const CellSpawnInfo& cell, bool overrideOnCollision = false) {

            if (!isNodeSpaceAvailable(nodeId, cell.row, cell.col, cell.rowSpan, cell.colSpan)) {
                if (!overrideOnCollision) {
                    qWarning() << "Cell insertion rejected: Space is occupied.";
                    return false;
                }
                QSqlQuery deleteQuery(db);
                deleteQuery.prepare(R"(
                    DELETE FROM node_cells
                    WHERE node_id = :node_id
                      AND :new_row < (layout_row + layout_row_span)
                      AND (:new_row + :new_row_span) > layout_row
                      AND :new_col < (layout_col + layout_col_span)
                      AND (:new_col + :new_col_span) > layout_col;
                )");
                deleteQuery.bindValue(":node_id", nodeId);
                deleteQuery.bindValue(":new_row", cell.row);
                deleteQuery.bindValue(":new_row_span", cell.rowSpan);
                deleteQuery.bindValue(":new_col", cell.col);
                deleteQuery.bindValue(":new_col_span", cell.colSpan);

                if (!deleteQuery.exec()) {
                    qWarning() << "Failed to evict overlapping cells during overwrite:" << deleteQuery.lastError().text();
                    return false;
                }
            }

            query.prepare(R"(
                INSERT INTO node_cells (node_id, layout_row, layout_col, layout_row_span, layout_col_span, cell_type)
                VALUES (:node_id, :row, :col, :row_span, :col_span, :type);
            )");
            query.bindValue(":node_id", nodeId);
            query.bindValue(":row", cell.row);
            query.bindValue(":col", cell.col);
            query.bindValue(":row_span", cell.rowSpan);
            query.bindValue(":col_span", cell.colSpan);
            query.bindValue(":type", cell.cellType);

            if (!query.exec()) {
                qWarning() << "Failed to execute Cell creation query:" << query.lastError().text();
                return false;
            }
            return true;
        }
	};
}