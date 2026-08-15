#pragma once

#include "../../../Registry/ARegistry.h"
#include "../../pin/VWPin.h"
#include "CellItem.h"

namespace WVCellDetails::CreatePin {

	inline VWPin::PinItem* createPinCell(
		ARegistry::Registry& registry,
		CellItem::CellItem* cellItem,
		const muuid::uuid& pinCoreId,
		const std::optional<QString>& text = std::nullopt
	) {
		if (!cellItem) return nullptr;

		const auto pinCoreOpt = registry.node.pinCoreRegistry.at(pinCoreId);
		if (!pinCoreOpt) return nullptr;

		auto* pinItem = new VWPin::PinItem(&registry, cellItem, pinCoreId, VWPin::Context::FactoryData{
			.flow = pinCoreOpt->flowId,
			.type = pinCoreOpt->typeId,
			.style = pinCoreOpt->styleId,
			.allowFlowVec = pinCoreOpt->allowFlowList.empty() ? nullptr : &pinCoreOpt->allowFlowList,
			.allowTypeVec = pinCoreOpt->allowTypeList.empty() ? nullptr : &pinCoreOpt->allowTypeList
	    });
		if (!pinItem) return nullptr;

		const auto fn = registry.nodeFunction.pinConfiguratorRegistry.at(pinCoreOpt->visualFactoryId);
		if (!fn) {
			delete pinItem;
			return nullptr;
		}

		VWPin::Context::PinContext pinContext;

		if (pinCoreOpt->flowId) {
			if (const auto flow = registry.node.pinFlowRegistry.at(*pinCoreOpt->flowId)) {
				pinContext.pinflow = VWPin::Context::PinFlowContext{
					.name = flow->name.toStdString(),
					.degree = static_cast<double>(flow->degree)
				};
			}
		}

		if (pinCoreOpt->typeId) {
			if (const auto type = registry.node.pinTypeRegistry.at(*pinCoreOpt->typeId)) {
				pinContext.pinType = VWPin::Context::PinTypeContext{
					.name = type->name.toStdString(),
					.bit_size = type->bit_size
				};
			}
		}

		if (pinCoreOpt->styleId) {
			if (const auto style = registry.node.pinStyleRegistry.at(*pinCoreOpt->styleId)) {
				pinContext.pinStyle = VWPin::Context::PinStyleContext{
					.name = style->name.toStdString(),
					.color = style->color.name(QColor::HexArgb).toStdString(),
					.wireThickness = style->wireThickness
				};
			}
		}

		if (!(*fn)(cellItem, pinItem, std::move(pinContext), text)) {
			delete pinItem;
			return nullptr;
		}

		return pinItem;
	}
}