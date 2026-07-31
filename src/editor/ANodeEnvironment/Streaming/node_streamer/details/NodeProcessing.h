#pragma once

#include "Config.h"
#include "../../details/Helper.h"

#include "../../../Storage/ANodeEnvDB.h"

namespace STNodeStreamerDetails::NodeProcessing {

	using namespace STStreamerDetails;

	// Node
	inline std::optional<ANodeEnvDB::Config::Node::FullNodeCoreRecord> processNodeCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& nodeCoreId);
	inline bool processNodeType(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Node::FullNodeCoreRecord& nodeCore);
	inline bool processNodeData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Node::FullNodeCoreRecord& nodeCore);
	inline bool processCell(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Cell::FullCellRecord& cell);

	inline bool processNode(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& nodeId) {
		if (!nodeEnvDB) return false;

		const auto nodeOpt = nodeEnvDB->node.getNode(query, nodeId);
		if (!nodeOpt) return false;

		payload.node = *nodeOpt;

		const auto nodeCoreOpt = processNodeCore(nodeEnvDB, query, cache, payload, nodeOpt->coreId);
		if (!nodeCoreOpt) return false;

		if (!processNodeType(nodeEnvDB, query, cache, payload, *nodeCoreOpt) || !processNodeData(nodeEnvDB, query, cache, payload, *nodeCoreOpt)) return false;

		const auto cellsOpt = nodeEnvDB->cell.getAllCells(query, nodeOpt->id, false);
		if (!cellsOpt) return false;

		for (const auto& cell : *cellsOpt) {
			if (!processCell(nodeEnvDB, query, cache, payload, cell)) return false;
		}
		return true;
	}
	inline std::optional<ANodeEnvDB::Config::Node::FullNodeCoreRecord> processNodeCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& nodeCoreId) {
		const ANodeEnvDB::Config::Node::FullNodeCoreRecord* nodeCore = Helper::getOrFetch(cache.nodeCores, nodeCoreId, [&]() {
			return nodeEnvDB->node.getNodeCore(query, nodeCoreId);
			});

		if (nodeCore) {
			payload.nodeCore.emplace(nodeCoreId, *nodeCore);
			return *nodeCore;
		}
		return std::nullopt;
	}

	inline bool processNodeType(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Node::FullNodeCoreRecord& nodeCore) {
		if (nodeCore.typeId) {
			return Helper::hydratePayloadOpt(cache.nodeTypes, payload.nodeType, *nodeCore.typeId, [&]() {
				return nodeEnvDB->nodeSource.getNodeData(query, *nodeCore.typeId);
				});
		}
		return true;
	}
	inline bool processNodeData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Node::FullNodeCoreRecord& nodeCore) {
		if (nodeCore.dataId) {
			return Helper::hydratePayloadOpt(cache.nodeData, payload.nodeData, *nodeCore.dataId, [&]() {
				return nodeEnvDB->nodeSource.getNodeData(query, *nodeCore.dataId);
				});
		}
		return true;
	}


	// cell
	inline bool processPinTemplate(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& pinCoreId);
	inline bool processPinInstance(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& pinCoreId);
	inline bool processWidget(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& widgetCoreId);

	inline bool processCell(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Cell::FullCellRecord& cell) {
		if (cell.pinInstanceId) {
			return processPinInstance(nodeEnvDB, query, cache, payload, *cell.pinInstanceId);
		}
		else if (cell.pinTemplateId) {
			return processPinTemplate(nodeEnvDB, query, cache, payload, *cell.pinTemplateId);
		}
		else if (cell.widgetId) {
			return processWidget(nodeEnvDB, query, cache, payload, *cell.widgetId);
		}
		return true;
	}


	// Pin
	inline std::optional<ANodeEnvDB::Config::Pin::FullPinCoreRecord> processPinCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& pinCoreId);
	inline bool processPinFlow(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Pin::FullPinCoreRecord& pinCore);
	inline bool processPinType(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Pin::FullPinCoreRecord& pinCore);
	inline bool processPinStyle(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Pin::FullPinCoreRecord& pinCore);

	inline bool processPinInstance(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& pinId) {
		const auto pinOpt = nodeEnvDB->pin.getPin(query, pinId);
		if (!pinOpt) return false;
		payload.pins.emplace(pinId, *pinOpt);

		return processPinTemplate(nodeEnvDB, query, cache, payload, pinOpt->coreId);
	}
	inline bool processPinTemplate(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& pinCoreId) {
		const auto pinCoreOpt = processPinCore(nodeEnvDB, query, cache, payload, pinCoreId);
		if (!pinCoreOpt) return false;

		return processPinFlow(nodeEnvDB, query, cache, payload, *pinCoreOpt)
			&& processPinType(nodeEnvDB, query, cache, payload, *pinCoreOpt)
			&& processPinStyle(nodeEnvDB, query, cache, payload, *pinCoreOpt);
	}
	inline std::optional<ANodeEnvDB::Config::Pin::FullPinCoreRecord> processPinCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& pinCoreId) {
		const auto pinCore = Helper::getOrFetch(cache.pinCores, pinCoreId, [&]() {
			return nodeEnvDB->pin.getPinCore(query, pinCoreId);
			});

		if (pinCore) {
			payload.pinCores.emplace(pinCoreId, *pinCore);
			return *pinCore;
		}
		return std::nullopt;
	}

	inline bool processPinFlow(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Pin::FullPinCoreRecord& pinCore) {
		if (pinCore.flowId) {
			return Helper::hydratePayloadMap(cache.pinFlows, payload.pinFlows, *pinCore.flowId, [&]() {
				return nodeEnvDB->pinSource.getFlow(query, *pinCore.flowId);
				});
		}
		return true;
	}
	inline bool processPinType(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Pin::FullPinCoreRecord& pinCore) {
		if (pinCore.typeId) {
			return Helper::hydratePayloadMap(cache.pinTypes, payload.pinTypes, *pinCore.typeId, [&]() {
				return nodeEnvDB->pinSource.getType(query, *pinCore.typeId);
				});
		}
		return true;
	}
	inline bool processPinStyle(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Pin::FullPinCoreRecord& pinCore) {
		if (pinCore.styleId) {
			return Helper::hydratePayloadMap(cache.pinStyles, payload.pinStyles, *pinCore.styleId, [&]() { return nodeEnvDB->pinSource.getStyle(query, *pinCore.styleId); });
		}
		return true;
	}


	// Widget
	inline std::optional<ANodeEnvDB::Config::Widget::FullWidgetCoreRecord> processWidgetCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& widgetCoreId);
	inline bool processWidgetType(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Widget::FullWidgetCoreRecord& widgetCore);
	inline bool processWidgetData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Widget::FullWidgetCoreRecord& widgetCore);

	inline bool processWidget(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& widgetId) {
		const auto widgetOpt = nodeEnvDB->widget.getWidget(query, widgetId);
		if (!widgetOpt) return false;
		payload.widgets.emplace(widgetId, *widgetOpt);

		const auto widgetCoreOpt = processWidgetCore(nodeEnvDB, query, cache, payload, widgetOpt->coreId);
		if (!widgetCoreOpt) return false;

		return processWidgetType(nodeEnvDB, query, cache, payload, *widgetCoreOpt)
			&& processWidgetData(nodeEnvDB, query, cache, payload, *widgetCoreOpt);
	}
	inline std::optional<ANodeEnvDB::Config::Widget::FullWidgetCoreRecord> processWidgetCore(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const muuid::uuid& widgetCoreId) {
		const auto widgetCore = Helper::getOrFetch(cache.widgetCores, widgetCoreId, [&]() {
			return nodeEnvDB->widget.getWidgetCore(query, widgetCoreId);
			});

		if (widgetCore) {
			payload.widgetCores.emplace(widgetCoreId, *widgetCore);
			return *widgetCore;
		}
		return std::nullopt;
	}

	inline bool processWidgetType(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Widget::FullWidgetCoreRecord& widgetCore) {
		if (widgetCore.typeId) {
			return Helper::hydratePayloadMap(cache.widgetTypes, payload.widgetTypes, *widgetCore.typeId, [&]() {
				return nodeEnvDB->widgetSource.getWidgetType(query, *widgetCore.typeId);
				});
		}
		return true;
	}
	inline bool processWidgetData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, Config::StreamCache& cache, Config::NodePayload& payload, const ANodeEnvDB::Config::Widget::FullWidgetCoreRecord& widgetCore) {
		if (widgetCore.dataId) {
			return Helper::hydratePayloadMap(cache.widgetData, payload.widgetData, *widgetCore.dataId, [&]() { 
				return nodeEnvDB->widgetSource.getWidgetData(query, *widgetCore.dataId); 
			});
		}
		return true;
	}
}