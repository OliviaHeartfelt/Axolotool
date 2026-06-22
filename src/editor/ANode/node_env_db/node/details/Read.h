#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDNodeDetails::Read {

    inline std::optional<NDNodeDetails::Config::Record> get(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT title, row_num, col_num, canvas_x, canvas_y, canvas_w, canvas_h 
            FROM nodes 
            WHERE node_id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec() || !query.next()) {
            return std::nullopt;
        }

        return NDNodeDetails::Config::Record{
            id,
            query.value(0).toString(),
            static_cast<short>(query.value(1).toInt()),
            static_cast<short>(query.value(2).toInt()),
            QPointF(query.value(3).toDouble(), query.value(4).toDouble()),
            query.value(5).toDouble(),
            query.value(6).toDouble()
        };
    }
    inline std::optional<QList<NDNodeDetails::Config::Record>> getAll(QSqlQuery& query, const bool continueAtFail = true) {
        QList<Config::Record> nodes;
        QString selectAll = "SELECT node_id, title, row_num, col_num, canvas_x, canvas_y, canvas_w, canvas_h FROM nodes;";

        if (!query.exec(selectAll)) {
            qWarning() << "Failed to fetch all nodes:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());

            if (!id) {
                if (continueAtFail) continue;
                else return std::nullopt;
            }

            nodes.append(NDNodeDetails::Config::Record{
                id.value(),
                query.value(1).toString(),
                static_cast<short>(query.value(2).toInt()),
                static_cast<short>(query.value(3).toInt()),
                QPointF(query.value(4).toDouble(), query.value(5).toDouble()),
                query.value(6).toDouble(),
                query.value(7).toDouble()
                });
        }
        return nodes;
    }
}