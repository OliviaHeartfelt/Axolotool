#pragma once

#include "../../../Storage/ANodeEnvDB.h"
#include "../../../Registry/ARegistry.h"
#include "../../cell/VWCell.h"
#include "Context.h"
#include "NodeItem.h"

namespace VWNodeDetails::CreateNode {

    inline NodeItem::Node* createNode(
        ANodeEnvDB::ANodeEnvDB* nodeEnvDB,
        ARegistry::Registry& registry,
        QGraphicsItem* parent,
        const VWNodeDetails::Context::FactoryData& factoryData,
        const bool continueAtFail = false,
        const bool overrideOnCollision = false
    ) {
        if (!nodeEnvDB) return nullptr;

        auto node = std::make_unique<NodeItem::Node>(parent, factoryData.node.coreId, factoryData.node.id);
        if (!node || !node->body) return nullptr;

        const bool isNew = !static_cast<bool>(factoryData.node.id);

        node->setPos(factoryData.node.xPos, factoryData.node.yPos);
        node->body->initGrid(factoryData.node.rowNum, factoryData.node.colNum, false);

        const auto nodeCore = registry.node.nodeCoreRegistry.at(factoryData.node.coreId);
        if (!nodeCore) return nullptr;

        for (const auto& cell : factoryData.nodeCells) {
            if (static_cast<bool>(cell.pinTemplateId) + static_cast<bool>(cell.widgetId) > 1) {
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

            std::unique_ptr<VWCell::CellItem::CellItem> cellItem(VWCell::createCell(nodeEnvDB, registry, node.get(), cellData, nodeCore->cellVisualFallbackId));

            if (!cellItem) {
                if (continueAtFail) continue;
                return nullptr;
            }
            if (!node->body->addItem(cellItem.get(), cell.row, cell.col, cell.rowSpan, cell.colSpan, false, overrideOnCollision)) {
                if (continueAtFail) continue;
                return nullptr;
            }

            cellItem->nodeId(node->id());
            cellItem->name(cell.name);
            cellItem->cellTransforms(cell.row, cell.col, cell.rowSpan, cell.colSpan);
            if (cell.pinTemplateId) {
                cellItem->pinTemplateId(cell.pinTemplateId);
            }
            else if (cell.widgetId) {
                cellItem->widgetId(cell.widgetId);
            }

            registry.nodeView.cellViewRegistry.addVisible(cellItem->id(), cellItem.get());
            qDebug() << "> Cell created! #Cells:" << registry.nodeView.cellViewRegistry.sizeVisible() - 1 << "->" << registry.nodeView.cellViewRegistry.sizeVisible();

            cellItem.release();
        }

        node->body->refresh();
        return node.release();
    }

    inline NodeItem::Node* createNewNode(
        ANodeEnvDB::ANodeEnvDB* nodeEnvDB,
        ARegistry::Registry* registry, 
        QGraphicsItem* parent, 
        const muuid::uuid& coreId, 
        const QPointF pos,
        const bool continueAtFail = false,
        const bool overrideOnCollision = false
    ) {
        if (!nodeEnvDB || !registry) return nullptr;

        std::optional<ANodeEnvDB::Config::Node::FullNodeCoreRecord> coreOpt = registry->node.nodeCoreRegistry.at(coreId);
        if (!coreOpt) {
            ANodeEnvDB::Helpers::useQuery(nodeEnvDB->getPool(), [&](QSqlQuery& query) {
                coreOpt = nodeEnvDB->node.getNodeCore(query, coreId);
            });

            if (coreOpt) {
                registry->node.nodeCoreRegistry.insert(coreId, *coreOpt);
            }
        }
        if (!coreOpt) return nullptr;

        auto nodeGridConfig = NodeItem::NodeGridConfig{
            .margin = 4.0,
            .spacing = 4.0
        };

        auto node = std::make_unique<NodeItem::Node>(parent, coreId, std::nullopt, nodeGridConfig);
        if (!node || !node->body) return nullptr;

        node->setPos(pos.x(), pos.y());
        node->body->initGrid(coreOpt->defaultRowNum, coreOpt->defaultColNum, false);

        auto cellFactory = registry->nodeFunction.cellFactoryRegistry.at(coreId);
        if (!cellFactory) return nullptr;

        const auto cells = (*cellFactory)(node->id());
        for (const auto& cell : cells) {
            const bool hasPin =    cell.pinCoreId.has_value();
            const bool hasWidget = cell.widgetCoreId.has_value();

            if (hasPin && hasWidget) {
                if (continueAtFail) continue;
                return nullptr;
            }

            VWCell::Context::FactoryData cellData;
            cellData.id = std::nullopt;
            cellData.name = cell.name;

            if (hasPin) {
                cellData.pin = VWCell::Context::PinFactoryData{
                    .pinCoreId = *cell.pinCoreId
                };
            }
            else if (hasWidget) {
                cellData.widget = VWCell::Context::WidgetFactoryData{
                    .widgetCoreId = *cell.widgetCoreId,
                    .widgetId = std::nullopt,
                    .state = std::nullopt
                };
            }

            std::unique_ptr<VWCell::CellItem::CellItem> cellItem(VWCell::createCell(nodeEnvDB, *registry, node.get(), cellData, coreOpt->cellVisualFallbackId));

            if (!cellItem) {
                if (continueAtFail) continue;
                return nullptr;
            }

            if (!node->body->addItem(cellItem.get(), cell.row, cell.col, cell.rowSpan, cell.colSpan, false, overrideOnCollision)) {
                if (continueAtFail) continue;
                return nullptr;
            }

            cellItem->nodeId(node->id());
            cellItem->name(cell.name);
            cellItem->cellTransforms(cell.row, cell.col, cell.rowSpan, cell.colSpan);

            if (hasPin) {
                cellItem->pinTemplateId(cell.pinCoreId);
            }
            else if (hasWidget) {
                cellItem->widgetId(cell.widgetCoreId);
            }

            registry->nodeView.cellViewRegistry.addVisible(cellItem->id(), cellItem.get());
            qDebug() << "> Cell created! #Cells:" << registry->nodeView.cellViewRegistry.sizeVisible() - 1 << "->" << registry->nodeView.cellViewRegistry.sizeVisible();

            cellItem.release();
        }

        node->body->refresh();
        return node.release();
    }
}