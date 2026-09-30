#pragma once

#include "Config.h"
#include "Helper.h"

namespace NDCellDetails::Update {

    inline bool updateCell(QSqlQuery& query, const muuid::uuid& id, const NDCellDetails::Config::UpdateCellRecord& newCellInfo, const bool overrideOnCollision = false) {

        const auto* optPinCorePtr = std::get_if<std::optional<muuid::uuid>>(&newCellInfo.pinCoreId);
        const auto* optWidgetPtr = std::get_if<std::optional<muuid::uuid>>(&newCellInfo.widgetId);

        if (optPinCorePtr->has_value() && optWidgetPtr->has_value()) {
            qWarning() << "Cell update rejected: A cell slot cannot contain more than one of pin core or widget.";
            return false;
        }

        if (newCellInfo.row || newCellInfo.col || newCellInfo.rowSpan || newCellInfo.colSpan) {
            query.prepare(R"(
                SELECT node_id, layout_row, layout_col, layout_row_span, layout_col_span 
                FROM cell 
                WHERE id = :id;
            )");
            query.bindValue(":id", Utility::UUID::uuidToBytes(id));

            if (!query.exec() || !query.next()) {
                qWarning() << "Cell layout update rejected: Failed to fetch current layout state.";
                return false;
            }

            const std::optional<muuid::uuid> nodeId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            if (!nodeId) return false;

            short row = newCellInfo.row ? *newCellInfo.row : static_cast<short>(query.value(1).toInt());
            short col = newCellInfo.col ? *newCellInfo.col : static_cast<short>(query.value(2).toInt());
            short rowSpan = newCellInfo.rowSpan ? *newCellInfo.rowSpan : static_cast<short>(query.value(3).toInt());
            short colSpan = newCellInfo.colSpan ? *newCellInfo.colSpan : static_cast<short>(query.value(4).toInt());

            if (!NDCellDetails::Helper::isCellAvailable(query, *nodeId, row, col, rowSpan, colSpan, id)) {
                if (!overrideOnCollision) {
                    qWarning() << "Cell layout update rejected: Target region is occupied.";
                    return false;
                }
                if (!NDCellDetails::Helper::removeCollidingCells(query, *nodeId, row, col, rowSpan, colSpan)) return false;
            }
        }

        QStringList clauses;

        if (newCellInfo.id)     clauses.append("id = :new_id");
        if (newCellInfo.nodeId) clauses.append("node_id = :new_node_id");

        const auto* optNamePtr = std::get_if<std::optional<QString>>(&newCellInfo.name);
        if (optNamePtr) clauses.append("name = :name");

        if (optPinCorePtr) clauses.append("pin_core_id = :pin_core_id");
        if (optWidgetPtr)  clauses.append("widget_id = :widget_id");

        if (newCellInfo.row && *newCellInfo.row >= 0)         clauses.append("layout_row = :row");
        if (newCellInfo.col && *newCellInfo.col >= 0)         clauses.append("layout_col = :col");
        if (newCellInfo.rowSpan && *newCellInfo.rowSpan >= 1) clauses.append("layout_row_span = :row_span");
        if (newCellInfo.colSpan && *newCellInfo.colSpan >= 1) clauses.append("layout_col_span = :col_span");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE cell SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newCellInfo.id)     query.bindValue(":new_id", Utility::UUID::uuidToBytes(*newCellInfo.id));
        if (newCellInfo.nodeId) query.bindValue(":new_node_id", Utility::UUID::uuidToBytes(*newCellInfo.nodeId));

        if (optNamePtr)    query.bindValue(":name", optNamePtr->has_value() ? QVariant(optNamePtr->value()) : QVariant(QMetaType::fromType<QString>()));
        if (optPinCorePtr) query.bindValue(":pin_core_id", optPinCorePtr->has_value() ? QVariant(Utility::UUID::uuidToBytes(optPinCorePtr->value())) : QVariant(QMetaType::fromType<QByteArray>()));
        if (optWidgetPtr)  query.bindValue(":widget_id", optWidgetPtr->has_value() ? QVariant(Utility::UUID::uuidToBytes(optWidgetPtr->value())) : QVariant(QMetaType::fromType<QByteArray>()));

        if (newCellInfo.row) query.bindValue(":row", *newCellInfo.row);
        if (newCellInfo.col) query.bindValue(":col", *newCellInfo.col);
        if (newCellInfo.rowSpan && *newCellInfo.rowSpan >= 1) query.bindValue(":row_span", *newCellInfo.rowSpan);
        if (newCellInfo.colSpan && *newCellInfo.colSpan >= 1) query.bindValue(":col_span", *newCellInfo.colSpan);

        if (!query.exec()) {
            qWarning() << "Failed to update cell:" << query.lastError().text();
            return false;
        }
        return true;
    }

    inline bool updateCellOrigin(QSqlQuery& query, const muuid::uuid& id, const NDCellDetails::Config::UpdateCellOriginRecord& newCellInfo) {

        const auto* optPinCorePtr = std::get_if<std::optional<muuid::uuid>>(&newCellInfo.pinCoreId);
        const auto* optWidgetCorePtr = std::get_if<std::optional<muuid::uuid>>(&newCellInfo.widgetCoreId);

        if (optPinCorePtr->has_value() && optWidgetCorePtr->has_value()) {
            qWarning() << "Cell update rejected: A cell slot cannot contain more than one of pin core or widget.";
            return false;
        }

        if (newCellInfo.row || newCellInfo.col || newCellInfo.rowSpan || newCellInfo.colSpan) {
            query.prepare(R"(
                SELECT node_core_id, layout_row, layout_col, layout_row_span, layout_col_span 
                FROM cell_origin 
                WHERE id = :id;
            )");
            query.bindValue(":id", Utility::UUID::uuidToBytes(id));

            if (!query.exec() || !query.next()) {
                qWarning() << "Cell origin layout update rejected: Failed to fetch current layout state.";
                return false;
            }

            const std::optional<muuid::uuid> nodeCoreId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            if (!nodeCoreId) return false;

            short row = newCellInfo.row ? *newCellInfo.row : static_cast<short>(query.value(1).toInt());
            short col = newCellInfo.col ? *newCellInfo.col : static_cast<short>(query.value(2).toInt());
            short rowSpan = newCellInfo.rowSpan ? *newCellInfo.rowSpan : static_cast<short>(query.value(3).toInt());
            short colSpan = newCellInfo.colSpan ? *newCellInfo.colSpan : static_cast<short>(query.value(4).toInt());

            if (!NDCellDetails::Helper::isCellOriginAvailable(query, *nodeCoreId, row, col, rowSpan, colSpan, id)) {
                qWarning() << "Cell layout update rejected: Target region is occupied.";
                return false;
            }
        }

        QStringList clauses;

        if (newCellInfo.id)         clauses.append("id = :new_id");
        if (newCellInfo.nodeCoreId) clauses.append("node_id = :new_node_id");

        const auto* optNamePtr = std::get_if<std::optional<QString>>(&newCellInfo.name);
        if (optNamePtr) clauses.append("name = :name");

        if (optPinCorePtr)    clauses.append("pin_core_id = :pin_core_id");
        if (optWidgetCorePtr) clauses.append("widget_core_id = :widget_core_id");

        if (newCellInfo.row && *newCellInfo.row >= 0)         clauses.append("layout_row = :row");
        if (newCellInfo.col && *newCellInfo.col >= 0)         clauses.append("layout_col = :col");
        if (newCellInfo.rowSpan && *newCellInfo.rowSpan >= 1) clauses.append("layout_row_span = :row_span");
        if (newCellInfo.colSpan && *newCellInfo.colSpan >= 1) clauses.append("layout_col_span = :col_span");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE cell_origin SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newCellInfo.id)         query.bindValue(":new_id", Utility::UUID::uuidToBytes(*newCellInfo.id));
        if (newCellInfo.nodeCoreId) query.bindValue(":new_node_id", Utility::UUID::uuidToBytes(*newCellInfo.nodeCoreId));

        if (optNamePtr)       query.bindValue(":name", optNamePtr->has_value() ? QVariant(optNamePtr->value()) : QVariant(QMetaType::fromType<QString>()));
        if (optPinCorePtr)    query.bindValue(":pin_core_id", optPinCorePtr->has_value() ? QVariant(Utility::UUID::uuidToBytes(optPinCorePtr->value())) : QVariant(QMetaType::fromType<QByteArray>()));
        if (optWidgetCorePtr) query.bindValue(":widget_core_id", optWidgetCorePtr->has_value() ? QVariant(Utility::UUID::uuidToBytes(optWidgetCorePtr->value())) : QVariant(QMetaType::fromType<QByteArray>()));

        if (newCellInfo.row) query.bindValue(":row", *newCellInfo.row);
        if (newCellInfo.col) query.bindValue(":col", *newCellInfo.col);
        if (newCellInfo.rowSpan && *newCellInfo.rowSpan >= 1) query.bindValue(":row_span", *newCellInfo.rowSpan);
        if (newCellInfo.colSpan && *newCellInfo.colSpan >= 1) query.bindValue(":col_span", *newCellInfo.colSpan);

        if (!query.exec()) {
            qWarning() << "Failed to update cell origin:" << query.lastError().text();
            return false;
        }
        return true;
    }
}