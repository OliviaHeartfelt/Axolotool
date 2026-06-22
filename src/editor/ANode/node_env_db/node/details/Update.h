#pragma once

#include "../../../../Utility/Utility.h"

namespace NDNodeDetails::Update {

    inline bool updateGeometry(QSqlQuery& query, const muuid::uuid& id, const QPointF& pos, double w = -1.0, double h = -1.0) {
        const bool updateDimensions = (w >= 0.0 && h >= 0.0);

        if (updateDimensions)
            query.prepare(R"(
                UPDATE nodes 
                SET canvas_x = :x, canvas_y = :y, canvas_w = :w, canvas_h = :h 
                WHERE node_id = :id;
            )");
        else
            query.prepare(R"(
                UPDATE nodes 
                SET canvas_x = :x, canvas_y = :y
                WHERE node_id = :id;
            )");

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));
        query.bindValue(":x", pos.x());
        query.bindValue(":y", pos.y());

        if (updateDimensions) {
            query.bindValue(":w", w);
            query.bindValue(":h", h);
        }

        if (!query.exec()) {
            qWarning() << "Failed to update node geometry:" << query.lastError().text();
            return false;
        }
        return true;
    }
}