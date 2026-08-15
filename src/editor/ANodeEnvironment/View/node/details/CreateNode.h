#pragma once

#include "../../../Registry/ARegistry.h"
#include "../../cell/VWCell.h"
#include "Context.h"
#include "NodeItem.h"

namespace VWNodeDetails::CreateNode {

    inline NodeItem::Node* createNode(
        ARegistry::Registry& registry,
        QGraphicsItem* parent,
        const VWNodeDetails::Context::FactoryData& factoryData,
        const bool continueAtFail = false,
        const bool overrideOnCollision = false
    ) {
        if (!parent) return nullptr;

        auto node = std::make_unique<NodeItem::Node>(parent, factoryData.node.coreId, factoryData.node.id);
        if (!node || !node->body) return nullptr;

        const bool isNew = !static_cast<bool>(factoryData.node.id);

        node->setPos(factoryData.node.xPos, factoryData.node.yPos);
        node->body->initGrid(factoryData.node.rowNum, factoryData.node.colNum, false);

        const auto nodeCore = registry.node.nodeCoreRegistry.at(factoryData.node.coreId);
        if (!nodeCore) return nullptr;

        for (const auto& cell : factoryData.nodeCells) {
            if (static_cast<bool>(cell.pinTemplateId) xor static_cast<bool>(cell.widgetId)) {
                if (continueAtFail) continue;
                return nullptr;
            }

            VWCell::Context::FactoryData cellData;

            cellData.id = isNew ? static_cast<std::optional<muuid::uuid>>(std::nullopt) : cell.id;
            
            cellData.name = cell.name;

            if (cell.pinTemplateId) {
                cellData.pin = VWCell::Context::PinFactoryData{
                    .pinCoreId = *cell.pinTemplateId
                };
            }
            else if (cell.widgetId) {
                auto it = factoryData.widgets.find(*cell.widgetId);
                if (it != factoryData.widgets.end()) {
                    cellData.widget = VWCell::Context::WidgetFactoryData{
                        .widgetCoreId = it->second.coreId,
                        .widgetId = isNew ? static_cast<std::optional<muuid::uuid>>(std::nullopt) : cell.widgetId,
                        .state = it->second.state
                    };
                }
            }

            std::unique_ptr<VWCell::CellItem::CellItem> cellItem(VWCell::createCell(registry, node.get(), cellData, nodeCore->cellVisualFallbackId));

            if (!cellItem) {
                if (continueAtFail) continue;
                return nullptr;
            }

            if (!node->body->addItem(cellItem.get(), cell.row, cell.col, cell.rowSpan, cell.colSpan, false, overrideOnCollision)) {
                if (continueAtFail) continue;
                return nullptr;
            }
            cellItem.release();
        }

        node->body->refresh();
        return node.release();
    }
}