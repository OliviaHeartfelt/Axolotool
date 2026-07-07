#pragma once

#include "Helper.h"
#include "Config.h"

namespace NDCellDetails::Create {

    inline bool create(QSqlQuery& query, const Config::CreateCellRecord& newCell, const bool overrideOnCollision = false) {
        if (newCell.pinId && newCell.widgetId) return false;

        const NDCellDetails::Config::CellInfo info{
            newCell.row,
            newCell.col,
            newCell.rowSpan,
            newCell.colSpan
        };
        
        if (!Helper::isCellAvailable(query, newCell.nodeId, info)) {
            if (!overrideOnCollision) {
                qWarning() << "Cell insertion rejected: Space is occupied.";
                return false;
            }
            if (!Helper::removeCollidingCells(query, newCell.nodeId, info)) return false;
        }
    
        query.prepare(R"(
            INSERT INTO node_cells (id,  node_id,  name,  layout_row,  layout_col,  layout_row_span,  layout_col_span,  pin_id,  widget_id,  is_out)
            VALUES (               :id, :node_id, :name, :layout_row, :layout_col, :layout_row_span, :layout_col_span, :pin_id, :widget_id, :is_out);
        )");

        query.bindValue(":id",              Utility::UUID::uuidToBytes(newCell.id));
        query.bindValue(":node_id",         Utility::UUID::uuidToBytes(newCell.nodeId));
        query.bindValue(":name",            newCell.name ? *newCell.name : QVariant());

        query.bindValue(":layout_row",      info.row);
        query.bindValue(":layout_col",      info.col);
        query.bindValue(":layout_row_span", info.rowSpan);
        query.bindValue(":layout_col_span", info.colSpan);
        query.bindValue(":is_out",          newCell.isOut? 1 : 0);

        query.bindValue(":pin_id",          newCell.pinId ?    Utility::UUID::uuidToBytes(*newCell.pinId) :    QVariant());
        query.bindValue(":widget_id",       newCell.widgetId ? Utility::UUID::uuidToBytes(*newCell.widgetId) : QVariant());
    
        if (!query.exec()) {
            qWarning() << "Failed to execute create cell:" << query.lastError().text();
            return false;
        }
        return true;
    }
}