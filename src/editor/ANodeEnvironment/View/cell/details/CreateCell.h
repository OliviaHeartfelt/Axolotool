#pragma once

#include "../../../Registry/ARegistry.h"
#include "../../../Storage/ANodeEnvDB.h"
#include "CellItem.h"
#include "Context.h"
#include "CreatePin.h"
#include "CreateWidget.h"
#include "Helpers.h"

namespace WVCellDetails::CreateCell {

	inline CellItem::CellItem* createPinCell(
		ANodeEnvDB::ANodeEnvDB* nodeEnvDB,
		ARegistry::Registry& registry,
		QGraphicsItem* node,
		const std::optional<muuid::uuid>& cellId,
		const muuid::uuid& pinCoreId,
		const std::optional<QString>& text = std::nullopt,
		const std::optional<muuid::uuid>& fallbackFunctionId = std::nullopt
	) {
		if (!node) return nullptr;

		auto nodeCell = std::make_unique<CellItem::CellItem>(node, cellId);
		nodeCell->setRect(0, 0, 60, 20);

		const auto* pin = CreatePin::createPinCell(nodeEnvDB, registry, nodeCell.get(), pinCoreId, text);
		if (pin) return nodeCell.release();

		if (Helpers::useFallback(registry, nodeCell.get(), text, fallbackFunctionId)) {
			return nodeCell.release();
		}

		return nullptr;
	}
	
	inline CellItem::CellItem* createWidgetCell(
		ARegistry::Registry& registry,
		QGraphicsItem* node,
		const std::optional<muuid::uuid>& cellId,
		const muuid::uuid& widgetCoreId,
		const std::optional<muuid::uuid>& widgetId = std::nullopt,
		const std::optional<QString>& text = std::nullopt,
		std::optional<std::vector<uint8_t>> state = std::nullopt,
		const std::optional<muuid::uuid>& fallbackFunctionId = std::nullopt
	) {
		if (!node) return nullptr;

		auto nodeCell = std::make_unique<CellItem::CellItem>(node, cellId);
		nodeCell->setRect(0, 0, 60, 20);

		const auto* widget = CreateWidget::createWidgetCell(registry, nodeCell.get(), widgetCoreId, widgetId, text, std::move(state));

		if (widget || Helpers::useFallback(registry, nodeCell.get(), text, fallbackFunctionId)) {
			return nodeCell.release();
		}

		return nullptr;
	}

	inline CellItem::CellItem* createFallbackCell(
		ARegistry::Registry& registry,
		QGraphicsItem* node,
		const std::optional<muuid::uuid>& cellId,
		const std::optional<QString>& text = std::nullopt,
		const std::optional<muuid::uuid>& fallbackFunctionId = std::nullopt
	) {
		if (!node) return nullptr;

		auto nodeCell = std::make_unique<CellItem::CellItem>(node, cellId);
		nodeCell->setRect(0, 0, 60, 20);

		if (Helpers::useFallback(registry, nodeCell.get(), text, fallbackFunctionId)) {
			return nodeCell.release();
		}

		return nullptr;
	}

	inline CellItem::CellItem* createCell(
		ANodeEnvDB::ANodeEnvDB* nodeEnvDB,
		ARegistry::Registry& registry,
		QGraphicsItem* node,
		const Context::FactoryData& cellData,
		const std::optional<muuid::uuid>& fallbackFunctionId = std::nullopt
	) {
		if (!nodeEnvDB) return nullptr;

		if (cellData.pin) {
			return createPinCell(
				nodeEnvDB,
				registry,
				node,
				cellData.id,
				cellData.pin->pinCoreId,
				cellData.name,
				fallbackFunctionId
			);
		}

		if (cellData.widget) {
			return createWidgetCell(
				registry,
				node,
				cellData.id,
				cellData.widget->widgetCoreId,
				cellData.widget->widgetId,
				cellData.name,
				cellData.widget->state,
				fallbackFunctionId
			);
		}

		return createFallbackCell(
			registry,
			node, 
			cellData.id,
			cellData.name,
			fallbackFunctionId
		);
	}
}