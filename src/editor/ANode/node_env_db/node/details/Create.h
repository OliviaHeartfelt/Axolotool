#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDNodeDetails::Create {

    inline bool create(QSqlQuery& query, const NDNodeDetails::Config::CreateNodeRecord& newNode) {
        query.prepare(R"(
            INSERT INTO node (id,  title,  row_num,  col_num,  canvas_x,  canvas_y,  canvas_w,  canvas_h)
            VALUES (         :id, :title, :row_num, :col_num, :canvas_x, :canvas_y, :canvas_w, :canvas_h);
        )");

        query.bindValue(":id",       Utility::UUID::uuidToBytes(newNode.id));
        query.bindValue(":title",    newNode.title);
        query.bindValue(":row_num",  newNode.rowNum);
        query.bindValue(":col_num",  newNode.colNum);
        query.bindValue(":canvas_x", newNode.pos.x());
        query.bindValue(":canvas_y", newNode.pos.y());
        query.bindValue(":canvas_w", newNode.width ?  *newNode.width :  -1.0);
        query.bindValue(":canvas_h", newNode.height ? *newNode.height : -1.0);

        if (!query.exec()) {
            qWarning() << "Failed to execute create node:" << query.lastError().text();
            return false;
        }
        return true;
    }
}