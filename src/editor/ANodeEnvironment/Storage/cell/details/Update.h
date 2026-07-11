#pragma once

#include "Config.h"
#include "Helper.h"

namespace NDCellDetails::Update {

    inline bool updateCell(QSqlQuery& query, const muuid::uuid& id, const NDCellDetails::Config::UpdateCellRecord& newCellInfo, const bool overrideOnCollision = false) {

        if (newCellInfo.pinId && newCellInfo.widgetId) {
            qWarning() << "Cell update rejected: A cell slot cannot contain both a Pin and a Widget.";
            return false;
        }

        const bool layoutChanged = (newCellInfo.row || newCellInfo.col || newCellInfo.rowSpan || newCellInfo.colSpan);
        const bool becomeHidden = newCellInfo.isOut && *newCellInfo.isOut;

        if (!becomeHidden && layoutChanged) {
            query.prepare("SELECT layout_row, layout_col, layout_row_span, layout_col_span, node_id FROM node_cells WHERE id = :id;");
            query.bindValue(":id", Utility::UUID::uuidToBytes(id));

            if(!query.exec() || !query.next()) {
                qWarning() << "Cell layout update rejected: Failed to fetch current layout state.";
                return false;
            }

            const std::optional<muuid::uuid> nodeId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            if (!nodeId) return false;

            const NDCellDetails::Config::CellInfo info{
                newCellInfo.row     ? *newCellInfo.row     : query.value(0).toInt(),
                newCellInfo.col     ? *newCellInfo.col     : query.value(1).toInt(),
                newCellInfo.rowSpan ? *newCellInfo.rowSpan : query.value(2).toInt(),
                newCellInfo.colSpan ? *newCellInfo.colSpan : query.value(3).toInt()
            };

            if (!NDCellDetails::Helper::isCellAvailable(query, *nodeId, info, id)) {
                if (!overrideOnCollision) {
                    qWarning() << "Cell layout update rejected: Target region is occupied.";
                    return false;
                }
                if (!NDCellDetails::Helper::removeCollidingCells(query, *nodeId, info)) {
                    return false;
                }
            }
        }

        QStringList clauses;
        if (newCellInfo.name) clauses.append("name = :name");

        if (newCellInfo.row && *newCellInfo.row >= 0)         clauses.append("layout_row = :row");
        if (newCellInfo.col && *newCellInfo.col >= 0)         clauses.append("layout_col = :col");
        if (newCellInfo.rowSpan && *newCellInfo.rowSpan >= 1) clauses.append("layout_row_span = :row_span");
        if (newCellInfo.colSpan && *newCellInfo.colSpan >= 1) clauses.append("layout_col_span = :col_span");

        if (newCellInfo.isOut)    clauses.append("is_out = :is_out");

        if (newCellInfo.pinId)    clauses.append("pin_id = :pin_id");
        if (newCellInfo.widgetId) clauses.append("widget_id = :widget_id");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE node_cells SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));
        if (newCellInfo.name) query.bindValue(":name", *newCellInfo.name);

        if (newCellInfo.row && *newCellInfo.row >= 0)         query.bindValue(":row", *newCellInfo.row);
        if (newCellInfo.col && *newCellInfo.col >= 0)         query.bindValue(":col", *newCellInfo.col);
        if (newCellInfo.rowSpan && *newCellInfo.rowSpan >= 1) query.bindValue(":row_span", *newCellInfo.rowSpan);
        if (newCellInfo.colSpan && *newCellInfo.colSpan >= 1) query.bindValue(":col_span", *newCellInfo.colSpan);

        if (newCellInfo.isOut)    query.bindValue(":is_out", *newCellInfo.isOut ? 1 : 0);

        if (newCellInfo.pinId)    query.bindValue(":pin_id", Utility::UUID::uuidToBytes(*newCellInfo.pinId));
        if (newCellInfo.widgetId) query.bindValue(":widget_id", Utility::UUID::uuidToBytes(*newCellInfo.widgetId));

        if (!query.exec()) {
            qWarning() << "Failed to update cell:" << query.lastError().text();
            return false;
        }
        return true;
    }
}