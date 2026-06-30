#pragma once

namespace NDPinDetails::Config {

	struct FullPinRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
		QList<muuid::uuid> allowFlowList = {};
		QList<muuid::uuid> allowTypeList = {};
	};
	struct PinRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
	};
	struct CreatePinRecord {
		muuid::uuid contributorId;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
		QList<muuid::uuid> allowFlowList = {};
		QList<muuid::uuid> allowTypeList = {};
	};
	struct UpdatePinRecord {
		std::variant<std::monostate, std::optional<muuid::uuid>> flowId = std::monostate{};
		std::variant<std::monostate, std::optional<muuid::uuid>> typeId = std::monostate{};
		std::variant<std::monostate, std::optional<muuid::uuid>> styleId = std::monostate{};
	};
}