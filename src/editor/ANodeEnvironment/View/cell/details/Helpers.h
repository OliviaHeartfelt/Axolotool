#pragma once

#include "../../../Registry/ARegistry.h"

namespace WVCellDetails::Helpers {

	inline bool useFallback(
		ARegistry::Registry& registry,
		QGraphicsRectItem* nodeCell,
		const std::optional<QString>& text,
		const std::optional<muuid::uuid>& fallbackFunctionId
	) {
		if (!fallbackFunctionId) return false;

		const auto fallbackFunction = registry.nodeFunction.cellFallbackConfiguratorRegistry.at(*fallbackFunctionId);
		if (!fallbackFunction) return false;

		const bool success = (*fallbackFunction)(nodeCell, text);
		return success;
	}
}