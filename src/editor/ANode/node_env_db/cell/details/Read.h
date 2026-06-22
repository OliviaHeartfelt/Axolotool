#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDCellDetails::Read {

    inline std::optional<Config::Record> get(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT node_id, layout_row, layout_col, layout_row_span, layout_col_span, cell_type 
            FROM node_cells 
            WHERE cell_id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));
    
        if (!query.exec() || !query.next()) return std::nullopt;
    
        auto nodeIdOpt = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!nodeIdOpt) return std::nullopt;
    
        return NDCellDetails::Config::Record{
            id,
            *nodeIdOpt,
            static_cast<short>(query.value(1).toInt()),
            static_cast<short>(query.value(2).toInt()),
            static_cast<short>(query.value(3).toInt()),
            static_cast<short>(query.value(4).toInt()),
            query.value(5).toString()
        };
    }
    inline std::optional<QList<Config::Record>> getAll(QSqlQuery& query, const muuid::uuid& nodeId, const bool continueAtFail = true) {
        QList<Config::Record> cells;
        query.prepare(R"(
            SELECT cell_id, layout_row, layout_col, layout_row_span, layout_col_span, cell_type 
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
                if (continueAtFail) continue;
                else return std::nullopt;
            }
    
            cells.append(Config::Record{
                *cellIdOpt,
                nodeId,
                static_cast<short>(query.value(1).toInt()),
                static_cast<short>(query.value(2).toInt()),
                static_cast<short>(query.value(3).toInt()),
                static_cast<short>(query.value(4).toInt()),
                query.value(5).toString()
                });
        }
        return cells;
    }
}