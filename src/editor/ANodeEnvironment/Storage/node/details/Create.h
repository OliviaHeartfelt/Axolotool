#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDNodeDetails::Create {

    inline bool createNodeCore(QSqlQuery& query, const NDNodeDetails::Config::CreateNodeCoreRecord& newNodeCore) {
        query.prepare(R"(
            INSERT INTO node_core (id,  contributor_id,  type_id,  data_id,  name,  default_row_num,  default_col_num,  default_node_w,  default_node_h)
            VALUES (              :id, :contributor_id, :type_id, :data_id, :name, :default_row_num, :default_col_num, :default_node_w, :default_node_h);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newNodeCore.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newNodeCore.contributorId));
        query.bindValue(":type_id",        newNodeCore.typeId ? Utility::UUID::uuidToBytes(*newNodeCore.typeId) : QVariant());
        query.bindValue(":data_id",        newNodeCore.dataId ? Utility::UUID::uuidToBytes(*newNodeCore.dataId) : QVariant());

        query.bindValue(":name",            newNodeCore.name);
        query.bindValue(":default_row_num", newNodeCore.defaultRowNum);
        query.bindValue(":default_col_num", newNodeCore.defaultColNum);

        query.bindValue(":default_node_w",  newNodeCore.defaultNodeWidth);
        query.bindValue(":default_node_h",  newNodeCore.defaultNodeHeight);

        if (!query.exec()) {
            qWarning() << "Failed to execute create node core:" << query.lastError().text();
            return false;
        }
        return true;
    }

    template<NDConcepts::ByteConvertible State>
    inline bool createNode(QSqlQuery& query, const NDNodeDetails::Config::CreateNodeRecord<State>& newNode) {
        query.prepare(R"(
            INSERT INTO node (id,  core_id,  name,  row_num,  col_num,  canvas_x,  canvas_y,  node_w,  node_h,  state)
            VALUES (         :id, :core_id, :name, :row_num, :col_num, :canvas_x, :canvas_y, :node_w, :node_h, :state);
        )");

        query.bindValue(":id",       Utility::UUID::uuidToBytes(newNode.id));
        query.bindValue(":core_id",  Utility::UUID::uuidToBytes(newNode.coreId));

        query.bindValue(":name",     newNode.name);
        query.bindValue(":row_num",  newNode.rowNum);
        query.bindValue(":col_num",  newNode.colNum);

        query.bindValue(":canvas_x", newNode.pos.x());
        query.bindValue(":canvas_y", newNode.pos.y());
        query.bindValue(":node_w",   newNode.width.value_or(-1.0));
        query.bindValue(":node_h",   newNode.height.value_or(-1.0));

        query.bindValue(":state", newNode.state ? QVariant(Utility::ByteArray::toQByteArray(newNode.state->classToBytes())) : QVariant());

        if (!query.exec()) {
            qWarning() << "Failed to execute create node:" << query.lastError().text();
            return false;
        }
        return true;
    }
}