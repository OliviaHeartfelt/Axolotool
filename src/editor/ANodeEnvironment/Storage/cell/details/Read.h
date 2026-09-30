#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDCellDetails::Read {

    inline std::optional<NDCellDetails::Config::FullCellRecord> getCell(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT node_id, name, layout_row, layout_col, layout_row_span, layout_col_span, pin_core_id, widget_id
            FROM cell 
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

        const NDHelpers::NullableField<QString> name = NDHelpers::parseNullableQstring(query.value(1));
        if (name.isCorrupted()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> pinCoreId = NDHelpers::parseNullableUUID(query.value(6));
        if (pinCoreId.isCorrupted()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> widgetId = NDHelpers::parseNullableUUID(query.value(7));
        if (widgetId.isCorrupted()) return std::nullopt;

        return NDCellDetails::Config::FullCellRecord{
            id,
            *nodeId,
            name.value,
            static_cast<short>(query.value(2).toInt()),
            static_cast<short>(query.value(3).toInt()),
            static_cast<short>(query.value(4).toInt()),
            static_cast<short>(query.value(5).toInt()),
            pinCoreId.value,
            widgetId.value
        };
    }
    inline std::optional<QList<Config::FullCellRecord>> getAllCells(QSqlQuery& query, const muuid::uuid& nodeId, const bool continueAtFail = true) {
        QList<Config::FullCellRecord> cells;
        query.prepare(R"(
            SELECT id, name, layout_row, layout_col, layout_row_span, layout_col_span, pin_core_id, widget_id
            FROM cell 
            WHERE node_id = :node_id;
        )");
        query.bindValue(":node_id", Utility::UUID::uuidToBytes(nodeId));

        if (!query.exec()) {
            qWarning() << "Failed to fetch cells for node:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            auto cellIdOpt = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const NDHelpers::NullableField<QString> name = NDHelpers::parseNullableQstring(query.value(1));

            const NDHelpers::NullableField<muuid::uuid> pinCoreId = NDHelpers::parseNullableUUID(query.value(6));
            const NDHelpers::NullableField<muuid::uuid> widgetId = NDHelpers::parseNullableUUID(query.value(7));

            if (!cellIdOpt || name.isCorrupted() || pinCoreId.isCorrupted() || widgetId.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            cells.append(Config::FullCellRecord{
                *cellIdOpt,
                nodeId,
                name.value,
                static_cast<short>(query.value(2).toInt()),
                static_cast<short>(query.value(3).toInt()),
                static_cast<short>(query.value(4).toInt()),
                static_cast<short>(query.value(5).toInt()),
                pinCoreId.value,
                widgetId.value
                });
        }
        return cells;
    }

    inline std::optional<QList<Config::FullCellOriginRecord>> getNodeCoreOriginCells(QSqlQuery& query, const muuid::uuid& nodeCoreId, const bool continueAtFail = true) {
        QList<Config::FullCellOriginRecord> cells;

        query.prepare(R"(
            SELECT id, node_core_id, name, layout_row, layout_col, layout_row_span, layout_col_span, pin_core_id, widget_core_id
            FROM cell_origin 
            WHERE node_core_id = :node_core_id;
        )");
        query.bindValue(":node_core_id", Utility::UUID::uuidToBytes(nodeCoreId));

        if (!query.exec()) {
            qWarning() << "Failed to fetch origin cells:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            auto nodeCoreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const NDHelpers::NullableField<QString> name = NDHelpers::parseNullableQstring(query.value(2));
            const NDHelpers::NullableField<muuid::uuid> pinCoreId = NDHelpers::parseNullableUUID(query.value(7));
            const NDHelpers::NullableField<muuid::uuid> widgetCoreId = NDHelpers::parseNullableUUID(query.value(8));

            if (!id || !nodeCoreId || name.isCorrupted() || pinCoreId.isCorrupted() || widgetCoreId.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            cells.append(Config::FullCellOriginRecord{
                *id,
                *nodeCoreId,
                name.value,
                pinCoreId.value,
                widgetCoreId.value,
                static_cast<short>(query.value(3).toInt()),
                static_cast<short>(query.value(4).toInt()),
                static_cast<short>(query.value(5).toInt()),
                static_cast<short>(query.value(6).toInt())
                });
        }
        return cells;
    }
}