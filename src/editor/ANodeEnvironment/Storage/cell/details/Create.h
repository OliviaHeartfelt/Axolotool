#pragma once

#include "Helper.h"
#include "Config.h"

namespace NDCellDetails::Create {

    inline bool createCell(QSqlQuery& query, const Config::CreateCellRecord& newCell, const bool overrideOnCollision = false) {
        if (newCell.pinCoreId && newCell.widgetId) return false;

        if (!Helper::isCellAvailable(query, newCell.nodeId, newCell.row, newCell.col, newCell.rowSpan, newCell.colSpan)) {
            if (!overrideOnCollision) {
                qWarning() << "Cell insertion rejected: Space is occupied.";
                return false;
            }
            if (!Helper::removeCollidingCells(query, newCell.nodeId, newCell.row, newCell.col, newCell.rowSpan, newCell.colSpan)) return false;
        }

        query.prepare(R"(
            INSERT OR IGNORE INTO cell (id,  node_id,  name,  layout_row,  layout_col,  layout_row_span,  layout_col_span,  pin_core_id,  widget_id)
            VALUES (                   :id, :node_id, :name, :layout_row, :layout_col, :layout_row_span, :layout_col_span, :pin_core_id, :widget_id);
        )");

        query.bindValue(":id", Utility::UUID::uuidToBytes(newCell.id));
        query.bindValue(":node_id", Utility::UUID::uuidToBytes(newCell.nodeId));
        query.bindValue(":name", newCell.name ? *newCell.name : QVariant());

        query.bindValue(":layout_row", newCell.row);
        query.bindValue(":layout_col", newCell.col);
        query.bindValue(":layout_row_span", newCell.rowSpan);
        query.bindValue(":layout_col_span", newCell.colSpan);

        query.bindValue(":pin_core_id", newCell.pinCoreId ? Utility::UUID::uuidToBytes(*newCell.pinCoreId) : QVariant());
        query.bindValue(":widget_id", newCell.widgetId ? Utility::UUID::uuidToBytes(*newCell.widgetId) : QVariant());

        if (!query.exec()) {
            qWarning() << "Failed to execute create cell:" << query.lastError().text();
            return false;
        }
        return true;
    }

    inline bool createCellOrigin(QSqlQuery& query, const Config::CreateCellOriginRecord& newCell) {
        if (newCell.pinCoreId && newCell.widgetCoreId) return false;

        if (!Helper::isCellOriginAvailable(query, newCell.nodeCoreId, newCell.row, newCell.col, newCell.rowSpan, newCell.colSpan)) {
            qWarning() << "Cell origin insertion rejected: Space is occupied.";
            return false;
        }

        query.prepare(R"(
            INSERT OR IGNORE INTO cell_origin (id,  node_core_id,  name,  layout_row,  layout_col,  layout_row_span,  layout_col_span,  pin_core_id,  widget_core_id)
            VALUES (                          :id, :node_core_id, :name, :layout_row, :layout_col, :layout_row_span, :layout_col_span, :pin_core_id, :widget_core_id);
        )");

        query.bindValue(":id", Utility::UUID::uuidToBytes(newCell.id));
        query.bindValue(":node_core_id", Utility::UUID::uuidToBytes(newCell.nodeCoreId));
        query.bindValue(":name", newCell.name ? *newCell.name : QVariant());

        query.bindValue(":layout_row", newCell.row);
        query.bindValue(":layout_col", newCell.col);
        query.bindValue(":layout_row_span", newCell.rowSpan);
        query.bindValue(":layout_col_span", newCell.colSpan);

        query.bindValue(":pin_core_id", newCell.pinCoreId ? Utility::UUID::uuidToBytes(*newCell.pinCoreId) : QVariant());
        query.bindValue(":widget_core_id", newCell.widgetCoreId ? Utility::UUID::uuidToBytes(*newCell.widgetCoreId) : QVariant());

        if (!query.exec()) {
            qWarning() << "Failed to execute create cell origin:" << query.lastError().text();
            return false;
        }
        return true;
    }
}