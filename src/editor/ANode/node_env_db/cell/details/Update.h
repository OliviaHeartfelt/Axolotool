#pragma once

#include "Config.h"
#include "Helper.h"

namespace NDCellDetails::Update {

    inline bool updateLayout(QSqlQuery& query, const muuid::uuid& id, const muuid::uuid& nodeId, const NDCellDetails::Config::RecordInfo& cellInfo, bool overrideOnCollision = false) {
        if (!NDCellDetails::Helper::isCellAvailable(query, nodeId, cellInfo, id)) {
            if (!overrideOnCollision) {
                qWarning() << "Cell layout update rejected: Target region is occupied.";
                return false;
            }
            if (NDCellDetails::Helper::removeCollidingCells(query, nodeId, cellInfo)) return false;
        }
    
        query.prepare(R"(
            UPDATE node_cells 
            SET layout_row = :row, layout_col = :col, layout_row_span = :row_span, layout_col_span = :col_span
            WHERE cell_id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        query.bindValue(":row", cellInfo.row);
        query.bindValue(":col", cellInfo.col);
        query.bindValue(":row_span", cellInfo.rowSpan);
        query.bindValue(":col_span", cellInfo.colSpan);
    
        if (!query.exec()) {
            qWarning() << "Failed to update cell geometry layout:" << query.lastError().text();
            return false;
        }
        return true;
    }
}