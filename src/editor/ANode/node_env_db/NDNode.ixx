export module NDNode;

export namespace NDNode {

    bool create(QSqlQuery& query, const QString& title, const short rowNum, const short colNum, const QPointF pos = { 0.0, 0.0 }) {
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

    void remove(QSqlDatabase& db, const int ID) {
        QSqlQuery query(db);
        query.prepare("DELETE FROM nodes WHERE node_id = :id;");
        query.bindValue(":id", ID);

        if (!query.exec()) {
            qWarning() << "Failed to remove node:" << query.lastError().text();
        }
    }
}