#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDCellDetails::Read {

    inline std::optional<NDCellDetails::Config::FullCellRecord> getCell(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT node_id, name, is_out, layout_row, layout_col, layout_row_span, layout_col_span, pin_id, widget_id
            FROM node_cells 
            WHERE cell_id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to fetch cell:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        auto nodeIdOpt = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!nodeIdOpt) return std::nullopt;

        NDCellDetails::Config::CellContent cellContent;
        if (!query.value(7).isNull()) {
            if (auto pinId = Utility::UUID::bytesToUuid(query.value(7).toByteArray()))
                cellContent = NDCellDetails::Config::PinItemRecord{ *pinId };
        }
        else if (!query.value(8).isNull()) {
            if (auto widgetId = Utility::UUID::bytesToUuid(query.value(8).toByteArray()))
                cellContent = NDCellDetails::Config::WidgetRecord{ *widgetId };
        }

        return NDCellDetails::Config::FullCellRecord{
            id,
            *nodeIdOpt,
            query.value(1).toString(),
            query.value(2).toBool(),
            static_cast<short>(query.value(3).toInt()),
            static_cast<short>(query.value(4).toInt()),
            static_cast<short>(query.value(5).toInt()),
            static_cast<short>(query.value(6).toInt()),
            cellContent
        };
    }

    inline std::optional<QList<Config::FullCellRecord>> getAllCells(QSqlQuery& query, const muuid::uuid& nodeId, const bool continueAtFail = true) {
        QList<Config::FullCellRecord> cells;
        query.prepare(R"(
            SELECT cell_id, name, is_out, layout_row, layout_col, layout_row_span, layout_col_span, pin_id, widget_id
            FROM node_cells 
            WHERE node_id = :node_id;
        )");
        query.bindValue(":node_id", Utility::UUID::uuidToBytes(nodeId));

        if (!query.exec()) {
            qWarning() << "Failed to fetch cells for node:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            auto cellIdOpt = Utility::UUID::bytesToUuid(query.value(0).toByteArray());

            if (!cellIdOpt) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            NDCellDetails::Config::CellContent cellContent;
            if (!query.value(7).isNull()) {
                if (auto pinId = Utility::UUID::bytesToUuid(query.value(7).toByteArray()))
                    cellContent = NDCellDetails::Config::PinItemRecord{ *pinId };
            }
            else if (!query.value(8).isNull()) {
                if (auto widgetId = Utility::UUID::bytesToUuid(query.value(8).toByteArray()))
                    cellContent = NDCellDetails::Config::WidgetRecord{ *widgetId };
            }

            cells.append(Config::FullCellRecord{
                *cellIdOpt,
                nodeId,
                query.value(1).toString(),
                query.value(2).toBool(),
                static_cast<short>(query.value(3).toInt()),
                static_cast<short>(query.value(4).toInt()),
                static_cast<short>(query.value(5).toInt()),
                static_cast<short>(query.value(6).toInt()),
                cellContent
            });
        }
        return cells;
    }
}