#pragma once

#include "../../../Registry/ARegistry.h"
#include "../../../Storage/ANodeEnvDB.h"
#include "../../pin/VWPin.h"
#include "CellItem.h"

namespace WVCellDetails::CreatePin {

	inline VWPin::PinItem* createPinCell(
		ANodeEnvDB::ANodeEnvDB* nodeEnvDB,
		ARegistry::Registry& registry,
		CellItem::CellItem* cellItem,
		const muuid::uuid& pinCoreId,
		const std::optional<QString>& text = std::nullopt
	) {
		if (!cellItem) return nullptr;

		auto pinCoreOpt = registry.node.pinCoreRegistry.at(pinCoreId);
		if (!pinCoreOpt) {

			if (!nodeEnvDB) return nullptr;

			ANodeEnvDB::Helpers::useQuery(nodeEnvDB->getPool(), [&](QSqlQuery& query) {
				pinCoreOpt = nodeEnvDB->pin.getFullPinCore(query, pinCoreId);
			});

			if (pinCoreOpt) {
				registry.node.pinCoreRegistry.insert(pinCoreId, *pinCoreOpt);
			}
		}

		if (!pinCoreOpt) return nullptr;

		auto* pinItem = new VWPin::PinItem(&registry, nodeEnvDB, cellItem, pinCoreId, VWPin::Context::FactoryData{
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
			auto flow = registry.node.pinFlowRegistry.at(*pinCoreOpt->flowId);
			if (!flow) {
				ANodeEnvDB::Helpers::useQuery(nodeEnvDB->getPool(), [&](QSqlQuery& query) {
					flow = nodeEnvDB->pinSource.getFlow(query, *pinCoreOpt->flowId);
				});

				if (flow) {
					registry.node.pinFlowRegistry.insert(*pinCoreOpt->flowId, *flow);
				}
			}
			if (flow) {
				pinContext.pinflow = VWPin::Context::PinFlowContext{
					.id = *pinCoreOpt->flowId,
					.name = flow->name.toStdString(),
					.degree = static_cast<double>(flow->degree)
				};
			}
		}

		if (pinCoreOpt->typeId) {
			auto type = registry.node.pinTypeRegistry.at(*pinCoreOpt->typeId);
			if (!type) {
				ANodeEnvDB::Helpers::useQuery(nodeEnvDB->getPool(), [&](QSqlQuery& query) {
					type = nodeEnvDB->pinSource.getType(query, *pinCoreOpt->typeId);
				});

				if (type) {
					registry.node.pinTypeRegistry.insert(*pinCoreOpt->typeId, *type);
				}
			}
			if (type) {
				pinContext.pinType = VWPin::Context::PinTypeContext{
					.id = *pinCoreOpt->typeId,
					.name = type->name.toStdString(),
					.bit_size = type->bit_size
				};
			}
		}

		if (pinCoreOpt->styleId) {
			auto style = registry.node.pinStyleRegistry.at(*pinCoreOpt->styleId);
			if (!style) {
				ANodeEnvDB::Helpers::useQuery(nodeEnvDB->getPool(), [&](QSqlQuery& query) {
					style = nodeEnvDB->pinSource.getStyle(query, *pinCoreOpt->styleId);
				});

				if (style) {
					registry.node.pinStyleRegistry.insert(*pinCoreOpt->styleId, *style);
				}
			}
			if (style) {
				pinContext.pinStyle = VWPin::Context::PinStyleContext{
					.id = *pinCoreOpt->styleId,
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