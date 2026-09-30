#pragma once

#include "../../../Storage/ANodeEnvDB.h"
#include "../../../Registry/ARegistry.h"

namespace VWNodeDetails::Helper {

    inline std::optional<QList<ANodeEnvDB::Config::Cell::FactoryCellRecord>> getNewCellData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, const muuid::uuid& nodeCoreId, const muuid::uuid& nodeId) {
        if (!nodeEnvDB) return std::nullopt;

        QList<ANodeEnvDB::Config::Cell::FactoryCellRecord> cells;
        auto vec = registry.node.cellOriginRegistry.at(nodeCoreId);

        if (!vec.empty()) {
            for (auto& cell : vec) {

                cells.emplace_back(ANodeEnvDB::Config::Cell::FactoryCellRecord{
                    .id = muuid::uuid::generate_unix_time_based(),
                    .nodeId = nodeId,
                    .name = cell.name,
                    .pinCoreId = cell.pinCoreId,
                    .widgetCoreId = cell.widgetCoreId,
                    .row = cell.row,
                    .col = cell.col,
                    .rowSpan = cell.rowSpan,
                    .colSpan = cell.colSpan
                    });
            }
        }
        else {
            auto optOriginCells = nodeEnvDB->cell.getNodeCoreOriginCells(nodeCoreId);
            if (!optOriginCells || optOriginCells->isEmpty()) return std::nullopt;

            cells.reserve(optOriginCells->size());

            for (auto& origincell : *optOriginCells) {
                registry.node.cellOriginRegistry.insert(origincell.nodeCoreId, origincell);
                cells.emplace_back(ANodeEnvDB::Config::Cell::FactoryCellRecord{
                    .id = muuid::uuid::generate_unix_time_based(),
                    .nodeId = nodeId,
                    .name = origincell.name,
                    .pinCoreId = origincell.pinCoreId,
                    .widgetCoreId = origincell.widgetCoreId,
                    .row = origincell.row,
                    .col = origincell.col,
                    .rowSpan = origincell.rowSpan,
                    .colSpan = origincell.colSpan
                    });
            }
        }

        return cells;
    }
}