#pragma once

#include "Config.h"
#include "Helper.h"

namespace NDCellDetails::Update {

    inline bool updateCell(QSqlQuery& query, const muuid::uuid& id, const muuid::uuid& nodeId, const NDCellDetails::Config::UpdateCellRecord& newCellInfo, const bool overrideOnCollision = false) {
        
        if (newCellInfo.pinId && newCellInfo.widgetId) {
            qWarning() << "Cell update rejected: A cell slot cannot contain both a Pin and a Widget.";
            return false;
        }

        const bool becomeHidden = newCellInfo.isOut;
        if (!becomeHidden && newCellInfo.row && newCellInfo.col && newCellInfo.rowSpan && newCellInfo.colSpan) {
            const NDCellDetails::Config::CellInfo info{
                *newCellInfo.row,
                *newCellInfo.col,
                *newCellInfo.rowSpan,
                *newCellInfo.colSpan
            };

            if (!NDCellDetails::Helper::isCellAvailable(query, nodeId, info, id)) {
                if (!overrideOnCollision) {
                    qWarning() << "Cell layout update rejected: Target region is occupied.";
                    return false;
                }
                if (!NDCellDetails::Helper::removeCollidingCells(query, nodeId, info)) {
                    return false;
                }
            }
        }

        QStringList clauses;
        if (newCellInfo.name) clauses.append("name = :name");

        if (newCellInfo.row && *newCellInfo.row >= 0)          clauses.append("row = :row");
        if (newCellInfo.col && *newCellInfo.col >= 0)          clauses.append("col = :col");
        if (newCellInfo.rowSpan && *newCellInfo.rowSpan >= 1)  clauses.append("row_span = :row_span");
        if (newCellInfo.colSpan && *newCellInfo.colSpan >= 1)  clauses.append("col_span = :col_span");

        if (newCellInfo.pinId)    clauses.append("pin_id = :pin_id");
        if (newCellInfo.widgetId) clauses.append("widget_id = :widget_id");
        if (newCellInfo.isOut)    clauses.append("is_out = :is_out");

        if (clauses.isEmpty()) return true; 

        QString updateSql = QString("UPDATE node_cells SET %1 WHERE cell_id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newCellInfo.name) query.bindValue(":name", *newCellInfo.name);

        if (newCellInfo.row && *newCellInfo.row >= 0)         query.bindValue(":row",      *newCellInfo.row);
        if (newCellInfo.col && *newCellInfo.col >= 0)         query.bindValue(":col",      *newCellInfo.col);
        if (newCellInfo.rowSpan && *newCellInfo.rowSpan >= 1) query.bindValue(":row_span", *newCellInfo.rowSpan);
        if (newCellInfo.colSpan && *newCellInfo.colSpan >= 1) query.bindValue(":col_span", *newCellInfo.colSpan);

        if (newCellInfo.pinId)    query.bindValue(":pin_id",    Utility::UUID::uuidToBytes(*newCellInfo.pinId));
        if (newCellInfo.widgetId) query.bindValue(":widget_id", Utility::UUID::uuidToBytes(*newCellInfo.widgetId));
        if (newCellInfo.isOut)    query.bindValue(":is_out",    newCellInfo.isOut ? 1 : 0);

        if (!query.exec()) {
            qWarning() << "Failed to update cell geometry layout:" << query.lastError().text();
            return false;
        }
        return true;
    }
}