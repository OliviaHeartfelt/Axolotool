#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDCellDetails::Helper {

    inline bool isCellAvailable(QSqlQuery& query, const muuid::uuid& nodeId, const NDCellDetails::Config::CellInfo& cellInfo, const std::optional<muuid::uuid>& ignoreCellId = std::nullopt) {
        QString checkSQL = R"(
            SELECT 1 FROM node_cells
            WHERE node_id = :node_id
              AND :new_row < (layout_row + layout_row_span)
              AND (:new_row + :new_row_span) > layout_row
              AND :new_col < (layout_col + layout_col_span)
              AND (:new_col + :new_col_span) > layout_col
        )";
        if (ignoreCellId) {
            checkSQL += " AND cellInfo_id != :ignore_id";
        }
        checkSQL += " LIMIT 1;";
        query.prepare(checkSQL);

        query.bindValue(":node_id",      Utility::UUID::uuidToBytes(nodeId));
        query.bindValue(":new_row",      cellInfo.row);
        query.bindValue(":new_row_span", cellInfo.rowSpan);
        query.bindValue(":new_col",      cellInfo.col);
        query.bindValue(":new_col_span", cellInfo.colSpan);
        if (ignoreCellId)
            query.bindValue(":ignore_id", Utility::UUID::uuidToBytes(*ignoreCellId));

        return !(query.exec() && query.next());
    }
    inline bool removeCollidingCells(QSqlQuery& query, const muuid::uuid& nodeId, const NDCellDetails::Config::CellInfo& cell) {
        query.prepare(R"(
            DELETE FROM node_cells
            WHERE node_id = :node_id
              AND :new_row < (layout_row + layout_row_span)
              AND (:new_row + :new_row_span) > layout_row
              AND :new_col < (layout_col + layout_col_span)
              AND (:new_col + :new_col_span) > layout_col;
        )");
        query.bindValue(":node_id",      Utility::UUID::uuidToBytes(nodeId));
        query.bindValue(":new_row",      cell.row);
        query.bindValue(":new_row_span", cell.rowSpan);
        query.bindValue(":new_col",      cell.col);
        query.bindValue(":new_col_span", cell.colSpan);

        if (!query.exec()) {
            qWarning() << "Failed to evict overlapping cells during overwrite:" << query.lastError().text();
            return false;
        }
        return true;
    }
}