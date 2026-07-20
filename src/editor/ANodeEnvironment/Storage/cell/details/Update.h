#pragma once

#include "Config.h"
#include "Helper.h"

namespace NDCellDetails::Update {

    inline bool updateCell(QSqlQuery& query, const muuid::uuid& id, const NDCellDetails::Config::UpdateCellRecord& newCellInfo, const bool overrideOnCollision = false) {

        const auto* optPinTemplatePtr = std::get_if<std::optional<muuid::uuid>>(&newCellInfo.pinTemplateId);
        const auto* optPinInstancePtr = std::get_if<std::optional<muuid::uuid>>(&newCellInfo.pinInstanceId);
        const auto* optWidgetPtr =      std::get_if<std::optional<muuid::uuid>>(&newCellInfo.widgetId);

        int activeInputsCount = 0;
        if (optPinTemplatePtr->has_value()) activeInputsCount++;
        if (optPinInstancePtr->has_value()) activeInputsCount++;
        if (optWidgetPtr->has_value())      activeInputsCount++;

        if (activeInputsCount > 1) {
            qWarning() << "Cell update rejected: A cell slot cannot contain more than one of pin_template, pin_instance, or widget.";
            return false;
        }

        const bool layoutChanged = (newCellInfo.row || newCellInfo.col || newCellInfo.rowSpan || newCellInfo.colSpan);
        const bool willSetHidden = newCellInfo.isOut && *newCellInfo.isOut;

        if (!willSetHidden) {
            query.prepare("SELECT layout_row, layout_col, layout_row_span, layout_col_span, node_id, is_out FROM node_cells WHERE id = :id;");
            query.bindValue(":id", Utility::UUID::uuidToBytes(id));

            if (!query.exec() || !query.next()) {
                qWarning() << "Cell layout update rejected: Failed to fetch current layout state.";
                return false;
            }

            const std::optional<muuid::uuid> nodeId = Utility::UUID::bytesToUuid(query.value(4).toByteArray());
            if (!nodeId) return false;

            const bool currentlyHidden = query.value(5).toInt() == 1;
            const bool willBeVisible = newCellInfo.isOut ? !(*newCellInfo.isOut) : !currentlyHidden;
            const bool transitionToVisible = currentlyHidden && willBeVisible;

            if (layoutChanged || transitionToVisible) {
                const NDCellDetails::Config::CellInfo info{
                    newCellInfo.row ? *newCellInfo.row : static_cast<short>(query.value(0).toInt()),
                    newCellInfo.col ? *newCellInfo.col : static_cast<short>(query.value(1).toInt()),
                    newCellInfo.rowSpan ? *newCellInfo.rowSpan : static_cast<short>(query.value(2).toInt()),
                    newCellInfo.colSpan ? *newCellInfo.colSpan : static_cast<short>(query.value(3).toInt())
                };

                if (!NDCellDetails::Helper::isCellAvailable(query, *nodeId, info, id)) {
                    if (!overrideOnCollision) {
                        qWarning() << "Cell layout update rejected: Target region is occupied.";
                        return false;
                    }
                    if (!NDCellDetails::Helper::removeCollidingCells(query, *nodeId, info)) return false;
                }
            }
        }

        QStringList clauses;

        if (newCellInfo.id)     clauses.append("id = :new_id");
        if (newCellInfo.nodeId) clauses.append("node_id = :new_node_id");

        const auto* optNamePtr = std::get_if<std::optional<QString>>(&newCellInfo.name);
        if (optNamePtr) clauses.append("name = :name");

        if (optPinTemplatePtr) clauses.append("pin_template_id = :pin_template_id");
        if (optPinInstancePtr) clauses.append("pin_instance_id = :pin_instance_id");
        if (optWidgetPtr)      clauses.append("widget_id = :widget_id");

        if (newCellInfo.row && *newCellInfo.row >= 0)         clauses.append("layout_row = :row");
        if (newCellInfo.col && *newCellInfo.col >= 0)         clauses.append("layout_col = :col");
        if (newCellInfo.rowSpan && *newCellInfo.rowSpan >= 1) clauses.append("layout_row_span = :row_span");
        if (newCellInfo.colSpan && *newCellInfo.colSpan >= 1) clauses.append("layout_col_span = :col_span");

        if (newCellInfo.isOut)    clauses.append("is_out = :is_out");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE node_cells SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newCellInfo.id)     query.bindValue(":new_id",      Utility::UUID::uuidToBytes(*newCellInfo.id));
        if (newCellInfo.nodeId) query.bindValue(":new_node_id", Utility::UUID::uuidToBytes(*newCellInfo.nodeId));

        if (optNamePtr)        query.bindValue(":name", optNamePtr->has_value()                   ? QVariant(optNamePtr->value())                                    : QVariant(QMetaType::fromType<QString>()));
        if (optPinTemplatePtr) query.bindValue(":pin_template_id", optPinTemplatePtr->has_value() ? QVariant(Utility::UUID::uuidToBytes(optPinTemplatePtr->value())) : QVariant(QMetaType::fromType<QByteArray>()));
        if (optPinInstancePtr) query.bindValue(":pin_instance_id", optPinInstancePtr->has_value() ? QVariant(Utility::UUID::uuidToBytes(optPinInstancePtr->value())) : QVariant(QMetaType::fromType<QByteArray>()));
        if (optWidgetPtr)      query.bindValue(":widget_id", optWidgetPtr->has_value()            ? QVariant(Utility::UUID::uuidToBytes(optWidgetPtr->value()))      : QVariant(QMetaType::fromType<QByteArray>()));

        if (newCellInfo.row && *newCellInfo.row >= 0)         query.bindValue(":row", *newCellInfo.row);
        if (newCellInfo.col && *newCellInfo.col >= 0)         query.bindValue(":col", *newCellInfo.col);
        if (newCellInfo.rowSpan && *newCellInfo.rowSpan >= 1) query.bindValue(":row_span", *newCellInfo.rowSpan);
        if (newCellInfo.colSpan && *newCellInfo.colSpan >= 1) query.bindValue(":col_span", *newCellInfo.colSpan);

        if (newCellInfo.isOut)    query.bindValue(":is_out", *newCellInfo.isOut ? 1 : 0);

        if (!query.exec()) {
            qWarning() << "Failed to update cell:" << query.lastError().text();
            return false;
        }
        return true;
    }
}