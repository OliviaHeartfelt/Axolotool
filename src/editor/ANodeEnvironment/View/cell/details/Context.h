#pragma once

namespace WVCellDetails::Context {

	struct PinFactoryData {
		muuid::uuid pinCoreId;
	};
	struct WidgetFactoryData {
		muuid::uuid widgetCoreId;
		std::optional<muuid::uuid> widgetId = std::nullopt;
		std::optional<std::vector<uint8_t>> state = std::nullopt;
	};

	struct FactoryData {
		std::optional<muuid::uuid> id;
		std::optional<PinFactoryData> pin = std::nullopt;
		std::optional<WidgetFactoryData> widget = std::nullopt;
		std::optional<QString> name;
	};
}