#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDCellDetails::Read {

    inline std::optional<NDCellDetails::Config::FullCellRecord> getCell(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT node_id, name, is_out, layout_row, layout_col, layout_row_span, layout_col_span, pin_template_id, pin_instance_id, widget_id
            FROM node_cells 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to fetch cell:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        auto nodeId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!nodeId) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> pinTemplateId = NDHelpers::parseNullableUUID(query.value(7));
        if (pinTemplateId.isCorrupted()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> pinInstanceId = NDHelpers::parseNullableUUID(query.value(8));
        if (pinInstanceId.isCorrupted()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> widgetId = NDHelpers::parseNullableUUID(query.value(9));
        if (widgetId.isCorrupted()) return std::nullopt;

        return NDCellDetails::Config::FullCellRecord{
            id,
            *nodeId,
            query.value(1).toString(),
            query.value(2).toBool(),
            static_cast<short>(query.value(3).toInt()),
            static_cast<short>(query.value(4).toInt()),
            static_cast<short>(query.value(5).toInt()),
            static_cast<short>(query.value(6).toInt()),
            pinTemplateId.value,
            pinInstanceId.value,
            widgetId.value
        };
    }

    inline std::optional<QList<Config::FullCellRecord>> getAllCells(QSqlQuery& query, const muuid::uuid& nodeId, const bool continueAtFail = true) {
        QList<Config::FullCellRecord> cells;
        query.prepare(R"(
            SELECT id, name, is_out, layout_row, layout_col, layout_row_span, layout_col_span, pin_template_id, pin_instance_id, widget_id
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
            const NDHelpers::NullableField<muuid::uuid> pinTemplateId = NDHelpers::parseNullableUUID(query.value(7));
            const NDHelpers::NullableField<muuid::uuid> pinInstanceId = NDHelpers::parseNullableUUID(query.value(8));
            const NDHelpers::NullableField<muuid::uuid> widgetId = NDHelpers::parseNullableUUID(query.value(9));

            if (!cellIdOpt || pinTemplateId.isCorrupted() || pinInstanceId.isCorrupted() || widgetId.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
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
                pinTemplateId.value,
                pinInstanceId.value,
                widgetId.value
            });
        }
        return cells;
    }
}