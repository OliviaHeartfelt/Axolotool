#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDNodeDetails::Update {

    inline bool updateNodeCore(QSqlQuery& query, const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeCoreRecord& newProperties) {
        QStringList clauses;

        if (newProperties.id)            clauses.append("id = :new_id");
        if (newProperties.contributorId) clauses.append("contributor_id = :new_contributor_id");

        const auto* typeIdPtr = std::get_if<std::optional<muuid::uuid>>(&newProperties.typeId);
        if (typeIdPtr) clauses.append("type_id = :type_id");

        const auto* dataIdPtr = std::get_if<std::optional<muuid::uuid>>(&newProperties.dataId);
        if (dataIdPtr) clauses.append("data_id = :data_id");

        const auto* fallbackIdPtr = std::get_if<std::optional<muuid::uuid>>(&newProperties.cellVisualFallbackId);
        if (fallbackIdPtr) clauses.append("cell_visual_fallback_id = :cell_visual_fallback_id");

        if (newProperties.name)   clauses.append("name = :name");

        if (newProperties.defaultRowNum)     clauses.append("default_row_num = :default_row_num");
        if (newProperties.defaultColNum)     clauses.append("default_col_num = :default_col_num");
        if (newProperties.defaultNodeWidth)  clauses.append("default_node_w = :default_node_w");
        if (newProperties.defaultNodeHeight) clauses.append("default_node_h = :default_node_h");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE node_core SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)            query.bindValue(":new_id",             Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.contributorId) query.bindValue(":new_contributor_id", Utility::UUID::uuidToBytes(*newProperties.contributorId));

        if (typeIdPtr)     query.bindValue(":type_id", typeIdPtr->has_value()                     ? QVariant(Utility::UUID::uuidToBytes(typeIdPtr->value()))     : QVariant(QMetaType::fromType<QByteArray>()));
        if (dataIdPtr)     query.bindValue(":data_id", dataIdPtr->has_value()                     ? QVariant(Utility::UUID::uuidToBytes(dataIdPtr->value()))     : QVariant(QMetaType::fromType<QByteArray>()));
        if (fallbackIdPtr) query.bindValue(":cell_visual_fallback_id", fallbackIdPtr->has_value() ? QVariant(Utility::UUID::uuidToBytes(fallbackIdPtr->value())) : QVariant(QMetaType::fromType<QByteArray>()));

        if (newProperties.name)              query.bindValue(":name", *newProperties.name);
        if (newProperties.defaultRowNum)     query.bindValue(":default_row_num", *newProperties.defaultRowNum);
        if (newProperties.defaultColNum)     query.bindValue(":default_col_num", *newProperties.defaultColNum);
        if (newProperties.defaultNodeWidth)  query.bindValue(":default_node_w", *newProperties.defaultNodeWidth);
        if (newProperties.defaultNodeHeight) query.bindValue(":default_node_h", *newProperties.defaultNodeHeight);

        if (!query.exec()) {
            qWarning() << "Failed to update node core:" << query.lastError().text();
            return false;
        }
        return true;
    }

    inline bool updateNode(QSqlQuery& query, const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeRecord& newProperties) {
        QStringList clauses;

        if (newProperties.id)     clauses.append("id = :new_id");
        if (newProperties.coreId) clauses.append("core_id = :new_core_id");

        if (newProperties.name)   clauses.append("name = :name");
        if (newProperties.rowNum) clauses.append("row_num = :row_num");
        if (newProperties.colNum) clauses.append("col_num = :col_num");
        if (newProperties.pos)    clauses.append("canvas_x = :canvas_x, canvas_y = :canvas_y");
        if (newProperties.width)  clauses.append("node_w = :node_w");
        if (newProperties.height) clauses.append("node_h = :node_h");

        const auto* optPtr = std::get_if<std::optional<std::vector<uint8_t>>>(&newProperties.state);
        if (optPtr) clauses.append("state = :state");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE node SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)     query.bindValue(":new_id",      Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.coreId) query.bindValue(":new_core_id", Utility::UUID::uuidToBytes(*newProperties.coreId));

        if (newProperties.name)   query.bindValue(":name",    *newProperties.name);
        if (newProperties.rowNum) query.bindValue(":row_num", *newProperties.rowNum);
        if (newProperties.colNum) query.bindValue(":col_num", *newProperties.colNum);

        if (newProperties.pos) {
            query.bindValue(":canvas_x", newProperties.pos->x());
            query.bindValue(":canvas_y", newProperties.pos->y());
        }

        if (newProperties.width)  query.bindValue(":node_w", *newProperties.width);
        if (newProperties.height) query.bindValue(":node_h", *newProperties.height);

        if (optPtr) query.bindValue(":state", optPtr->has_value() ? QVariant(Utility::ByteArray::toQByteArray(optPtr->value())) : QVariant());

        if (!query.exec()) {
            qWarning() << "Failed to update node:" << query.lastError().text();
            return false;
        }
        return true;
    }
}