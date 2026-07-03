#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDNodeDetails::Create {

    inline std::optional<muuid::uuid> create(QSqlQuery& query, const NDNodeDetails::Config::CreateNodeRecord& newNode) {
        query.prepare(R"(
            INSERT INTO nodes (id,  title,  row_num,  col_num,  canvas_x,  canvas_y,  canvas_w,  canvas_h)
            VALUES (          :id, :title, :row_num, :col_num, :canvas_x, :canvas_y, :canvas_w, :canvas_h);
        )");
        muuid::uuid newNodeId = muuid::uuid::generate_unix_time_based();

        query.bindValue(":id",       Utility::UUID::uuidToBytes(newNodeId));
        query.bindValue(":title",    newNode.title);
        query.bindValue(":row_num",  newNode.rowNum);
        query.bindValue(":col_num",  newNode.colNum);
        query.bindValue(":canvas_x", newNode.pos.x());
        query.bindValue(":canvas_y", newNode.pos.y());
        query.bindValue(":canvas_w", newNode.width ?  *newNode.width :  -1.0);
        query.bindValue(":canvas_h", newNode.height ? *newNode.height : -1.0);

        if (!query.exec()) {
            qWarning() << "Failed to execute create node:" << query.lastError().text();
            return std::nullopt;
        }
        return newNodeId;
    }
}