#pragma once

#include "ANodeEnvironment/ANodeEnvironment.h"

namespace MockPlugin {

	inline bool initPlugin(ANodeEnvironment::ANodeEnvironment* nodeEnv) {
		using namespace ANodeEnvironment::Storage::Config;

		if (!nodeEnv) return false;
		auto& nodeRegistry = nodeEnv->registry();

        // Wire Visual Factory ------------------------------------------------------------------------------------------------------------------------------------------

        const auto createWireConfiguratorID = muuid::uuid::from_chars("01a01708-add2-75ac-b870-db4c3ee26920");
        if (!createWireConfiguratorID) return false;
        std::function<bool(QGraphicsItem* wire, VWWireDetails::Context::WireContext wireContext)> wireConfigurator = [&](QGraphicsItem* wire, VWWireDetails::Context::WireContext wireContext) {

            //wireContext.name;

            if (wireContext.style) {
                auto wireItem = dynamic_cast<QGraphicsPathItem*>(wire);
                if (!wireItem) return false;

                QPen pen;
                pen.setColor(wireContext.style->color);
                pen.setWidthF(wireContext.style->wireThickness);

                pen.setCapStyle(Qt::RoundCap);
                pen.setJoinStyle(Qt::RoundJoin);

                wireItem->setPen(pen);
            }
            if (wireContext.data) {
                //wireContext.data->data
            }

            return true;
            };
        if (!nodeRegistry.wireFunction.wireConfiguratorRegistry.insert(*createWireConfiguratorID, wireConfigurator)) return false;


        // pinConfiguratorRegistry ------------------------------------------------------------------------------------------------------------------------------------------
        auto pinConfiguratorID = muuid::uuid::from_chars("01a01038-f084-7167-a126-1523448df0b3");
        if (!pinConfiguratorID) return false;

        auto pinConfiguratorFn = [&](QGraphicsRectItem* nodeCell, QGraphicsItem* pin, VWPinDetails::Context::PinContext pinContext, const std::optional<QString> text) -> bool {

            const auto m_createPinInFlowID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-73302c1d52e7");
            const auto m_createPinOutFlowID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-6d477e5dbd7d");

            if (!nodeCell || !pin || !m_createPinInFlowID || !m_createPinOutFlowID) return false;

            const QRectF rawPinRect = pin->boundingRect();
            const QRectF visualPinRect = rawPinRect.adjusted(8.0, 8.0, -8.0, -8.0);

            QGraphicsTextItem* label = new QGraphicsTextItem(nodeCell);
            label->setDefaultTextColor("#d0d0d0");
            label->setFlag(QGraphicsItem::ItemIsMovable, false);
            label->setFlag(QGraphicsItem::ItemIsSelectable, false);
            label->document()->setDocumentMargin(0);

            if (text) label->setPlainText(*text);

            constexpr qreal cellHeight = 20.0;
            constexpr qreal spacing = 6.0;

            // ---------------- IN-FLOW PIN ----------------
            if (pinContext.pinflow && pinContext.pinflow->id == *m_createPinInFlowID) {
                qreal pinX = 0.0 - visualPinRect.left();
                qreal pinY = (cellHeight / 2.0) - rawPinRect.center().y();
                pin->setPos(pinX, pinY);

                qreal pinVisualRight = pinX + visualPinRect.right();
                qreal labelX = pinVisualRight + spacing;
                qreal labelY = (cellHeight / 2.0) - label->boundingRect().center().y();
                label->setPos(labelX, labelY);

                qreal totalWidth = labelX + label->boundingRect().width() + spacing;
                nodeCell->setRect(0, 0, totalWidth, cellHeight);

                return true;
            }

            // ---------------- OUT-FLOW PIN ----------------
            if (pinContext.pinflow && pinContext.pinflow->id == *m_createPinOutFlowID) {
                qreal labelX = spacing;
                qreal labelY = (cellHeight / 2.0) - label->boundingRect().center().y();
                label->setPos(labelX, labelY);

                qreal labelRight = labelX + label->boundingRect().width();
                qreal pinX = (labelRight + spacing) - visualPinRect.left();
                qreal pinY = (cellHeight / 2.0) - rawPinRect.center().y();
                pin->setPos(pinX, pinY);

                qreal totalWidth = pinX + visualPinRect.right();
                nodeCell->setRect(0, 0, totalWidth, cellHeight);

                return true;
            }

            delete label;
            return false;
            };
        nodeRegistry.nodeFunction.pinConfiguratorRegistry.insert(*pinConfiguratorID, pinConfiguratorFn);


        // Node Core ------------------------------------------------------------------------------------------------------------------------------------------

        // Node Function Registry ------------------------------------------------------------------------------------------------------------------------------------------
        bool cellFactoryRegistrySuccess = true;

        // --- Node A Factory ---
        const auto createNodeACoreId = muuid::uuid::from_chars("01a01024-8932-7727-9b20-30a7f81ad76a");
        if (!createNodeACoreId) return false;
        std::function<std::vector<ARegistryDetails::Context::CreateCellRecord>(const muuid::uuid& nodeId)> NodeAFn = [&](const muuid::uuid& nodeId) {
            std::vector<ARegistryDetails::Context::CreateCellRecord> cells;

            auto m_createPinIntOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-1434f616f62f");
            if (!m_createPinIntOutCoreID) return cells;

            cells.emplace_back(ARegistryDetails::Context::CreateCellRecord{
                .id = muuid::uuid::generate_unix_time_based(),
                .nodeId = nodeId,
                .name = "Node A Out",
                .pinCoreId = *m_createPinIntOutCoreID,
                .widgetCoreId = std::nullopt,
                .row = 0,
                .col = 0,
                .rowSpan = 1,
                .colSpan = 1
                });
            return cells;
            };
        if (!nodeRegistry.nodeFunction.cellFactoryRegistry.insert(*createNodeACoreId, NodeAFn)) cellFactoryRegistrySuccess = false;

        // --- Node B Factory ---
        const auto createNodeBCoreId = muuid::uuid::from_chars("01a01029-f8e6-73e9-9db3-926c8ad12f52");
        if (!createNodeBCoreId) return false;
        std::function<std::vector<ARegistryDetails::Context::CreateCellRecord>(const muuid::uuid& nodeId)> NodeBFn = [&](const muuid::uuid& nodeId) {
            std::vector<ARegistryDetails::Context::CreateCellRecord> cells;

            auto m_createPinIntInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-19435996d0ed");
            auto m_createPinIntOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-1434f616f62f");
            if (!m_createPinIntInCoreID || !m_createPinIntOutCoreID) return cells;

            cells.emplace_back(ARegistryDetails::Context::CreateCellRecord{
                .id = muuid::uuid::generate_unix_time_based(),
                .nodeId = nodeId,
                .name = "Node B In",
                .pinCoreId = *m_createPinIntInCoreID,
                .widgetCoreId = std::nullopt,
                .row = 0,
                .col = 0,
                .rowSpan = 1,
                .colSpan = 1
                });
            cells.emplace_back(ARegistryDetails::Context::CreateCellRecord{
                .id = muuid::uuid::generate_unix_time_based(),
                .nodeId = nodeId,
                .name = "Node B Out",
                .pinCoreId = *m_createPinIntOutCoreID,
                .widgetCoreId = std::nullopt,
                .row = 0,
                .col = 1,
                .rowSpan = 1,
                .colSpan = 1
                });

            return cells;
            };
        if (cellFactoryRegistrySuccess && !nodeRegistry.nodeFunction.cellFactoryRegistry.insert(*createNodeBCoreId, NodeBFn)) cellFactoryRegistrySuccess = false;

        // --- Node C Factory ---
        const auto createNodeCCoreId = muuid::uuid::from_chars("01a01029-f8e6-73e9-9db3-964174d5c085");
        if (!createNodeCCoreId) return false;
        std::function<std::vector<ARegistryDetails::Context::CreateCellRecord>(const muuid::uuid& nodeId)> NodeCFn = [&](const muuid::uuid& nodeId) {
            std::vector<ARegistryDetails::Context::CreateCellRecord> cells;

            auto m_createPinIntInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-19435996d0ed");
            if (!m_createPinIntInCoreID) return cells;

            cells.emplace_back(ARegistryDetails::Context::CreateCellRecord{
                .id = muuid::uuid::generate_unix_time_based(),
                .nodeId = nodeId,
                .name = "Node C In",
                .pinCoreId = *m_createPinIntInCoreID,
                .widgetCoreId = std::nullopt,
                .row = 0,
                .col = 0,
                .rowSpan = 1,
                .colSpan = 1
                });
            return cells;
            };
        if (cellFactoryRegistrySuccess && !nodeRegistry.nodeFunction.cellFactoryRegistry.insert(*createNodeCCoreId, NodeCFn)) cellFactoryRegistrySuccess = false;

        return true;
    }
}