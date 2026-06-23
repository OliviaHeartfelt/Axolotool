#pragma once

#include "Helper.h"
#include "Config.h"

namespace NDCellDetails::Create {

    static std::optional<muuid::uuid> create(QSqlQuery& query, const muuid::uuid& nodeId, const Config::RecordInfo& cellInfo, bool overrideOnCollision = false) {
        if (!Helper::isCellAvailable(query, nodeId, cellInfo)) {
            if (!overrideOnCollision) {
                qWarning() << "Cell insertion rejected: Space is occupied.";
                return std::nullopt;
            }
            if (!Helper::removeCollidingCells(query, nodeId, cellInfo)) return std::nullopt;
        }
    
        query.prepare(R"(
            INSERT INTO node_cells (cell_id, node_id, layout_row, layout_col, layout_row_span, layout_col_span)
            VALUES (:cell_id, :node_id, :row, :col, :row_span, :col_span);
        )");
        muuid::uuid newCellId = muuid::uuid::generate_unix_time_based();
    
        query.bindValue(":cell_id", Utility::UUID::uuidToBytes(newCellId));
        query.bindValue(":node_id", Utility::UUID::uuidToBytes(nodeId));
        query.bindValue(":row", cellInfo.row);
        query.bindValue(":col", cellInfo.col);
        query.bindValue(":row_span", cellInfo.rowSpan);
        query.bindValue(":col_span", cellInfo.colSpan);
    
        if (!query.exec()) {
            qWarning() << "Failed to execute Cell creation query:" << query.lastError().text();
            return std::nullopt;
        }
        return newCellId;
    }
}