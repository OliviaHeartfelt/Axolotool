#pragma once

#include "Config.h"
#include "../../details/Helper.h"

#include "../../../Storage/ANodeEnvDB.h"
#include "../../../Registry/ARegistry.h"

namespace STLoadingNodeStreamerDetails::NodeProcessing {

	// Node
	inline std::optional<ANodeEnvDB::Config::Node::FullNodeCoreRecord> processNodeCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const muuid::uuid& nodeCoreId);
	inline bool processNodeType(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Node::FullNodeCoreRecord& nodeCore);
	inline bool processNodeData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Node::FullNodeCoreRecord& nodeCore);
	inline bool processCell(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, Config::NodePayload& payload, const ANodeEnvDB::Config::Cell::FullCellRecord& cell);

	inline bool processNode(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, Config::NodePayload& payload, const muuid::uuid& nodeId) {
		if (!nodeEnvDB) return false;

		const auto nodeOpt = nodeEnvDB->node.getNode(query, nodeId);
		if (!nodeOpt) return false;

		payload.node = *nodeOpt;

		const auto nodeCoreOpt = processNodeCore(nodeEnvDB, registry, query, nodeOpt->coreId);
		if (!nodeCoreOpt) return false;
		if (!processNodeType(nodeEnvDB, registry, query, *nodeCoreOpt) || !processNodeData(nodeEnvDB, registry, query, *nodeCoreOpt)) return false;

		const auto cellsOpt = nodeEnvDB->cell.getAllCells(query, nodeOpt->id, false);
		if (!cellsOpt) return false;

		for (const auto& cell : *cellsOpt) {
			if (processCell(nodeEnvDB, registry, query, payload, cell)) {
				payload.nodeCells.push_back(cell);
			}
			else {
				return false;
			}
		}
		return true;
	}
	inline std::optional<ANodeEnvDB::Config::Node::FullNodeCoreRecord> processNodeCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const muuid::uuid& nodeCoreId) {
		const auto nodeCore = STStreamerDetails::Helper::getOrFetch(registry.node.nodeCoreRegistry, nodeCoreId, [&]() {
			return nodeEnvDB->node.getNodeCore(query, nodeCoreId);
			});

		if (!nodeCore) return std::nullopt;
		return *nodeCore;
	}

	inline bool processNodeType(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Node::FullNodeCoreRecord& nodeCore) {
		if (nodeCore.typeId) {
			return STStreamerDetails::Helper::ensureCached(registry.node.nodeTypeRegistry, *nodeCore.typeId, [&]() {
				return nodeEnvDB->nodeSource.getNodeType(query, *nodeCore.typeId);
				});
		}
		return true;
	}
	inline bool processNodeData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Node::FullNodeCoreRecord& nodeCore) {
		if (nodeCore.dataId) {
			return STStreamerDetails::Helper::ensureCached(registry.node.nodeDataRegistry, *nodeCore.dataId, [&]() {
				return nodeEnvDB->nodeSource.getNodeData(query, *nodeCore.dataId);
				});
		}
		return true;
	}


	// cell
	inline bool processPinTemplate(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const muuid::uuid& pinCoreId);
	inline bool processWidget(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, Config::NodePayload& payload, const muuid::uuid& widgetCoreId);

	inline bool processCell(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, Config::NodePayload& payload, const ANodeEnvDB::Config::Cell::FullCellRecord& cell) {

		if (cell.pinTemplateId) {
			return processPinTemplate(nodeEnvDB, registry, query, *cell.pinTemplateId);
		}
		else if (cell.widgetId) {
			return processWidget(nodeEnvDB, registry, query, payload, *cell.widgetId);
		}
		return true;
	}


	// Pin
	inline std::optional<ANodeEnvDB::Config::Pin::CompletePinCore> processPinCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const muuid::uuid& pinCoreId);
	inline bool processPinFlow(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Pin::CompletePinCore& pinCore);
	inline bool processPinType(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Pin::CompletePinCore& pinCore);
	inline bool processPinStyle(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Pin::CompletePinCore& pinCore);

	inline bool processPinTemplate(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const muuid::uuid& pinCoreId) {
		const auto pinCoreOpt = processPinCore(nodeEnvDB, registry, query, pinCoreId);
		if (!pinCoreOpt) return false;

		return processPinFlow(nodeEnvDB, registry, query, *pinCoreOpt)
			&& processPinType(nodeEnvDB, registry, query, *pinCoreOpt)
			&& processPinStyle(nodeEnvDB, registry, query, *pinCoreOpt);
	}
	inline std::optional<ANodeEnvDB::Config::Pin::CompletePinCore> processPinCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const muuid::uuid& pinCoreId) {
		const auto pinCore = STStreamerDetails::Helper::getOrFetch(registry.node.pinCoreRegistry, pinCoreId, [&]() {
			return nodeEnvDB->pin.getFullPinCore(query, pinCoreId);
			});
		if (!pinCore) return std::nullopt;

		return *pinCore;
	}

	inline bool processPinFlow(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Pin::CompletePinCore& pinCore) {
		if (pinCore.flowId) {
			return STStreamerDetails::Helper::ensureCached(registry.node.pinFlowRegistry, *pinCore.flowId, [&]() {
				return nodeEnvDB->pinSource.getFlow(query, *pinCore.flowId);
				});
		}
		return true;
	}
	inline bool processPinType(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Pin::CompletePinCore& pinCore) {
		if (pinCore.typeId) {
			return STStreamerDetails::Helper::ensureCached(registry.node.pinTypeRegistry, *pinCore.typeId, [&]() {
				return nodeEnvDB->pinSource.getType(query, *pinCore.typeId);
				});
		}
		return true;
	}
	inline bool processPinStyle(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Pin::CompletePinCore& pinCore) {
		if (pinCore.styleId) {
			return STStreamerDetails::Helper::ensureCached(registry.node.pinStyleRegistry, *pinCore.styleId, [&]() {
				return nodeEnvDB->pinSource.getStyle(query, *pinCore.styleId);
				});
		}
		return true;
	}


	// Widget
	inline std::optional<ANodeEnvDB::Config::Widget::FullWidgetCoreRecord> processWidgetCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, Config::NodePayload& payload, const muuid::uuid& widgetCoreId);
	inline bool processWidgetType(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Widget::FullWidgetCoreRecord& widgetCore);
	inline bool processWidgetData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Widget::FullWidgetCoreRecord& widgetCore);

	inline bool processWidget(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, Config::NodePayload& payload, const muuid::uuid& widgetId) {
		const auto widgetOpt = nodeEnvDB->widget.getWidget(query, widgetId);
		if (!widgetOpt) return false;
		payload.widgets.emplace(widgetId, *widgetOpt);

		const auto widgetCoreOpt = processWidgetCore(nodeEnvDB, registry, query, payload, widgetOpt->coreId);
		if (!widgetCoreOpt) return false;

		return processWidgetType(nodeEnvDB, registry, query, *widgetCoreOpt)
			&& processWidgetData(nodeEnvDB, registry, query, *widgetCoreOpt);
	}
	inline std::optional<ANodeEnvDB::Config::Widget::FullWidgetCoreRecord> processWidgetCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, Config::NodePayload& payload, const muuid::uuid& widgetCoreId) {
		const auto widgetCore = STStreamerDetails::Helper::getOrFetch(registry.node.widgetCoreRegistry, widgetCoreId, [&]() {
			return nodeEnvDB->widget.getWidgetCore(query, widgetCoreId);
			});
		if (!widgetCore) return std::nullopt;

		return *widgetCore;
	}

	inline bool processWidgetType(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Widget::FullWidgetCoreRecord& widgetCore) {
		if (widgetCore.typeId) {
			return STStreamerDetails::Helper::ensureCached(registry.node.widgetTypeRegistry, *widgetCore.typeId, [&]() {
				return nodeEnvDB->widgetSource.getWidgetType(query, *widgetCore.typeId);
				});
		}
		return true;
	}
	inline bool processWidgetData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry& registry, QSqlQuery& query, const ANodeEnvDB::Config::Widget::FullWidgetCoreRecord& widgetCore) {
		if (widgetCore.dataId) {
			return STStreamerDetails::Helper::ensureCached(registry.node.widgetDataRegistry, *widgetCore.dataId, [&]() {
				return nodeEnvDB->widgetSource.getWidgetData(query, *widgetCore.dataId); 
			});
		}
		return true;
	}
}